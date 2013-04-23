#!/bin/bash 
HAHA=`date | awk '{print $4}'`
./test.out $PKEY $PSECRET $HAHA
#echo $PKEY $PSECRET $HAHA
