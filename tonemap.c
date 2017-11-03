/*****************************************************************************
 * tonemap: a vapoursynth plugin for tone mapping HDR video
 *****************************************************************************
 * VapourSynth plugin
 *     Copyright (C) 2017 Phillip Blucas
 *
 * Hable ported from vf_tonemap
 *     Copyright (c) 2017 Vittorio Giovara
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111, USA.
 *
 *****************************************************************************/

#include <VapourSynth.h>
#include <VSHelper.h>

typedef struct {
    VSNodeRef *node;
    const VSVideoInfo *vi;
    float exposure, a, b, c, d, e, f, w;
} HableData;

static void VS_CC hableInit( VSMap *in, VSMap *out, void **instanceData, VSNode *node, VSCore *core, const VSAPI *vsapi )
{
    HableData *d = (HableData *) * instanceData;
    vsapi->setVideoInfo( d->vi, 1, node );
}

static float hable( HableData *d, float in )
{
    return ( in * (in * d->a + d->b * d->c) + d->d * d->e ) /
           ( in * ( in * d->a + d->b ) + d->d * d->f ) - d->e / d->f;
}

static const VSFrameRef *VS_CC hableGetFrame( int n, int activationReason, void **instanceData, void **frameData, VSFrameContext *frameCtx, VSCore *core, const VSAPI *vsapi )
{
    HableData *d = (HableData *) * instanceData;
    if( activationReason == arInitial )
    {
        vsapi->requestFrameFilter( n, d->node, frameCtx );
    }
    else if( activationReason == arAllFramesReady )
    {
        const VSFrameRef *src = vsapi->getFrameFilter( n, d->node, frameCtx );
        const VSFormat *fi = d->vi->format;

        int height = vsapi->getFrameHeight( src, 0 );
        int width = vsapi->getFrameWidth( src, 0 );
        VSFrameRef *dst = vsapi->newVideoFrame( fi, width, height, src, core );

        for( int plane = 0; plane < fi->numPlanes; plane++ )
        {
            const float *srcp = (float *)vsapi->getReadPtr( src, plane );
            float *dstp = (float *)vsapi->getWritePtr( dst, plane );
            int h = vsapi->getFrameHeight( src, plane );
            int w = vsapi->getFrameWidth( src, plane );
            intptr_t stride = vsapi->getStride( src, plane ) / sizeof(float);

            const float coeff_w = d->w;
            const float exposure = d->exposure;
            const float whitescale = 1.0 / hable( d, coeff_w );
            for( int y = 0; y < h; y++ )
            {
                for( int x = 0; x < w; x++ )
                {
                    float sig = hable( d, exposure * srcp[x] );
                    dstp[x] = sig * whitescale;
                }
                dstp += stride;
                srcp += stride;
            }
        }
        vsapi->freeFrame( src );
        return dst;
    }
    return 0;
}

static void VS_CC hableFree( void *instanceData, VSCore *core, const VSAPI *vsapi )
{
    HableData *d = (HableData *)instanceData;
    vsapi->freeNode( d->node );
    free( d );
}

static void VS_CC hableCreate( const VSMap *in, VSMap *out, void *userData, VSCore *core, const VSAPI *vsapi )
{
    HableData d;
    HableData *data;
    int err = 0;

    d.node = vsapi->propGetNode( in, "clip", 0, 0 );
    d.vi = vsapi->getVideoInfo( d.node );

    d.exposure = vsapi->propGetFloat( in, "exposure", 0, &err );
    if( err )
        d.exposure = 2.0;
    d.a = vsapi->propGetFloat( in, "a", 0, &err );
    if( err )
        d.a = 0.15;
    d.b = vsapi->propGetFloat( in, "b", 0, &err );
    if( err )
        d.b = 0.50;
    d.c = vsapi->propGetFloat( in, "c", 0, &err );
    if( err )
        d.c = 0.10;
    d.d = vsapi->propGetFloat( in, "d", 0, &err );
    if( err )
        d.d = 0.20;
    d.e = vsapi->propGetFloat( in, "e", 0, &err );
    if( err )
        d.e = 0.02;
    d.f = vsapi->propGetFloat( in, "f", 0, &err );
    if( err )
        d.f = 0.30;
    d.w = vsapi->propGetFloat( in, "w", 0, &err );
    if( err )
        d.w = 11.2;

    data = malloc( sizeof(d) );
    *data = d;

    if( !isConstantFormat(d.vi) || d.vi->format->sampleType == stInteger || d.vi->format->bytesPerSample != 4 )
    {
        vsapi->setError( out, "Hable: only constant format 32 bit float input supported" );
        vsapi->freeNode( d.node );
        return;
    }
    vsapi->createFilter( in, out, "Hable", hableInit, hableGetFrame, hableFree, fmParallel, 0, data, core );
}

VS_EXTERNAL_API(void) VapourSynthPluginInit( VSConfigPlugin configFunc, VSRegisterFunction registerFunc, VSPlugin *plugin )
{
    configFunc( "com.ifb.tonemap", "tonemap", "Simple tone mapping for VapourSynth", VAPOURSYNTH_API_VERSION, 1, plugin );
    registerFunc( "Hable",
                  "clip:clip;"
                  "exposure:float:opt;"
                  "a:float:opt;"
                  "b:float:opt;"
                  "c:float:opt;"
                  "d:float:opt;"
                  "e:float:opt;"
                  "f:float:opt;"
                  "w:float:opt;",
                  hableCreate, 0, plugin );
}
