@echo off
for %%i in (..\common\command\*.proto) do (
	echo "compile %%i"
	..\bin\protoc -I=..\common\command\ --cpp_out=..\src\netproto %%i)
	
for %%i in (..\common\dbstruct\*.proto) do (
	echo "compile %%i"
	..\bin\protoc -I=..\common\dbstruct\ --cpp_out=..\src\dbproto %%i)
pause

