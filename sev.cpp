#include "tf_server.hpp"

int main()
{
  server::handler::ptr handler(new tf_server_handler());
  server endpoint(handler);
  endpoint.listen(31415);
}
