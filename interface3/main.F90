program main
implicit none
real a,x(3),y(4)
integer m
include "interfaces"

m=2
a=3.0
call sub(a,m,x)
print*,x
call sub2(a,m,y)
print*,y

end program main
