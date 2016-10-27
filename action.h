#ifndef ACTION_H
#define ACTION_H
#include "rfb/rfb.h"
static const int bpp=4;
static int maxx=800, maxy=600;
/* TODO: odd maxx doesn't work (vncviewer bug) */

/* This initializes a nice (?) background */

static void initBuffer(unsigned char* buffer)
{
    int i,j;
    for(j=0;j<maxy;++j) {
        for(i=0;i<maxx;++i) {
            buffer[(j*maxx+i)*bpp+0]=(i+j)*128/(maxx+maxy); /* red */
            buffer[(j*maxx+i)*bpp+1]=i*128/maxx; /* green */
            buffer[(j*maxx+i)*bpp+2]=j*256/maxy; /* blue */
        }
        buffer[j*maxx*bpp+0]=0xff;
        buffer[j*maxx*bpp+1]=0xff;
        buffer[j*maxx*bpp+2]=0xff;
        buffer[j*maxx*bpp+3]=0xff;
    }
}

/* switch to new framebuffer contents */

static void newframebuffer(rfbScreenInfoPtr screen, int width, int height)
{
    unsigned char *oldfb, *newfb;

    maxx = width;
    maxy = height;
    oldfb = (unsigned char*)screen->frameBuffer;
    newfb = (unsigned char*)malloc(maxx * maxy * bpp);
    initBuffer(newfb);
    rfbNewFramebuffer(screen, (char*)newfb, maxx, maxy, 8, 3, bpp);
    free(oldfb);

    /*** FIXME: Re-install cursor. ***/
}

/* aux function to draw a line */

static void drawline(unsigned char* buffer,int rowstride,int bpp,int x1,int y1,int x2,int y2)
{
    int i,j;
    i=x1-x2; j=y1-y2;
    if(i==0 && j==0) {
        for(i=0;i<bpp;i++)
            buffer[y1*rowstride+x1*bpp+i]=0xff;
        return;
    }
    if(i<0) i=-i;
    if(j<0) j=-j;
    if(i<j) {
        if(y1>y2) { i=y2; y2=y1; y1=i; i=x2; x2=x1; x1=i; }
        for(j=y1;j<=y2;j++)
            for(i=0;i<bpp;i++)
                buffer[j*rowstride+(x1+(j-y1)*(x2-x1)/(y2-y1))*bpp+i]=0xff;
    } else {
        if(x1>x2) { i=y2; y2=y1; y1=i; i=x2; x2=x1; x1=i; }
        for(i=x1;i<=x2;i++)
            for(j=0;j<bpp;j++)
                buffer[(y1+(i-x1)*(y2-y1)/(x2-x1))*rowstride+i*bpp+j]=0xff;
    }
}

#endif // ACTION_H
