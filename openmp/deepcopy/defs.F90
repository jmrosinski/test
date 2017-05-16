module defs
  type nested_stuff
    real :: z
  end type nested_stuff

  type tobecopied
     integer :: i
     real, pointer :: x(:)
     type (nested_stuff), pointer :: y(:)
  end type tobecopied
end module defs
