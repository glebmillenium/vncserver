#ifndef DESKTOP_H
#define DESKTOP_H
#include <rfb/rfb.h>
#include <rfb/keysym.h>
#include <clientdata.h>
#include <eventhandler.h>

class Desktop
{
private:
    int bpp;
    int max_x, max_y;
    rfbScreenInfoPtr display;
public:
    Desktop();
    void changeSizeDisplay(int width, int height);
    int start();
};

#endif // DESKTOP_H
