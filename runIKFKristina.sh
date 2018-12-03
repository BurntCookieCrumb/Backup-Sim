#!/bin/bash
#date "+DATE: %Y%m%d_%H%M%S"


PROGRAMM=$1
HISTOPREFIX=$2
HOMEDIR=`pwd`
NUMBEROFTHREADS=9
CONFIGFOLDERS=(configTune0 configTune6 configTune14 configTune21)
NUMBEROFFOLDERS=4

for KILLPID in `ps -U $USER | grep '${PROGRAMM}' | awk ' { print $1;}'`; do
    kill -9 $KILLPID;
done

for CONFIG in `seq 0 ${NUMBEROFFOLDERS-1}`
do
    cp $PROGRAMM ${CONFIGFOLDERS[$CONFIG]}
done

while [ 1 ]; do
    for i in `seq 0 $NUMBEROFTHREADS`
    do
        cd ${CONFIGFOLDERS[$(($i % $NUMBEROFFOLDERS))]}
        echo "running config ${CONFIGFOLDERS[$(($i % $NUMBEROFFOLDERS))]}"
        mkdir -p $HISTOPREFIX
        ./$PROGRAMM $HISTOPREFIX`date "+%Y%m%d_%H%M%S"` > `date "+%Y%m%d_%H%M%S"`.log  &
        sleep 1
        cd $HOMEDIR
    done
    wait
    rm *.log
    echo "$NUMBEROFTHREADS jobs done"
done
