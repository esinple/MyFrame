#!/bin/bash

if [ ! -d "../log/" ] ; then
    mkdir ../log
fi

project_name=(SuperServer GameServer GatewayServer )
#project_name=(GameServer)
if [ ! -n "$1" -o "$1" == "debug" ] ; then
    for file in ${project_name[*]}
    do
        echo "start ${file}_Debug"
        ../bin/${file}_Debug > ../log/asan_${file} 2>&1 &
    done
else 
    for file in ${project_name[*]}
    do
        echo "start ${file}_Release"
        ../bin/${file}_Release &
    done
fi

./show_server.sh
