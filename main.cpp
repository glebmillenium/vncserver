#include <iostream>
#include <rfb/rfb.h>
#include <socket.h>
#include <desktop.h>

using namespace std;



int main(int argc, char *argv[])
{
    //Socket socket;
    //socket.start(argc, argv);
    Desktop desktop;
    desktop.start();
    return 0;
}
