setlocal ENABLEDELAYEDEXPANSION
del *.bmp

C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\convert.exe -verbose "C:\Work\animationtech\HMScenes1\for_mp\thalamus_logo_v2.png" -type truecolor ThalamusLogo.bmp
C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\mogrify.exe -verbose -brightness-contrast -10 ThalamusLogo.bmp


FOR /L %%A IN (0,1,1) DO (
	SET B=0000%%A
	copy "C:\work\animationtech\HMScenes1\for_mp\hm_logo2.bmp" cut00-!B:~-5!.bmp
)


rem The panels
copy "C:\Work\animationtech\HMScenes1\GameArt\intro\Scene_1\" cut01-*.bmp
copy "C:\Work\animationtech\HMScenes1\GameArt\intro\Scene_2\" cut02-*.bmp
copy "C:\Work\animationtech\HMScenes1\GameArt\intro\scene_3\" cut03-*.bmp
copy "C:\Work\animationtech\HMScenes1\GameArt\intro\scene_4\" cut04-*.bmp
copy "C:\Work\animationtech\HMScenes1\GameArt\intro\Scene_5\" cut05-*.bmp
copy "C:\Work\animationtech\HMScenes1\GameArt\intro\Scene_6\" cut06-*.bmp
rem copy "C:\Work\animationtech\HMScenes1\GameArt\intro\Scene6-animatingBlackhole\" cut06-*.bmp

FOR /L %%A IN (0,1,1) DO (
	SET B=0000%%A
	copy "C:\Work\animationtech\HMScenes1\GameArt\intro\Title\title-placeholder-expanded.bmp" cut09-!B:~-5!.bmp
)

rem Placeholder outro animation
copy "C:\Work\animationtech\HMScenes1\GameArt\outro\" cut10-*.bmp

C:\Downloads\ImageMagick-7.0.7-4-portable-Q16-x64\convert.exe -verbose cut*.bmp -background black -gravity north -extent 320x200 -type truecolor frm%%05d.bmp
