#ifndef _LWS_SERVER_HPP_
#define _LWS_SERVER_HPP_

#include <vector>
#include <cstring>
#include <libwebsockets.h>
#include "tf_server.hpp"

static int callback_http(struct libwebsocket_context *context,
                         struct libwebsocket *wsi,
                         enum libwebsocket_callback_reasons reason, void *user,
                         void *in, size_t len)
{
	return 0;
}

struct fun_send
{
  void operator()(libwebsocket *wsi, void *in, size_t len)
  {
    buf.resize(LWS_SEND_BUFFER_PRE_PADDING + len + LWS_SEND_BUFFER_POST_PADDING);
    unsigned char *b= buf.data() + LWS_SEND_BUFFER_PRE_PADDING;
    memcpy(b, in, len);
    libwebsocket_write(wsi, b, len, LWS_WRITE_TEXT);
  }
  std::vector<unsigned char> buf;
};
 
typedef transfer::server<libwebsocket*, fun_send> websocket_server;
websocket_server *the_server;

static int callback_tf_server(struct libwebsocket_context *context,
                                   struct libwebsocket *wsi,
                                   enum libwebsocket_callback_reasons reason,
                                   void *user, void *in, size_t len)
{
  switch(reason)
  {
    case LWS_CALLBACK_ESTABLISHED:
      the_server-> on_open(wsi);
      break;
    case LWS_CALLBACK_RECEIVE:
      the_server-> on_message(wsi, (unsigned char*)in, len);
      break;
    case LWS_CALLBACK_CLOSED:
      the_server-> on_close(wsi);
      break;
  }
  return 0;
}

static struct libwebsocket_protocols protocols[] = {
    /* first protocol must always be HTTP handler */
    {
        "http-only",   // name
        callback_http, // callback
        0              // per_session_data_size
    },
    {
        "htc-transfer-protocol", // protocol name - very important!
        callback_tf_server,   // callback
        0
    },
    {
        NULL, NULL, 0   /* End of list */
    }
};

#endif
