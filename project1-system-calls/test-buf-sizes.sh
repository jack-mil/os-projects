#!/usr/bin/env bash

cp_cmd=./fcopy

# create data file of 20mb
data=test_data.bin
data_size=20480
dd if=/dev/zero of=$data bs=1024 count=0 seek=$data_size
echo "Created file of size $data_size"

# clear log file before use
echo -n > test.log

# loop through test block sizes and time program
# save to log file
for ((i,j=1;i<15;i+=1,j*=2))
do
    echo "BUFFER SIZE:" $j >> test.log
    (time $cp_cmd $data junk $j) >> test.log 2>&1
    /usr/bin/rm -f junk
done

/usr/bin/rm -f $data