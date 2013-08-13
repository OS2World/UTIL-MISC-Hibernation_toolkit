Hibernation toolkit for OS/2 Warp
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

1. Introduction

   One of certain features available in Windows 2000 is its "hibernation"
   (a.k.a. "fast boot") facility, which makes it possible to save the current
   system state, shutdown the system, and restore the system state at the next
   reboot. Surprisingly, this feature has been developed for OS/2 back in
   1995, when Windows NT 3.51 didn't even exist! However, it hasn't been paid
   with much attention since then, so it requires certain hacking in order to
   work now.

   There is also a so-called "Dedicated DOS mode", which utilizes the
   hibernation facility to invoke a sort of DOS dual-boot while retaining the
   OS/2 execution environment on HDD. This may be useful to run certain
   low-level applications or lauch Windows 3.1 in 386 Enhanced mode. A subset
   of PC DOS v 7.0 is provided for this case.

   This toolkit performs the following functions:

	- Installation of hibernation-capable environment to OS/2 v 4.50:
		- Installation of base modules
		- Replacement of OS/2 loader
		- Fixes for file system and device driver shutdown

        - Fixes for hibernation on XR_M006 and later CSDs
		- Fixes for file system and device driver shutdown

        - Hibernation of Warp 3 by means of an OS/2 v 4.0 kernel
		- Patching the kernel to report version 3.0
		- Fixes for file system and device driver shutdown,
	     OR - Workaround for the above mentioned fixes by supplying
                  XR_M005 kernel and patching PMVIOP.DLL (not recommended).

2. Requirements

	- OS/2 v 4.50: OS/2 v 4.0 with XR_M013+
                    OR Merlin Convenience Pack
                    OR eComStation
        - OS/2 v 4.00: XR_M006 and higher
        - OS/2 v 3.00: XR_M032 and higher

        - FAT partition: OS/2 _must_ be installed on a primary FAT16 partition
          mapped as C:. It does not matter if there are other partitions
          and file systems besides C:, but the boot drive must be C:, FAT16.
          If your system is installed on HPFS/Ext2FS/..., and/or you're booting
          from drive other than C:, then DO NOT INSTALL THIS TOOLKIT!

        - LXLITE v 1.21 or higher. For now, version 1.30 is available as:
          ftp://hobbes.nmsu.edu/pub/os2/util/archiver/lxlt130.exe

        - 512K of disk space. More will be needed when you hibernate your
          system.

	- Overall knowledge of OS/2 system management tasks, i.e. unlocking
	  DLLs, handling bundles, dealing with packed executables, recovering
	  from boot problems, etc. There isn't much of tutorial here, and it's
	  quite dangerous job!

   Notes:

      1. In OS/2 v 4.0 with system levels prior to XR_M006 it's not necessary
         to install this toolkit at all. It's useless - there is nothing that
         can be fixed.

      2. For OS/2 v 3.0, a subset of Warp 4 has to be supplied (see below). 
         It's certainly not legal to copy files from a newer version to OS/2,
         but it is up to you to decide. You have been warned.

