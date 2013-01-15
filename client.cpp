#include <iostream>
#include "libwebsockets_client.hpp"

int main(int argc, char *argv[])
{
  transfer::id id((unsigned char *)argv[1]);
  websocket_client c(id);
  the_client= &c;
  libwebsocket_context *context=
      libwebsocket_create_context(31415, NULL, protocols, NULL, NULL, NULL, NULL, -1, -1, 0);
  libwebsockets_fork_service_loop(context);
  std::string str;
  while(getline(std::cin, str))
  {
    str+= '\n';
    c.send(id, const_cast<unsigned char*>((unsigned char*)str.data()), str.size());
  }
}
