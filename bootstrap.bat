rem Run VsDevCmd.bat before executing this file!!!
rem example path: "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"

rem Copyright (C) 2020 David Sloan
rem
rem This library is free software; you can redistribute it and/or
rem modify it under the terms of the GNU Lesser General Public
rem License as published by the Free Software Foundation; either
rem version 2.1 of the License, or (at your option) any later version.
rem
rem This library is distributed in the hope that it will be useful,
rem but WITHOUT ANY WARRANTY; without even the implied warranty of
rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
rem Lesser General Public License for more details.
rem
rem You should have received a copy of the GNU Lesser General Public
rem License along with this library; if not, see <http://www.gnu.org/licenses/>.

@echo off

set @CL_EXE=cl

set @ARGS=/EHsc /Zi /std:c++14 /W3

set @CC=%@CL_EXE% %@ARGS% /I libs

set @BUILD_FILES=SourceBuild.cpp

call :make_o libs\Base, Char
if %errorlevel% neq 0 exit /b %errorlevel%
call :make_o libs\Base, Exception
if %errorlevel% neq 0 exit /b %errorlevel%
call :make_o libs\Base, String
if %errorlevel% neq 0 exit /b %errorlevel%

call :make_o libs\FileSystem, FileStream
if %errorlevel% neq 0 exit /b %errorlevel%
call :make_o libs\FileSystem, Path
if %errorlevel% neq 0 exit /b %errorlevel%

call :make_o libs\IO, BaseStream
if %errorlevel% neq 0 exit /b %errorlevel%
call :make_o libs\IO, ReadStream
if %errorlevel% neq 0 exit /b %errorlevel%
call :make_o libs\IO, StdIO
if %errorlevel% neq 0 exit /b %errorlevel%
call :make_o libs\IO, StreamException
if %errorlevel% neq 0 exit /b %errorlevel%
call :make_o libs\IO, TextStream
if %errorlevel% neq 0 exit /b %errorlevel%
call :make_o libs\IO, WriteStream
if %errorlevel% neq 0 exit /b %errorlevel%

call :make_o ., parsing_helpers
if %errorlevel% neq 0 exit /b %errorlevel%
call :make_o ., Source
if %errorlevel% neq 0 exit /b %errorlevel%
call :make_o ., SourceHeader
if %errorlevel% neq 0 exit /b %errorlevel%
call :make_o ., SourceProject
if %errorlevel% neq 0 exit /b %errorlevel%

@echo on
%@CC% %@BUILD_FILES% /Fesourcebuild.exe
@echo off

exit /B %ERRORLEVEL%

:make_o
set @OBJ_PATH=%~1\.obj
if not exist "%@OBJ_PATH%" mkdir "%@OBJ_PATH%"
set @OBJ="%~1\.obj\%~2.obj"
set @SRC="%~1\%~2.cpp"
set @BUILD_FILES=%@BUILD_FILES% %@OBJ%
@echo on
%@CC% /c %@SRC% /Fo%@OBJ%
@echo off
exit /B %errorlevel%

