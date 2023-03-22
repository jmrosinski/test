module gptl_acc
  implicit none
  public

! Function prototypes/bindings

  interface
    integer function gptlstart_gpu (handle) bind(C,name="GPTLstart_gpu")
      use iso_c_binding, only: c_int
      integer(c_int), intent(in), VALUE :: handle
!$acc routine seq
    end function gptlstart_gpu

    integer function gptlstop_gpu (handle) bind(C,name="GPTLstop_gpu")
      use iso_c_binding, only: c_int
      integer(c_int), intent(in), VALUE :: handle
!$acc routine seq
    end function gptlstop_gpu
  end interface
end module gptl_acc
