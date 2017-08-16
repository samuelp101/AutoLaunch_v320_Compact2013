AutoLaunch component for Windows Embedded Compact 2013
	Version:		v320
	Catalog Item Name:	AutoLaunch_v320_Compact2013
	OS Version:		Windows Embedded Compact 2013
	Supported processor:	All
	Last updated:		January 21, 2015
-----------------------------------------------------------------
*****************************************************************
*  AutoLaunch_v320_Compact2013
*  http://AutoLaunch4CE.Codeplex.net 
*****************************************************************

The AutoLaunch component, when included to the OS image, can be configured to launch 
one or more application when the Windows Embedded Compact 2013 OS starts.

When included to the OS design, the following file and registry are added to the image:
  -	AutoLaunch.EXE

  [HKEY_LOCAL_MACHINE\Init]
    	"Depend99"=hex:0a,00,14,00,1e,00,32,00
	"Launch99"="AutoLaunch.exe"

Registry entries under the [HKEY_LOCAL_MACHINE\Init] key configures the Compact 2013 OS 
to launch the AutoLaunch.EXE executable during boot up.

When AutoLaunch.EXE executes, it scans and launch application with proper entries in the 
following registry key:
	[HKEY_LOCAL_MACHINE\Startup]

To configure the AutoLaunch component to launch the intended applications, add registry 
entries to launch the intended application as follow:
  [HKEY_LOCAL_MACHINE\Startup]	
	"Process2"="App1.EXE"
	"Process2Delay"=dword:00001388
	"Process3"="App2.EXE"
	"Process3Delay"=dword:00002710
	"Process4"="App3.EXE"
	"Process4Delay"=dword:00003A98

Note:  
  -  Process 0 & 1 are reserved, start with Process2 to launch your application.

  -  The Process delay value is arbitrary assigned in the above example. 

  -  The Process delay parameter, in HEX, set the delay time (after OS is launched) in miliseconds 
     for the AutoLaunch component to wait before launching the associated application. 
	1388H = 5000ms
	2710H = 10000ms
	3A98H = 15000ms

-----------------------------------------------------------------
When the "AutoLaunch CoreCon" sub-component is included to the OS design, the following registry 
entries are automatically added to the OS design to launch "ConmanClient2.exe":
    [HKEY_LOCAL_MACHINE\Startup]
        "Process0"="ConmanClient2.exe"
        "Process0Delay"=dword:00001388
    [HKEY_LOCAL_MACHINE\System] 
        "CoreConOverrideSecurity"=dword:1

----------------------------------------------------------------
When the "UDP IP Address Broadcast" sub-component is included to the OS design, the following
registry entries are automatically added to the OS design to launch "Compact2013UDPClient.exe":
    [HKEY_LOCAL_MACHINE\Startup]
        "Process1"="IPBroadcastCompact2013.exe"
        "Process1Delay"=dword:00002710 

----------------------------------------------------------------
Note:	
When adding registry entries to launch your application, do not modify the AutoLaunch.REG file.

Add registry entries to the OS design project's OSDESIGN.REG.
Entries to OSDESIGN.REG will supersede entries in AutoLaunch.REG. 

Add registry entries to the OSDESIGN.REG as follow, to launch the intended application:

    [HKEY_LOCAL_MACHINE\Startup]	
        "Process2"="App1.EXE"
        "Process2Delay"=dword:00002710
        ;
        "Process3"="App2.EXE"
        "Process3Delay"=dword:00002710

Note:  
Since Process0 and Process1 are used to launch ConmanClient2.exe and Compact2013UDPClient.exe, 
start with Process2 to launch the intended application.

The value for the Process2Delay and Process3Delay above are arbitary selected, which can be set to 0 or other value.

-----------------------------------------------------------------

The AutoLaunch_v320_Compact2013 component is released to the community on Codeplex.
  http://AutoLaunch4CE.Codeplex.com

-----------------------------------------------------------------

The codes were initially based on an article published by Mike Hall on Microsoft website and have been changed and modified with help from the following individuals:

  -  James Y. Wilson

  -  David Jones
     http://www.embedded101.com/Blogs/DavidJones.aspx

  -  Thierry Joubert
     http://www.embedded101.com/Blogs/ThierryJoubert.aspx


Samuel Phung
Windows Embedded MVP
http://www.embedded101.com








 




  
