attrib -h *.suo /s
del *.opt *.suo *.ncb *.dpbcd *.dprul *.dpsup *.exp *.scc *.vss* *.vsps* *.vcproj.*.user *.sdf /s

rem ** Built files here
call CleanProject.bat
