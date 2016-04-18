#!/bin/bash

NODES="64 128"
THREADS="01 04 16 64"
MODES="0 1 2 3"
MATRIX="131072"

mkdir -p logs
mkdir -p out


for THREAD in $THREADS;
do
  for NODE in $NODES;
  do
    MPI=`expr 64 / $THREAD`
    TOTAL=`expr $MPI "*" $NODE`
    for IOMODE in $MODES;
    do
      
      SBATCH_FILE="sbatch/sbatch-N$NODE-T$THREAD-M$IOMODE.sh"
      LOG_FILE="logs/$IOMODE-$NODE-$THREAD"
      ERR_FILE="err/err-N$NODE-T$THREAD-M$IOMODE.log"
      PREFIX=`expr "(" "(" "(" $NODE "*" 4 ")" + $THREAD ")" "*" 4 ")" + $IOMODE`

      echo "#!/bin/sh" > $SBATCH_FILE
      echo "#SBATCH --job-name=ASS4_PCP" >> $SBATCH_FILE
      echo "#SBATCH -D `pwd`" >> $SBATCH_FILE
      #echo "#SBATCH -d singleton" >> $SBATCH_FILE
      echo "#SBATCH -t 60" >> $SBATCH_FILE
      echo "#SBATCH -p medium" >> $SBATCH_FILE
      echo "#SBATCH -N 128" >> $SBATCH_FILE
      echo "#SBATCH -O" >> $SBATCH_FILE
      echo "#SBATCH -o $ERR_FILE" >> $SBATCH_FILE
      echo "#SBATCH --ntasks-per-node=$MPI" >> $SBATCH_FILE
      if [ $NODE = "128" ];
      then
        echo "srun -N$NODE -n$TOTAL -o $LOG_FILE ./assignment4.xl $MATRIX $THREAD $IOMODE $PREFIX" >> $SBATCH_FILE        
      else
        NODE2=`expr $NODE / 2`
        TOTAL2=`expr $MPI "*" $NODE2`
        LOG_FILE2="logs/$IOMODE-$NODE2-$THREAD"
        PREFIX2=`expr "(" "(" "(" $NODE2 "*" 4 ")" + $THREAD ")" "*" 4 ")" + $IOMODE`
        
        echo "srun -N$NODE -n$TOTAL -o $LOG_FILE ./assignment4.xl $MATRIX $THREAD $IOMODE $PREFIX &" >> $SBATCH_FILE
        echo "srun -N$NODE2 -n$TOTAL2 -o $LOG_FILE2 ./assignment4.xl $MATRIX $THREAD $IOMODE $PREFIX2 &" >> $SBATCH_FILE
        echo "wait" >> $SBATCH_FILE
        
      fi
      sbatch $SBATCH_FILE
    done
  done
done
