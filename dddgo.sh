#!/bin/bash 
HAHA=`date | awk '{print $4}'`
ddd ./test.out $PKEY $PSECRET $HAHA
