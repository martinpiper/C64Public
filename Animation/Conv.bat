mkdir Data
del Data\* /q

setlocal EnableDelayedExpansion

for /l %%x in (1,1,7777) do (
set "formattedValue=000000%%x"
	echo !formattedValue:~-5!
	call ConvFrame.bat frm!formattedValue:~-5!
)
