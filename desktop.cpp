#include "desktop.h"
#include <rfb/keysym.h>

using namespace std;

Desktop::Desktop()
{
    this->bpp = 4;
    this->max_x = 800;
    this->max_y = 600;
    this->display = rfbGetScreen(0,(char**) "a",this->max_x,this->max_y,8,3,this->bpp);
    this->display->frameBuffer = (char*)malloc(this->max_x*this->max_y*bpp);
}

void Desktop::changeSizeDisplay(int width, int height)
{
    this->max_x = width;
    this->max_y = height;
}

int Desktop::start()
{
    this->display->newClientHook = newclient;
    this->display->ptrAddEvent = doptr;
    this->display->kbdAddEvent = dokey;
    this->display->port = 5901;
    this->display->ipv6port = 5901;
    //this->display->backgroundLoop = true;
    initBuffer((unsigned char*) this->display->frameBuffer);
    rfbDrawString(this->display,&radonFont,550,20,"Hello, guest!",0xffffff);
    rfbDrawString(this->display,&radonFont,550,40,"Welcome to interactive system",0xffffff);

    /* This call creates a mask and then a cursor: */
    /* rfbScreen->defaultCursor =
         rfbMakeXCursor(exampleCursorWidth,exampleCursorHeight,exampleCursor,0);
    */

    MakeRichCursor(this->display);
    rfbInitServer(this->display); // init server
    rfbRunEventLoop(this->display, -1, false);//arg3 = true, async mode

    return 1;
}
