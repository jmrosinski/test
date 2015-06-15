program main
  implicit none

  integer, parameter :: myrank = 0
  integer :: cpn
  integer :: pin2core
  integer :: ret

  integer, external :: print_affinity, set_affinity

  write(6,*)'Initial affinity:'
  ret = print_affinity (myrank)

  do while (.true.)
    write(6,*)'Enter cores per node'
    read(5,*) cpn
    write(6,*)'Enter 1 to pin to a core, 0 to pin to a range'
    read(5,*) pin2core

    ret = set_affinity (myrank, cpn, cpn, pin2core)
    write(6,*)'Affinity after pinning for cpn=',cpn,' pin2core=',pin2core
    ret = print_affinity (myrank)
  end do
end program main