3. Installation

   Please read this section carefully and perform all the steps.

   3.1. Verification

	Please verify that the patch facility (HIBPATCH.EXE) works by invoking
        it - a help will be displayed. It relies on IBM LIBC and will fail if
        its DLLs are missing.

   3.2. Kernel installation

	 3.2.1. OS/2 v 3.0 procedure

		We'll now install OS/2 v 4.0 kernel. Please find a copy of
		Warp 4, and a fixpak: XR_M005 or XR_M012 is recommended.
		Unless you want a "technically correct" hibernation, we advise
		XR_M012, for the following particular reasons:
			1. The latest kernels are more stable.
			2. Re-forwarding undocumented ordinals in PMVIOP.DLL
			   is a much more dangerous job than stubbing certain notification
		   routines in kernel.

		First of all, install the following files to \OS2\BOOT (backup
		of existing files may be wise):

		IBMKBD.SYS
		RESOURCE.SYS
		ISAPNP.SNP
		PNP.SYS

		Now, unfold the Fixpak, then manually unpack and install the
		following files (to unlock locked DLLs, run UNLOCK.EXE from
		LXLite, specifying their name on the command line):

		\OS2KRNL
		\OS2LDR
		\OS2LDR.MSG
		\OS2DUMP
		\OS2\DLL\DOSCALL1.DLL
		\OS2\MDOS\VW32S.SYS

		Most of files are in FIX\OS2.1 directory of the FixPak. Note
		that there may be two separate OS2LDR files, one for OS/2, and
		another one for WSoD. You'll need the OS/2 one (it's located
		in FIX\OS2.2 directory, not anywhere else!)

		Now you're about to patch the kernel. Unpack \OS2KRNL with
		LXLITE /X, and run HIBPATCH \OS2KRNL /WARP3. You should get a
		report about specific entry points being patched (there are 5
		or 6 such ones).

		One more patch:
	
		1. If you're using XR_M005 or earlier kernel: unpack
		   \OS2\DLL\PMVIOP.DLL and run
		   HIBPATCH \OS2\DLL\PMVIOP.DLL /PMVIOP.
	        2. If you're using XR_M012, run HIBPATCH \OS2KRNL /KRNLFIX.
		   This time it will report two entries.

		Now, if everything went fine, the kernel may be packed back.
		Run LXLITE \OS2KRNL and read on.

	 3.2.2. OS/2 v 4.0 (XR_M006...XR_M012) and 4.50 procedure

		For OS/2 v 4.x systems, only one kernel patch is necessary.
		Run LXLITE /X \OS2KRNL, then HIBPATCH \OS2KRNL /KRNLFIX. If
		it does not report errors, pack the kernel: LXLITE \OS2KRNL,
		and read on.

   3.3. Installation of hibernation environment

	There are two separate cases which have to be followed correctly unless
	you want to crash the OS/2 loader.

	 3.3.1. OS/2 v 3.0 and 4.0 (XR_M006...XR_M012)

		Take your OS/2 v 4.0 installation CD and unpack the TRUEMODE
		bundle (it's in \OS2IMAGE\DISK_39\TRUEMODE on my CD) on your
		startup drive. You may watch how \HYBERLDR goes to the root
		and certain files pass to \OS2\SYSTEM. If you have installed
		"Dedicated session support" during first-time installation,
		then you may actually skip this step.

	 3.3.2. OS/2 v 4.50

		Even if you're just using OS/2 v 4.0 with a recent FixPak, and
		the installer has provided a hibernation environment for you,
		it's outdated anyway. The "contrib" directory in this package
		contains a TRUEMODE.450 bundle that will create or update the
		hibernation environment. Please note that your \OS2LDR will
		get overwritten with a special unofficial edition from IBM,
		and it will place certain restrictions:

			1. This OS2LDR has to be retained even if a newer one
			   from FixPak overrides it.
			2. If you have more than 64 megabytes of RAM installed
			   but OS/2 reports 64, look for a loader patch
			   (PATCHLDR.ZIP).
			3. Some day this loader may become outdated, failing
			   to load newer kernels, and then the hibernation
			   will be unusable once again.

		In OS/2 v 4.50 the following patch is strongly recommended:
		HIBPATCH \OS2\SYSTEM\SHELL.COM /SHELLFIX. It prevents you from
		watching the endless CHKDSK /F if your system hangs when
		returning from hibernation.

   3.4. Modifications to CONFIG.SYS

	The only modification required so far is adding one line:

	RUN=C:\OS2\SYSTEM\HYBERSET.EXE

	It doesn't matter where you insert it.

   3.5. OS/2 v 3.0 only - modifying the startup banner files

	The hot keys of recovery choices displayed during ALT+F1 have been
	changed since you installed an OS/2 v 4.0 kernel:

	 C   ->   F2
	 V   ->   F3
	 M   ->   F4

	New keys will be added: F5 for enabling full hardware detection, F6
	for disabling the hardware detection. However, these are not functional
	since snooper files (*.SNP) are required for them.

   3.6. Rebooting

	Now, if you have considered a backdoor for the case of boot failure,
	reboot your system and get to the PM.

4. Usage

   For the beginning, there is nothing simpler than issuing

   HYBERNAT

   from the command line. If it ran fine, the rest is self-explaining.
   Now, for a more complicated usage:

   4.1. HYBERNAT.EXE parameters

	There are some ones:

	     /r = reboot when hibernation is done.
	     /p = "page-out" mode: place all swappable RAM to the
		  swap file, then hibernate.
	/n<xxx> = display "Starting <XXX>..." when hibernating.
	     /s = diagnostic switch (?). Never hibernates, may be
		  useful together with /p to move all garbage to the
		  SWAPPER.DAT, freeing up the physical RAM.
	/t<xxx> = Program path for the dedicated DOS session (see
		  below).
	/w<xxx> = Work directory for the dedicated DOS session.

   4.2. Dedicated DOS sessions

	These ones are a specific feature, worth much more than a couple
	of help pages and paragraphs in documentation. To start one in
	OS/2 v 4.xx, you may just utilize the GUI since OS/2 v 4.xx WPS
	allows you to create "Dedicated session" objects for programs.
	
	Running a dedicated session manually is not much harder but will
	require typing certain additional parameters to HYBERNAT.EXE:
	
	HYBERNAT /t"c:\games\runme.bat" /n"Hot action game" /w"c:\games"
	
	In this example, a batch file is run, and the reboot is invoked
	automatically (/r is not required). When the batch file exits,
	you are brought to OS/2.

   4.3. Configuration files

	The \OS2\SYSTEM\CONFIG.DOS and \OS2\SYSTEM\AUTOEXEC.DOS can be
	used to set up the DOS environment. For some reasons, they are
	renamed to \CONFIG.SYS and \AUTOEXEC.BAT by HYBERNAT.EXE, this
	makes some dual-boot programs mad (e.g. System Commander). See
        below for workaround.

   4.4. Switching to Windows 95/98/Millenium from OS/2

	This may be a reasonable question to ask: can Windows 95 run from
	the dedicated session? It has been designed to run from its own
	MS-DOS v 7.0 only but there were patches to run it from Caldera
        OpenDOS, et al. I haven't tested any of them, nor do I have Windows 95
	anywhere. NT can be booted by means of ordinary hibernation and
	selection of startup system from boot manager.

5. Troubleshooting

   5.1. TVFS

	Even if fix for file system shutdown is applied, there is [at least]
	one file system that can't be handled this way: TVFS. Unless you are
	using TVFS for mission-critical daemons, you can take it out
	efficiently with the following script:
	
	tvkill
	shift
	hybernat %0 %1 %2 %3 %4 %5 %6 %7 %8 %9
	set TVFS_RESTORE_CMD=C:\OS2\TVFS\TVFS_RST.CMD
	tvctl -p -w -r
	
	All applications accessing files on a TVFS volume will go into
	undetermined state for the time between TVKILL is issued and the
	system is suspended by HYBERNAT. The second critical point is
	when HIBERNAT.EXE exits but the TVFS initialization sequence is
	not over. That's why daemon processes running with open files on
        TVFS may not survive.

   5.2. LVM

	Hibernation has not been tested with LVM from Warp Server for
	e-business (it's also a sinister outlaw to use it there!), nor has it
	been tested with JFS setup of eComStation (if it does utilize
	OS2LVM.DMD). If you have anything to report, your comments are
	appreciated via e-mail.

   5.3. System Commander

	A heavy-duty workaround is required for this beast (but only the
	"MultiFAT" setup needs it). You will have to avoid WPS functionality
	for making "dedicated session" objects, and to compose a wrapper for
	HYBERNAT.EXE. Here is mine:

	@echo off
	REM $Id: README.TXT,v 1.3 2001/03/31 16:48:45 root Exp $
	REM
	REM Hibernation event wrapper
	REM
	shift
	start /FS hiber2.cmd %0 %1 %2 %3 %4 %5 %6 %7 %8 %9

	@echo off
	REM $Id: README.TXT,v 1.3 2001/03/31 16:48:45 root Exp $
	REM
	REM -- Not to be called directly! --
	REM
	c:\os2\tvfs\tvkill
	copy c:\config.sys c:\sc\os2\config.XXX
	copy c:\autoexec.bat c:\sc\os2\autoexec.XXX
	shift
	hybernat %0 %1 %2 %3 %4 %5 %6 %7 %8 %9
	del c:\config.sys
	del c:\autoexec.bat
	copy c:\sc\os2\config.XXX c:\sc\os2\config.sys
	copy c:\sc\os2\autoexec.XXX c:\sc\os2\autoexec.bat
	copy c:\sc\os2\config.sys c:\
	copy c:\sc\os2\autoexec.bat c:\
	del c:\sc\os2\config.XXX
	del c:\sc\os2\autoexec.XXX
	start /c /MIN tvfsinit
	exit

	These are not two separate wrappers but two scripts. When HIBERNAT.CMD
	is run, it opens a full-screen session that does all the job without
	switching to/from PM (hibernation needs full-screen, if you start it
	in the background or bail from it with Ctrl+ESC, you'll see nothing
	after restore).

	tvfsinit is an initialization script for TVFS, outlined above.

	Now, if I need to run a dedicated session, I do it as follows:

	hibernat /t"c:\egts\start2k.bat" /n"EGTS-2000" /w"c:\egts"

	Note the "y" being changed to "i". This name for script has no special
	meaning... but what is the correct spelling for "hibernation", anyway?

   5.4. Possible reasons for HYBERNAT.EXE failing to run:

	- Disk space: the free space on disk C: has to be capable to keep
	  the hibernation file (SWAPPER2.DAT), which is equal to the amount
	  of physical RAM.

	- Conflicting drivers: in very unlucky cases, hibernation may fail
	  consistently, until a reboot is performed. The reason is unknown,
	  since this situation is very unfrequent on my PC.

	- Network connections. There is a plenty of ones, and I'm not able to
	  test all kinds of networks. LAN0 and PPP0 do not compromise the
	  hibernation (but well compromise the data being sent over them).

	- SMP. Merlin was not SMP-capable!

   5.5. Possible reasons for the system failing to restore:

	- EXT2FLT.FLT, if installed with the /A switch, results in broken
	  hibernation file.

	- More than 64 MB of RAM: so far, I haven't tested this feature on
	  systems with more than 32 megabytes. Submit your practice to e-mail.

	- SCSI disk drives. Not tested with SCSI.

   5.6. Where it has been tested

	- 486DX2-50/8/770: OS/2 v 3.0 with XR_M005 and XR_M012 kernels.
	- 5x86-133/32/20 GB: OS/2 v 4.0 with XR_M012...XR_M015, and kernel
	  postfixes through 14.064d.

	The file systems involved in both cases are FAT16, HPFS and Ext2FS.

6. Technical background

   6.1. DosSysCtl()

	The hibernation process is accomplished by issuing the DosSysCtl()
	API, which is DOSCALL1.876. The exact declaration may be something
	like this:

	APIRET DosSysCtl(ULONG code, PVOID data)

	where code is a system-management function, with 11...20 being used
	by the hibernation program:

	(*)	11 = specifies the hibernation state (?)
		12 = runs the main routine, _hybernate
		13 = _hyberfreezesys
	(*)	14 = _TKFuBuff with _hyberfreezesys
	(*)	15 = performs FSD shutdown via _FSD_FlushBuf
	(*)	16 = the "page-out" for HYBERNAT /p, _hyberPageOut
	(*)	17 = _hyberOK
	(*)	18 = _getSwapperPath
	(*)	19 = _hyberFileCheck
	(*)	20 = (stub?)

	(*) marks functions invoked by HYBERNAT.EXE.

	A detailed DosSysCtl() toolkit may be useful but is hard to provide.
	Any mistakes may lead in data being stored at physical address 0/0/1
	instead of \SWAPPER2.DAT, which breaks the system partition table and
	requires hard labor for recovering it.

   6.2. HYBERNAT.EXE

	This program is a wrapper around DosSysCtl, however quite a thick
	wrapper, since dedicated DOS mode is much of its job. Besides checking
	certain conditions, it also performs the dedicated session setup:

		1. It backs up \CONFIG.SYS and \AUTOEXEC.BAT.
		2. It creates the \CONFIG.SYS and \AUTOEXEC.BAT	from
		   \OS2\SYSTEM\*.DOS.
		3. It inserts shell invocation in these files, yielding 
		   \DOS.CFG and \HYBER.CFG. That are the files used by
		   IBM PC DOS loader in \OS2\SYSTEM.
		4. After return, the files are switched once again. The DOS
		   configuration files in \OS2\SYSTEM are updated with the
		   files from root, that's why they are copied there at step 2.

	Writing a custom HYBERNAT.EXE may be a generous job, to get rid of its
	hard-coded logic. At least, the "Cannot hibernate the system" message
	may become a little more informative.

   6.3. The loaders

	OS2LDR works in cooperation with HYBERLDR. It's hard to distinguish
	which tasks are performed by kernel rather than HYBERLDR, but the
	function of OS2LDR is quite simple: to locate the \SWAPPER2.DAT, and
	if one exists, check for its first character being 'H'. If it is, then
	it's replaced with 'X' (to prevent looping), a blurb is displayed and
	the kernel proceeds with HYBERLDR. That's correct for OS/2 v 4.0.

	It's somehow different in OS/2 v 4.50, so we need a special OS2LDR
	with code which, for some reason (TCO profits?), does not make it into
	official fixes. OS2LDR now reads up HYBERLDR, performs memory arena
	setup and only then runs HYBERLDR (or maybe kernel). This difference
	results in necessity to patch \OS2\SYSTEM\SHELL.COM, since the loader
	now does some sort of disk access prematurely, relying on the
	environment left from dedicated session, whose interrupt handlers are
	already overwritten.

	An effort to supply additional code to regular OS2LDR has been made
	(see the "experimental" OS2LDR section in the source) but there is
	some sort of communication area in RAM which is hard to track down, so
	it is not operable for now. If you run an open-source development
	facility and wish to continue this project, you're welcome. Just drop
	a note.

   6.4. HYBERSET.EXE

	Looks like a helper used by HYBERNAT.EXE. Nothing worth to tell about.

7. Contacting the author

   Mail any suggestions to: Andrew Belov <andrew_belov@newmail.ru>.

   $Id: README.TXT,v 1.3 2001/03/31 16:48:45 root Exp $
