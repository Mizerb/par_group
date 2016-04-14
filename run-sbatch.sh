#!/bin/bash

NODES="64 128"
THREADS="1 4 16 64"
MODES=""
MATRIX="131072"

mkdir -p logs
mkdir -p out


for THREAD in $THREADS;
do
  for NODE in $NODES;
  do
    MPI=`expr 64 / $THREAD`
    TOTAL=`expr $MPI "*" $NODE`
    
    SBATCH_FILE="sbatch-N$NODE-T$THREAD.sh"
    echo "#!/bin/sh" > $SBATCH_FILE
    echo "#SBATCH --job-name=ASS4_PCP" >> $SBATCH_FILE
    echo "#SBATCH -D `pwd`" >> $SBATCH_FILE
    #echo "#SBATCH -d singleton" >> $SBATCH_FILE
    echo "#SBATCH -t 60" >> $SBATCH_FILE
    echo "#SBATCH -p medium" >> $SBATCH_FILE
    echo "#SBATCH -N 128" >> $SBATCH_FILE
    echo "#SBATCH -O" >> $SBATCH_FILE
    echo "#SBATCH -o output-N$NODE-T$THREAD.log" >> $SBATCH_FILE
    echo "#SBATCH --ntasks-per-node=$MPI" >> $SBATCH_FILE
    if [ $NODE = "128" ];
    then
      echo "srun -N$NODE -n$TOTAL -o logs/r$NODE-$THREAD.log ./assignment4.xl $MATRIX $THREAD" >> $SBATCH_FILE
      
      sbatch $SBATCH_FILE
    else
      NODE2=`expr $NODE / 2`
      TOTAL2=`expr $MPI "*" $NODE2`
      
      echo "srun -N$NODE -n$TOTAL -o logs/r$NODE-$THREAD.log ./assignment4.xl $MATRIX $THREAD &" >> $SBATCH_FILE
      echo "srun -N$NODE2 -n$TOTAL2 -o logs/r$NODE2-$THREAD.log ./assignment4.xl $MATRIX $THREAD &" >> $SBATCH_FILE
      echo "wait" >> $SBATCH_FILE
      
      sbatch $SBATCH_FILE
    fi

    
#    for IOMODE in $MODES;
#    do
    
#    done
  done
done
