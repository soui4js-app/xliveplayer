@ECHO off
if defined SOUI4JS_PATH (
 echo %SOUI4JS_PATH%
 start %SOUI4JS_PATH%\Release\soui4js-host xliveplayer
 exit
) else (
 echo "no path"
 if exist ..\soui4js\Release\soui4js-host.exe (
 	start ..\soui4js\Release\soui4js-host xliveplayer
 	exit
 ) else (
 	echo "û���ҵ���������:SOUI4JS_PATH, ��ͬ��Ŀ¼Ҳû���ҵ�soui4js, ����ʧ��"
 )
)
