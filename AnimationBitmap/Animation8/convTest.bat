setlocal ENABLEDELAYEDEXPANSION
del *.bmp

C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\convert.exe -verbose "C:\work\animationtech\HMScenes1\for_mp\thalamus_logo - Copy.bmp" -type truecolor ThalamusLogo.bmp
C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -brightness-contrast -10 ThalamusLogo.bmp


FOR /L %%A IN (0,1,1) DO (
	SET B=0000%%A
	copy "C:\work\animationtech\HMScenes1\for_mp\hm_logo2.bmp" cut00-!B:~-5!.bmp
)


rem The panels
copy "C:\Users\Martin Piper\Downloads\loopingtest\Scene_2" cut01-*.bmp
copy "C:\Users\Martin Piper\Downloads\loopingtest\Scene_3" cut02-*.bmp
copy "C:\Users\Martin Piper\Downloads\loopingtest\scene_4\placeholder" cut03-*.bmp
copy "C:\Users\Martin Piper\Downloads\HMScenes1\GameArt\intro\Scene_6\scene6-everyframe\scene6-everyframe" cut04-*.bmp
copy "C:\Users\Martin Piper\Downloads\GameArt (2)\intro\Scene_5\scene5_flattened_warp\scene5_flattened_warp" cut05-*.bmp
copy "C:\Users\Martin Piper\Downloads\GameArt (2)\intro\Scene_6\scene6_flat_nobiganim_CORRECTSIZE\scene6_flat_nobiganim_" cut06-*.bmp


FOR /L %%A IN (0,1,1) DO (
	SET B=0000%%A
	copy "C:\Users\Martin Piper\Downloads\GameArt (2)\intro\Title\title-placeholder-expanded.bmp" cut09-!B:~-5!.bmp
)

C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\convert.exe -verbose cut*.bmp -background black -gravity north -extent 320x200 -type truecolor frm%%05d.bmp
