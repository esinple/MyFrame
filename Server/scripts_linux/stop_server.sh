#!/bin/bash

project_name=(GatewayServer GameServer SuperServer)
sig=2
if [ ! -n "$1" ] ; then
	sig=2
else
	sig=$1
fi

echo sig is $sig

for file in ${project_name[*]}
do
	pid=`ps x|grep ${file}|grep -v grep|awk '{print $1}'`
	if [ ! -n "$pid" ] ; then
		continue
	fi
	echo ${file} pid is ${pid}
	kill -${sig} ${pid}
	while [[ ${pid} != "" ]]
	do
		echo "Stopping ${file} ..."
		sleep 1s
		pid=`ps x|grep ${file}|grep -v grep|awk '{print $1}'`
	done
done

echo "all server stopped!"
#pkill Server
