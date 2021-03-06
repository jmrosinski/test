subroutine handle_err (ret)
  use netcdf
  implicit none

  integer, intent(in) :: ret
  
  if (ret /= NF90_NOERR) then
    write(6,*) nf90_strerror (ret)
    stop 999
  end if
  return
end subroutine handle_err
