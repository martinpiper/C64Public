@echo off
rem Print file totals

echo Total
dir Data\frm0*.del | findstr " File(s)"
