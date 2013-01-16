#include <cstdio>
#include <cstring>
#include <boost/thread.hpp>
#include "libwebsockets_client.hpp"

void work()
{
  transfer::id id("4321");
  std::string str;
  while(std::getline(std::cin, str))
  {
    the_client->send(id, const_cast<char*>(str.c_str()), str.size());
  }
}

int main(int argc, char *argv[])
{
  transfer::id id((unsigned char *)argv[1]);
  the_client= new websocket_client(id);
  libwebsocket_context *context=
      libwebsocket_create_context(31415, NULL, protocols, NULL, NULL, NULL, NULL, -1, -1, 0);

  boost::thread thread(work);
  while(1) libwebsocket_service(context, 1); 
}
