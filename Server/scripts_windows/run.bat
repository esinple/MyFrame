::@echo off
::for %%i in (..\bin\*Server.exe) do (
::	echo "start %%i"
::	start %%i
::)

start ../bin/SuperServer
start ../bin/GameServer
start ../bin/GatewayServer

::pause