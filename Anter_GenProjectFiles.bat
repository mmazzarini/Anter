@echo off
set UPROJECT=%~dp0Anter_Termite_Mayhem.uproject
set UBT="C:\Program Files\Epic Games\UE_4.27\Engine\Binaries\DotNET\UnrealBuildTool.exe"

echo Generating project files...
%UBT% -projectfiles -project="%UPROJECT%" -game -rocket -progress

echo Done.
pause