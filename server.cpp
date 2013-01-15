#include <iostream>
#include "libwebsockets_server.hpp"

int main(int argc, char *argv[])
{
  websocket_server s;
  the_server= &s;
  libwebsocket_context *context=
      libwebsocket_create_context(31415, NULL, protocols, NULL, NULL, NULL, NULL, -1, -1, 0);
  while (1)
  {
    libwebsocket_service(context, 1);
  }
}
