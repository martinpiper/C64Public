SETLOCAL EnableDelayedExpansion

for %%x in (
	0
	20000
	40000
	60000
	80000
	100000
	120000
	140000
	160000
	180000
	200000
) do (
	set /a "c=6770000+%%x"
	SET d=0000000%%x
	SET d=!d:~-7%!
	echo !d!

	..\ExternalTools\Gnu\bin\sed.exe -n "/%%x/p" Scroller.map >>CartCode\CartOpts.a
	C:\Downloads\WinVICE-3.1-x86-r34062\WinVICE-3.1-x86-r34062\x64sc.exe -autostartprgmode 1 +autostart-delay-random +VICIIdsize +VICIIdscan -VICIIborders 2 -VICIIfilter 0 +sound -warp -remotemonitor -exitscreenshot c:\temp\t-!d!.png -limitcycles !c! %cd%\Scroller.crt
)
