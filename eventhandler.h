#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H
#ifdef WIN32
#define sleep Sleep
#else
#include <unistd.h>
#endif

#ifdef __IRIX__
#include <netdb.h>
#endif

#include <rfb/rfb.h>
#include <rfb/keysym.h>
#include <action.h>


/* Here the pointer events are handled */

static void doptr(int buttonMask,int x,int y,rfbClientPtr cl)
{
    ClientData* cd=(ClientData*) cl->clientData;

    if(x>=0 && y>=0 && x<maxx && y<maxy) {
        if(buttonMask) {
            int i,j,x1,x2,y1,y2;

            if(cd->oldButton==buttonMask) { /* draw a line */
                drawline((unsigned char*)cl->screen->frameBuffer,cl->screen->paddedWidthInBytes,bpp,
                         x,y,cd->oldx,cd->oldy);
                x1=x; y1=y;
                if(x1>cd->oldx) x1++; else cd->oldx++;
                if(y1>cd->oldy) y1++; else cd->oldy++;
                rfbMarkRectAsModified(cl->screen,x,y,cd->oldx,cd->oldy);
            } else { /* draw a point (diameter depends on button) */
                int w=cl->screen->paddedWidthInBytes;
                x1=x-buttonMask; if(x1<0) x1=0;
                x2=x+buttonMask; if(x2>maxx) x2=maxx;
                y1=y-buttonMask; if(y1<0) y1=0;
                y2=y+buttonMask; if(y2>maxy) y2=maxy;

                for(i=x1*bpp;i<x2*bpp;i++)
                    for(j=y1;j<y2;j++)
                        cl->screen->frameBuffer[j*w+i]=(char)0xff;
                rfbMarkRectAsModified(cl->screen,x1,y1,x2,y2);
            }

            /* we could get a selection like that:
      rfbGotXCutText(cl->screen,"Hallo",5);
      */
        } else
            cd->oldButton=0;

        cd->oldx=x; cd->oldy=y; cd->oldButton=buttonMask;
    }
    rfbDefaultPtrAddEvent(buttonMask,x,y,cl);
}

/* aux function to draw a character to x, y */

#include "vnc/radon.h"

/* Here the key events are handled */

static void dokey(rfbBool down,rfbKeySym key,rfbClientPtr cl)
{
    if(down) {
        switch (key) {
        case XK_Escape:
            rfbCloseClient(cl);
            break;
        //case XK_1:
          //  cl->afterEncBuf
        case XK_F12:
            rfbShutdownServer(cl->screen,TRUE);
            break;
        case XK_F11:
            rfbShutdownServer(cl->screen,FALSE); //close down server, but wait for all clients to disconnect
            break;
        case XK_Page_Up:
            initBuffer((unsigned char*)cl->screen->frameBuffer);
            rfbMarkRectAsModified(cl->screen,0,0,maxx,maxy);
            break;
        case XK_Up:
            if (maxx < 1024) {
                if (maxx < 800) {
                    newframebuffer(cl->screen, 800, 600);
                } else {
                    newframebuffer(cl->screen, 1024, 768);
                }
            }
            break;
        case XK_Down:
            if (maxx > 640) {
                if (maxx > 800) {
                    newframebuffer(cl->screen, 800, 600);
                } else {
                    newframebuffer(cl->screen, 640, 480);
                }
            }
            break;

        default:
            break;
        }
        if(key>=' ' && key<0x100) {
            ClientData* cd=(ClientData*) cl->clientData;
            int x1=cd->oldx,y1=cd->oldy,x2,y2;
            cd->oldx+=rfbDrawCharWithClip(cl->screen,&radonFont,cd->oldx,cd->oldy,(char)key,0,0,cl->screen->width,cl->screen->height,0x00ffffff,0x00ffffff);
            rfbFontBBox(&radonFont,(char)key,&x1,&y1,&x2,&y2);
            rfbMarkRectAsModified(cl->screen,x1,y1,x2-1,y2-1);
        }
    }
}

/* Example for an XCursor (foreground/background only) */

#ifdef JUST_AN_EXAMPLE

static int exampleXCursorWidth=9,exampleXCursorHeight=7;
static char exampleXCursor[]=
        "         "
        " xx   xx "
        "  xx xx  "
        "   xxx   "
        "  xx xx  "
        " xx   xx "
        "         ";

#endif

/* Example for a rich cursor (full-colour) */

static void MakeRichCursor(rfbScreenInfoPtr rfbScreen)
{
    int i,j,w=20,h=16;
    rfbCursorPtr c = rfbScreen->cursor;
    /*char bitmap[]=
            "                                "
            "              xxxxxx            "
            "       xxxxxxxxxxxxxxxxx        "
            "      xxxxxxxxxxxxxxxxxxxxxx    "
            "    xxxxx  xxxxxxxx  xxxxxxxx   "
            "   xxxxxxxxxxxxxxxxxxxxxxxxxxx  "
            "  xxxxxxxxxxxxxxxxxxxxxxxxxxxxx "
            "  xxxxx   xxxxxxxxxxx   xxxxxxx "
            "  xxxx     xxxxxxxxx     xxxxxx "
            "  xxxxx   xxxxxxxxxxx   xxxxxxx "
            " xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx "
            " xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx "
            " xxxxxxxxxxxx  xxxxxxxxxxxxxxx  "
            " xxxxxxxxxxxxxxxxxxxxxxxxxxxx   "
            " xxxxxxxxxxxxxxxxxxxxxxxxxxxx   "
            " xxxxxxxxxxx   xxxxxxxxxxxxxx   "
            " xxxxxxxxxx     xxxxxxxxxxxx    "
            "  xxxxxxxxx      xxxxxxxxx      "
            "   xxxxxxxxxx   xxxxxxxxx       "
            "      xxxxxxxxxxxxxxxxxxx       "
            "       xxxxxxxxxxxxxxxxxxx      "
            "         xxxxxxxxxxxxxxxxxxx    "
            "             xxxxxxxxxxxxxxxxx  "
            "                xxxxxxxxxxxxxxx "
            "   xxxx           xxxxxxxxxxxxx "
            "  xx   x            xxxxxxxxxxx "
            "  xxx               xxxxxxxxxxx "
            "  xxxx             xxxxxxxxxxx  "
            "   xxxxxx       xxxxxxxxxxxx    "
            "    xxxxxxxxxxxxxxxxxxxxxx      "
            "      xxxxxxxxxxxxxxxx          "
            "                                ";*/
    char bitmap [] =
            "      ||            "
            "     |||||          "
            "    |||||||         "
            "   ||||||||||       "
            "   |||||||||||      "
            "  ||||||||||||||    "
            "  ||||||||||||||||  "
            " |||||||||||||||||| "
            " ||||| ||||||| |||| "
            " |||| |||||||  |||  "
            " |||   |||||||      "
            " |     |||||||      "
            "       |||||||      "
            "        ||||||      "
            "        ||||||      "
            "         ||||       ";
    c = rfbScreen->cursor =rfbMakeXCursor(w,h,bitmap,bitmap);
    c->xhot = 0; c->yhot = 0;

    c->richSource = (unsigned char*)malloc(w*h*bpp);

    c->cleanupRichSource = TRUE;
    for(j=0;j<h;j++) {
        for(i=0;i<w;i++) {
            c->richSource[j*w*bpp+i*bpp+0]=0x000000;
            c->richSource[j*w*bpp+i*bpp+1]=0x000000;
            c->richSource[j*w*bpp+i*bpp+2]=0x000000;
            c->richSource[j*w*bpp+i*bpp+3]=0x000000;
        }
    }
}

#endif // EVENTHANDLER_H
