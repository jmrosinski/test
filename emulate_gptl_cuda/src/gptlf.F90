module gptl
  implicit none
  public

! Function prototypes

  interface
    integer function gptlstart (arg)
      integer :: arg
    end function gptlstart
    
    integer function gptlstop (arg)
      integer :: arg
    end function gptlstop

    integer function gptlcudadevsync () bind(C,name="cudaDeviceSynchronize")
    end function gptlcudadevsync
  end interface
end module gptl
