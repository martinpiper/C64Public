mkdir Data
del Data\* /q

setlocal EnableDelayedExpansion

rem for /l %%x in (1,1,7777) do (
for /l %%x in (1,1,28) do (
set "formattedValue=000000%%x"
	echo !formattedValue:~-5!
	call ConvFrame.bat frm!formattedValue:~-5!
)
