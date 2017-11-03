Description
===========

A filter for converting high dynamic range (HDR) video to standard dynamic range (SDR).  Currently only the Hable operator is implemented.

Ported from the FFmpeg vf_tonemap filter
<http://www.ffmpeg.org/ffmpeg-filters.html#tonemap>


Usage
=====

    tonemap.Hable(clip clip[, float exposure=2.0, float a=0.15, float b=0.50, float c=0.10, float d=0.20, float e=0.02, float f=0.30, float w=11.2])

* clip: Clip to process. Only planar 32-bit float is supported.

* exposure: Gain to apply.

* a: Shoulder strength

* b: Linear strength

* c: Linear angle

* d: Toe strength

* e: Toe numerator

* f: Toe denominator

* w: Linear white point

hable(x) = ( (x\*(a\*x+c\*b)+d\*e) / (x\*(a\*x+b)+d\*f) ) - e/f

output = hable(exposure \* input) / hable(w)

#####
References:
<http://filmicworlds.com/blog/filmic-tonemapping-operators>
<http://www.slideshare.net/ozlael/hable-john-uncharted2-hdr-lighting>


Compilation
===========
The usual autotools method:
```
./autogen.sh
./configure
make
```

On Mingw-64 you can try something like the following:
```
gcc -c tonemap.c -I include/vapoursynth -O3 -ffast-math -mfpmath=sse -msse2 -march=native -std=c99 -Wall
gcc -shared -o tonemap.dll tonemap.o -Wl,--out-implib,tonemap.a
```
