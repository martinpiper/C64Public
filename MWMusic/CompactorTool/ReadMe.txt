This will only read C64 editor saved V2 files (or newer)

Debug\CompactorTool.exe -i bin\armalytev2.mus -o t.a
..\acme.exe -f cbm -o t.prg --cpu 6502 -v4 --setpc $c000 MusicPlayer_Defs.a MusicPlayer.a t.a
