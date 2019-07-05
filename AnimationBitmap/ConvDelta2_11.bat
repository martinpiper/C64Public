@echo off

mkdir Data
del Data\*.raw /q
del Data\*.del /q
del Data\*.tcmp /q
del Data\Dictionary*.bin /q


rem call ConvDelta2_11_segment.bat 1 992

call ConvDelta2_11_segment.bat 1 60
call ConvDelta2_11_segment.bat 61 724
call ConvDelta2_11_segment.bat 725 764
call ConvDelta2_11_segment.bat 765 816
call ConvDelta2_11_segment.bat 817 900
call ConvDelta2_11_segment.bat 901 992
rem call ConvDelta2_11_segment.bat 993 4064

echo Total
dir Data\frm0*.del | findstr " File(s)"
