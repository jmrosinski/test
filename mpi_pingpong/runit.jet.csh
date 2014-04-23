#!/bin/csh

foreach skip ( 5 )
# normal-mic or development
#foreach skip ( 1 2 )
  foreach oneatatime ( 0 1 )
    set append = s_${skip}.o_${oneatatime}
    echo "Running with -s $skip -o $oneatatime ..."
    mpiexec -n 12 "./a.out -s $skip -o $oneatatime" |& tee out.$append
    mv timing.summary timing.summary.$append
    foreach i ( timing.[0-9] timing.[0-9][0-9] )
      mv $i ${i}.$append
    end
  end
end
exit 0
