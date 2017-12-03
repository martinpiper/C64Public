How it works
============

This routine works on the principal of reusing sprite slots as soon as possible after the bottom of the sprite is drawn. A couple of lines is used below the sprite bottom for safety.
This is contrary to the other method of setting a raster to appear N-lines above where the next sprite needs to be drawn.
Basically starting from just above the first sprite draw the first chunk of eight sprites.
a) Then check the raster for being below the bottom of the next sprite slot (sprite 0 to 7).
If raster below then draw the next sprite slot and check for no more sprites (if so then run sorting interrupt instead) and loop to a)
Set IRQ to trigger no less than three pixels below the current raster.
