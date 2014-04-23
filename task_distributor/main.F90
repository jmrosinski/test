program main
  implicit none
#include <mpif.h>

  integer :: cpn = 4           ! cores per node (default 4)
  integer :: nwt = 0           ! number of write tasks (default none)
  integer :: mwtpn = 4         ! max write tasks per node (default cpn)
  logical :: root_own_node = .false. ! whether root is on a node by itself (default true)

  integer :: i, ii, n       ! indices 
  integer :: ierr           ! MPI return
  integer :: resultlen      ! length of name (returned by MPI_GET_PROCESSOR_NAME 
  integer :: coresleft      ! number of cores remaining on node after write tasks assigned 
  integer :: num_tasks_world          ! number of MPI tasks in WORLD 
  integer, allocatable :: proclist(:)      ! list of MPI ranks 
  integer :: iam_world            ! rank in MPI_COMM_WORLD 
  integer :: iamnew         ! rank in comm 
  integer :: inc
  integer :: looplim
  integer :: begwriterank = -1  ! rank of 1st write task
  integer :: endwriterank = -1  ! rank of last write task

  integer :: mpi_group_world ! world
  integer :: group = -1      ! group changes size
  integer :: fimcomm_with_holes = -1       ! communicator 
  integer :: total_comm = -1
  integer :: status
  integer, parameter :: tmp_comm = MPI_COMM_WORLD
  integer, parameter :: tag = 999

  character(len=MPI_MAX_PROCESSOR_NAME+1) :: mynode = ' '
  character(len=MPI_MAX_PROCESSOR_NAME+1), allocatable :: nodes(:)
  character(len=MPI_MAX_ERROR_STRING) :: string

  logical, parameter :: abort_on_bad_task_distrib = .true.

  call mpi_init (ierr)
  call mpi_comm_size (tmp_comm, num_tasks_world, ierr)
  call mpi_comm_rank (tmp_comm, iam_world, ierr)
  call mpi_get_processor_name (mynode, resultlen, ierr)

  if (iam_world == 0) then
    write(6,*) 'Enter cores per node'
    read (5,*) cpn

    write(6,*) 'Enter number of write tasks'
    read (5,*) nwt

    write(6,*) 'Enter max write tasks per node'
    read (5,*) mwtpn

    write(6,*) 'Enter whether root is to be on his own node (.true. or .false.)'
    read (5,*) root_own_node

    write(6,*) 'cpn=', cpn
    write(6,*) 'nwt=', nwt
    write(6,*) 'mwtpn=', mwtpn
    write(6,*) 'root_own_node=', root_own_node
  end if

  call  mpi_bcast (cpn, 1, MPI_INTEGER, 0, tmp_comm, ierr)
  call  mpi_bcast (nwt, 1, MPI_INTEGER, 0, tmp_comm, ierr)
  call  mpi_bcast (mwtpn, 1, MPI_INTEGER, 0, tmp_comm, ierr)
  call  mpi_bcast (root_own_node, 1, MPI_LOGICAL, 0, tmp_comm, ierr)

  allocate (nodes(0:2*cpn-1))            ! Check 1st 2 nodes
  allocate (proclist(0:num_tasks_world-1))     ! Ranks start at 0
  nodes(:) = ' '

!JR Gather node name info for 1st 2 nodes to check conformance with expected node allocation

  looplim = min (num_tasks_world-1, 2*cpn-1)
  if (iam_world == 0) then
    nodes(0) = mynode
    do i=1,looplim
      call mpi_recv (nodes(i), MPI_MAX_PROCESSOR_NAME, MPI_CHARACTER, &
                     i, tag, tmp_comm, status, ierr)
    end do
  else if (iam_world <= looplim) then
    call mpi_send (mynode, MPI_MAX_PROCESSOR_NAME, MPI_CHARACTER, &
                   0, tag, tmp_comm, ierr)
  end if

  if (iam_world == 0 .and. num_tasks_world > cpn) then
    if (nodes(0) == nodes(cpn)) then
      write(6,*)'core_setup_fim: MPI task 0 name=',trim(nodes(0)),' matches task ',cpn, &
                ' name=',trim(nodes(cpn)),' cpn=',cpn
      write(6,*)'Perhaps namelist value for cpn', (cpn),' is incorrect?'
      if (abort_on_bad_task_distrib) then
        write(6,*)'abort_on_bad_task_distrib is true so model is aborting...'
        call mpi_abort (MPI_COMM_WORLD, istatus, ignore)
        stop
      end if
    end if
  end if

!JR First: Set proclist for 1st node. Contents depend on whether root has a node to himself
  
  proclist(0) = 0  ! Root task always same in both communictors
  if (root_own_node) then
    i = cpn         ! Move pointer to 1st core of next node
    ii = 1          ! Set proclist index for next entry
  else              ! Fill remainder of node 0 with compute tasks
    looplim = min (num_tasks_world-1, cpn-1)
    ii = 1          ! Set proclist index for next entry
    do i=1,looplim
      proclist(ii) = i
      if (iam_world == 0 .and. nodes(i-1) /= nodes(i)) then
        write(6,*)'core_setup_fim: MPI task ',i-1,' name=',trim(nodes(i-1)), &
                  ' does not match task ',i,' name=',trim(nodes(i)),' cpn=',cpn
        if (abort_on_bad_task_distrib) then
          write(6,*)'abort_on_bad_task_distrib is true so model is aborting...'
          call mpi_abort (MPI_COMM_WORLD, istatus, ignore)
          stop
        end if
      end if
      ii = ii + 1
    end do
    i = cpn
  end if

  ! Add write tasks to proclist. "i" currently points to 1st core of 2nd node

  if (nwt > 0) then
    begwriterank = i
  end if

  do n=1,nwt
    if (i >= num_tasks_world) then
      if (iam_world == 0) then
	write(6,*)'Ran out of MPI tasks assigning write task=',n,'. i=',i
      end if
      call mpi_abort()
      stop
    end if
    proclist(ii) = i
    endwriterank = i
    if (mod(n,mwtpn) == 0) then  ! Move pointer to start of next node
      inc = cpn - mwtpn + 1
    else                         ! Haven't filled up the node with write tasks yet
      inc = 1
    end if
    i = i + inc
    ii = ii + 1
  end do

  ! Compute tasks after write tasks start at next empty node
  
  if (mod (i, cpn) /= 0) then
    coresleft = cpn - mod (i, cpn)
    i = i + coresleft
  end if

  ! Remainder of proclist is compute tasks 

  do while (i < num_tasks_world)
    proclist(ii) = i
    i = i + 1
    ii = ii + 1
  end do

  ! Define the new communicator. ii is the size of the group

  call mpi_comm_group (tmp_comm, mpi_group_world, ierr)            ! create group=world
  call mpi_group_incl (mpi_group_world, ii, proclist, group, ierr) ! set contents
  call mpi_comm_create (tmp_comm, group, fimcomm_with_holes, ierr)

  if (ierr /= 0) then
    call mpi_error_string (ierr, string, resultlen)
    write(6,*)'Error in mpi_comm_create:',string(1:resultlen)
    call mpi_abort()
  else if (fimcomm_with_holes /= MPI_COMM_NULL) then
    call mpi_comm_rank (fimcomm_with_holes, iamnew, ierr)
    if (iamnew == 0) then
      write(6,*) 'Root task is running on ', trim(mynode)
    else if (iamnew < begwriterank .or. iamnew > endwriterank) then
      write(6,'(a,i2,a,i2,a,a)') 'Compute task: WORLD rank ',iam_world, &
           ' = newcomm rank ', iamnew, ' node=', trim(mynode)
    else 
      write(6,'(a,i2,a,i2,a,a)') 'Write task: WORLD rank ',iam_world, &
           ' = newcomm rank ', iamnew, ' node=', trim(mynode)
    end if
  else
!JR Do nothing: just wait till mpi_finalize is called by others in the communicator
    call mpi_finalize (ierr)
    stop
  end if

  call mpi_finalize (ierr)
  stop
end program main
