module external_interfaces
  implicit none
  public
  
  interface 
    subroutine sub (u_vel)
      real :: u_vel(:)
    end subroutine sub
  end interface
end module external_interfaces
