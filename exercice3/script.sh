#!/bin/zsh

count=0.0
msg="a"
ans=""
brouillage=0.0
space=" "

for inc in `seq 1 50`; do
  count=0
  brouillage=$(($brouillage + 0.1))
  echo $brouillage
  for i in `seq 1 1000`; do
    ans=$(echo $msg | ./programme_1 | ./programme_2 | ./programme_3 | ./programme_4 $brouillage | ./programme_5 | ./programme_6 | ./programme_7 | ./programme_8)
    if [ "$msg" = "$ans" ]; then
      count=$(($count + 1.0))
    fi
  done
  count=$(($count / 1000.0))
  echo "$brouillage $count \n" >> results
done
