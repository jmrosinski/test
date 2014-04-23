subroutine sub2(a,m,x)
implicit none
real a,x(4)
integer m
include "interfaces"

x    = a
x(m) = 8.0

return
end subroutine sub2
