

@echo Copying Content Files from Resource Files folder to Targeted Debug Directory
set _CORECONPATH=%CommonProgramFiles%\Microsoft Shared\Embedded Tools\CoreCon\%VisualStudioVersion%\Target\wce%_WINCEOSVER%\%_TGTCPU%
set _UDPCLIENTPATH=%_WINCEROOT%\3rdParty\AutoLaunch_v320_Compact2013

copy ".\Resources\*.*" "%SG_OUTPUT_ROOT%\oak\target\%_TGTCPU%\%WINCEDEBUG%"  /Y
@echo Copying Content Files from CoreCon Folder to Targeted Debug Directory
xcopy     "%_CORECONPATH%\*.*"       "%SG_OUTPUT_ROOT%\oak\target\%_TGTCPU%\%WINCEDEBUG%"  /Y

xcopy     "%_UDPCLIENTPATH%\IPBroadcastCompact2013.exe"       "%SG_OUTPUT_ROOT%\oak\target\%_TGTCPU%\%WINCEDEBUG%"  /Y

@echo .
@echo Copying Content Files to FlatRelease Directory

@echo Copying Content Files from Resource Files folder to FlatRelease Directory
copy ".\Resources\*.*" %_FLATRELEASEDIR%  /Y
@echo Copying Content Files from CoreCon folder to FlatRelease Directory
xcopy     "%_CORECONPATH%\*.*"      %_FLATRELEASEDIR%  /Y

@echo Copying IPBroadcastCompact2013 to FlatRelease Directory
xcopy  "%_UDPCLIENTPATH%\IPBroadcastCompact2013.exe"  %_FLATRELEASEDIR%  /Y

:DONECOPY
@echo .
@echo Done copying. Now checking.

If NOT EXIST  %SG_OUTPUT_ROOT%\oak\target\%_TGTCPU%\%WINCEDEBUG%\IPBroadcastCompact2013.exe (
rem		@echo Error IPBroadcastCompact2013.exe is missing from the Targeted Debug Directory.
	@echo Error IPBroadcastCompact2013.exe is missing from the Targeted Debug Directory.
	GOTO ABORT
)

If NOT EXIST  %SG_OUTPUT_ROOT%\oak\target\%_TGTCPU%\%WINCEDEBUG%\clientshutdown3.exe (
rem		@echo Error clientshutdown.exe is missing from the Targeted Debug Directory.
	@echo Error clientshutdown3.exe is missing from the Targeted Debug Directory.
	GOTO ABORT
)
If NOT EXIST  %SG_OUTPUT_ROOT%\oak\target\%_TGTCPU%\%WINCEDEBUG%\CMAccept3.exe (
REM 	@echo Error CMAccept.exe is missing from the Targeted Debug Directory.
	@echo Error CMAccept3.exe is missing from the Targeted Debug Directory.
	GOTO ABORT
)
If NOT EXIST  %SG_OUTPUT_ROOT%\oak\target\%_TGTCPU%\%WINCEDEBUG%\ConmanClient3.exe (
REM 	@echo Error ConmanClient2.exe is missing from the Targeted Debug Directory.
	@echo Error ConmanClient3.exe is missing from the Targeted Debug Directory.
	GOTO ABORT
)
If NOT EXIST  %SG_OUTPUT_ROOT%\oak\target\%_TGTCPU%\%WINCEDEBUG%\DeviceAgentTransport3.dll (
REM	@echo Error DeviceAgentTransport2.dll is missing from the Targeted Debug Directory.
	@echo Error DeviceAgentTransport3.dll is missing from the Targeted Debug Directory.
	GOTO ABORT
)
If NOT EXIST  %SG_OUTPUT_ROOT%\oak\target\%_TGTCPU%\%WINCEDEBUG%\eDbgTL3.dll (
REM		@echo Error eDbgTL.dll is missing from the Targeted Debug Directory.
	@echo Error eDbgTL3.dll is missing from the Targeted Debug Directory.
	GOTO ABORT
)
If NOT EXIST  %SG_OUTPUT_ROOT%\oak\target\%_TGTCPU%\%WINCEDEBUG%\TcpConnectionA3.dll (
REM		@echo Error TcpConnectionA.dll is missing from the Targeted Debug Directory.
	@echo Error TcpConnectionA3.dll is missing from the Targeted Debug Directory.
	GOTO ABORT
)

If NOT EXIST  %_FLATRELEASEDIR%\clientshutdown3.exe (
REN	@echo Error clientshutdown.exe is missing from the FLATRELEASE Directory.
	@echo Error clientshutdown3.exe is missing from the FLATRELEASE Directory.
	GOTO ABORT
)
If NOT EXIST  %_FLATRELEASEDIR%\CMAccept3.exe (
REM	@echo Error CMAccept.exe is missing from the from the FLATRELEASE Directory.
	@echo Error CMAccept3.exe is missing from the from the FLATRELEASE Directory.
	GOTO ABORT
)
If NOT EXIST  %_FLATRELEASEDIR%\ConmanClient3.exe (
REM	@echo Error ConmanClient2.exe is missing from the from the FLATRELEASE Directory.
	@echo Error ConmanClient3.exe is missing from the from the FLATRELEASE Directory.
	GOTO ABORT
)
If NOT EXIST  %_FLATRELEASEDIR%\DeviceAgentTransport3.dll (
REM		@echo Error DeviceAgentTransport2.dll is missing from the FLATRELEASE Directory.
	@echo Error DeviceAgentTransport3.dll is missing from the FLATRELEASE Directory.

	GOTO ABORT
)
If NOT EXIST  %_FLATRELEASEDIR%\eDbgTL3.dll (
REM		@echo Error eDbgTL.dll is missing from the FLATRELEASE Directory.
	@echo Error eDbgTL3.dll is missing from the FLATRELEASE Directory.
	GOTO ABORT
)
If NOT EXIST  %_FLATRELEASEDIR%\TcpConnectionA3.dll (
REM		@echo Error TcpConnectionA.dll is missing from the FLATRELEASE Directory.
	@echo Error TcpConnectionA3.dll is missing from the FLATRELEASE Directory.
	GOTO ABORT
)

@echo All CoreCon Folder files copied.
@echo .

GOTO NEXT1

:ABORT
@echo An ERROR occurred
if %9==999 THIS_IS_A_DUMMY_ERROR_TO_FORCE_ABNORMAL_ABORT_AND_RETURN_ERROR_CONDITION_TO_BUILD
REM Returning ERRORLEVEL not equal to 0 isn't picked up by BUild
SET %ERRORLEVEL%=1000
GOTO SKIP
:NEXT1


@echo .
@echo Building .cab file
@echo .

PUSHD
cd %_FLATRELEASEDIR%
IF EXIST CoreCon3.inf (
    cabwiz CoreCon3.inf
	IF EXIST CoreCon3.cab (
		@echo Generated .cab file: CoreCon3.cab in FLATRELEASEDIR.
	) else (
		@echo Generation of .cab file: CoreCon3.cab failed.
	)
	del CoreCon3.inf
)else (
	@echo No file CoreCon3.inf for .cab file generation
)
 
POPD

@echo .
@echo Done Copying
@echo .

:SKIP

