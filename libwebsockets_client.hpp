#ifndef _LWS_CLIENT_HPP_
#define _LWS_CLIENT_HPP_

#include <iostream>
#include <libwebsockets.h>
#include "tf_client.hpp"

static int callback_http(struct libwebsocket_context *context,
                         struct libwebsocket *wsi,
                         enum libwebsocket_callback_reasons reason, void *user,
                         void *in, size_t len)
{
	return 0;
}

class websocket_client : 
  public transfer::client<libwebsocket*, LWS_SEND_BUFFER_PRE_PADDING, LWS_SEND_BUFFER_POST_PADDING>
{
public:
  websocket_client(transfer::id d): id_(d) {}

  transfer::id get_id()const
  { return id_; }

  void raw_send(libwebsocket *wsi, void *msg, size_t cnt)
  { libwebsocket_write(wsi, (unsigned char*)msg, cnt, LWS_WRITE_TEXT); }

  void on_data(const transfer::id id, void *in, size_t cnt)
  {
    char* msg= (char*)in;
    msg[cnt]= 0;
    std::cout<< "from "<< id<< ':'<< msg<< std::endl;
  }

private:
  transfer::id id_;
};

websocket_client *the_client;

static int callback_tf_client(struct libwebsocket_context *context,
                                   struct libwebsocket *wsi,
                                   enum libwebsocket_callback_reasons reason,
                                   void *user, void *in, size_t len)
{
  switch(reason)
  {
    case LWS_CALLBACK_ESTABLISHED:
      std::cout<<"linked."<<std::endl;
      the_client-> on_established(wsi);
      the_client-> set_id(the_client-> get_id());
      break;
    case LWS_CALLBACK_RECEIVE:
      the_client-> on_message((unsigned char*)in, len);
      break;
    default:
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
        callback_tf_client,   // callback
        0
    },
    {
        NULL, NULL, 0   /* End of list */
    }
};

#endif
