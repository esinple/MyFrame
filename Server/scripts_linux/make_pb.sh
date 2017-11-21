#!/bin/bash

for file in ../common/command/*
do
    if test -f $file
    then
        echo $file
        ../bin/protoc -I=../common/command/ --cpp_out=../src/netproto/ $file
    fi
done

