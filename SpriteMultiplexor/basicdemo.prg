   "LOADING ";L  LēLŠ1 9  Lē1 § "SPRITES",8,1 U  Lē2 § "MPLXC000",8,1 [d  jm  X POS LO n  54, 78, 0, 0, 0, 0, 114, 162 ķo  62, 86, 110, 102, 102, 126,126,150 Þp  130,154,106,130, 0, 138, 174, 134 
	x  80, 104, 128, 152, 176, 200, 224, 248 	  Y POS A	  54, 53, 255, 255, 255, 255, 113, 113 d	  175, 175,176,50,71,71,92, 92 	  134, 134, 155, 155, 255, 113, 255, 255 ū	  225, 225, 225, 225, 225, 225, 225, 225 č	  "THIS SPRITE DEMO DISPLAYS A LARGE" 
  "MOVING SPRITE OBJECT CONTROLLED" #
  "FROM BASIC" A
  "START THE MULTIPLEXOR" M
  49152 u
É  "SETUP SCREEN AND SPRITE COLOURS" 
Ę  53280,0:53281,0 Ī
Ë  53285,1:53286,11 Ņ
Ņ  "SETUP SPRITE POINTER AND MULTICOLOUR" ú
Ō Tē0Ī31: 49297ŠT,213ŠT:49329ŠT,1: Ó  "SETUP SPRITE COLOURS" bÔ Tē0Ī7: 49265ŠTŽ4,2: 49265ŠTŽ4Š1,6: 49265ŠTŽ4Š2,3: 49265ŠTŽ4Š3,5: Û  "SETUP POSITIONS XPOS AND YPOS" ĶÜ Tē0Ī31: A: 49169ŠT,A: Äæ Tē0Ī31: A: 49233ŠT,A: Þų  "UPDATE 32 SPRITES" íú  49155,32 ú Tē0Ī63 - CLEAR THE FRAME SIGNAL, WAIT FOR A NEW FRAME E 49156,0: 49156,1  MOVE 24 SPRITES RIGHT TWO PIXELS STARTING AT INDEX 0 Ŧ780,2:781,24:782,0:49160:49155,24 ą	 ū Tē0Ī31 ņ CLEAR THE FRAME SIGNAL, WAIT FOR A NEW FRAME 	 49156,0: 49156,1 D MOVE 24 SPRITES LEFT FOUR PIXELS STARTING AT INDEX 0 o780,4:781,24:782,0:49157:49155,24 u  Tē0Ī16 ĩ CLEAR THE FRAME SIGNAL, WAIT FOR A NEW FRAME Í 49156,0: 49156,1  MOVE 24 SPRITES DOWN ONE PIXELS STARTING AT INDEX 0 2780,1:781,24:782,0:49166:49155,24 8 E" Tē0Ī16 x# CLEAR THE FRAME SIGNAL, WAIT FOR A NEW FRAME $ 49156,0: 49156,1 Č% MOVE 24 SPRITES UP ONE PIXELS STARTING AT INDEX 0 ó&780,1:781,24:782,0:49163:49155,24 ų' , Tē0Ī63 9- CLEAR THE FRAME SIGNAL, WAIT FOR A NEW FRAME Q. 49156,0: 49156,1 / MOVE 8 SPRITES RIGHT TWO PIXELS STARTING AT INDEX 24 ·0780,2:781,8:782,24:49160:49155,32 ―1 Ę6 Tē0Ī63 ý7 CLEAR THE FRAME SIGNAL, WAIT FOR A NEW FRAME 8 49156,0: 49156,1 O9 MOVE 8 SPRITES LEFT TWO PIXELS STARTING AT INDEX 24 z:780,2:781,8:782,24:49157:49155,32 ; @ 260   