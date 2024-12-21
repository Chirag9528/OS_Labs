#!/bin/bash

for ((i=1; i<=500; i++))
do
   ./a.out 10
done

for ((i=1 ; i<=500; i++))
do
    ./b.out 10
done
