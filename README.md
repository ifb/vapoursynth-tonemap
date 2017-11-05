Description
===========

A filter for converting high dynamic range (HDR) video to standard dynamic range (SDR).

Ported from mpv and the FFmpeg vf_tonemap filter
<http://mpv.io/>
<http://www.ffmpeg.org/ffmpeg-filters.html#tonemap>

Usage
=====

Hable
===

    tonemap.Hable(clip clip[, float exposure=2.0, float a=0.15, float b=0.50, float c=0.10, float d=0.20, float e=0.02, float f=0.30, float w=11.2])

Peserve both dark and bright details better than Reinhard, at the cost of slightly darkening everything. Use it when detail preservation is more important than color and brightness accuracy.

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

Mobius
===

    tonemap.Mobius(clip clip[, float exposure=2.0, float transition=0.3])

Generalization of Reinhard to a Mobius transform with linear section. Smoothly maps out-of-range values while retaining contrast and colors for in-range material as much as possible. Use this when you care about color accuracy more than detail preservation.

* clip: Clip to process. Only planar 32-bit float is supported.

* exposure: Gain to apply.

* transition: Specifies the transition point from linear to mobius transform. Every value below this point is guaranteed to be mapped 1:1. The higher the value, the more accurate the result will be, at the cost of losing bright details. Defaults to 0.3, which due to the steep initial slope still preserves in-range colors fairly accurately.

* peak: Reference peak brightness

#####
References:
<https://github.com/mpv-player/mpv/commit/d8a3b10f45eb10fb34ce9da3a9a76e3bd8644e3d>

Reinhard
===

    tonemap.Reinhard(clip clip[, float exposure=2.0, contrast=0.5])

* clip: Clip to process. Only planar 32-bit float is supported.

* exposure: Gain to apply.

* contrast: Controls the local contrast coefficient at the display peak. Default to 0.5, which means that in-gamut values will be about half as bright as when clipping.

#####
References:
<https://www.cs.utah.edu/~reinhard/cdrom/>

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
