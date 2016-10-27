#ifndef CLIENTDATA_H
#define CLIENTDATA_H
#include "rfb/rfb.h"

typedef struct ClientData {
  rfbBool oldButton;
  int oldx,oldy;
}ClientData;

static void clientgone(rfbClientPtr cl)
{
  free(cl->clientData);
  cl->clientData = NULL;
}

static enum rfbNewClientAction newclient(rfbClientPtr cl)
{
  cl->clientData = (void*)calloc(sizeof(ClientData),1);
  cl->clientGoneHook = clientgone;
  return RFB_CLIENT_ACCEPT;
}

#endif // CLIENTDATA_H
