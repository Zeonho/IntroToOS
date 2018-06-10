COUNT=0
n=0
while [ $COUNT -lt 10 ];do
    # echo -n $COUNT
    let n=COUNT-1
    while [ $n -ge 0 ];do
      echo -n $n
      let n-=1
    done
    echo
    let COUNT+=1
  done
