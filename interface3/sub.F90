subroutine sub(a,m,x)
implicit none
real a,x(3)
integer m
include "interfaces"

x    = a
x(m) = 8.0

return
end subroutine sub
