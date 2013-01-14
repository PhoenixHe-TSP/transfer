#ifndef _H_TF_CLIENT_
#define _H_TF_CLIENT_

#include <iostream>
#include <string>
#include <websocketpp/roles/client.hpp>
#include <websocketpp/websocketpp.hpp>
#include "id.hpp"
#include "common.hpp"

using websocketpp::client;

class tf_client_handler : public client::handler
{
public:
  enum{ HEAD_LEN= 1+id::LEN };
  void set_id(const id d);
  void ping(const id to);
  void send(const id to, const char* in, size_t cnt);
  void send(const id to, const std::string& msg);
  void close()
  {
    con_.
  }

  virtual void on_setting_succ(const id){}
  virtual void on_data(const id, const std::string& msg){}
  virtual void on_pong(const id){}
  virtual void on_error(const id, const std::string& msg){}

private:

  void on_open

  void on_message(connection_ptr con, message_ptr msg_p)
  {
#define RET(x) {msg[0]= x;con->send(msg, websocketpp::frame::opcode::BINARY);return;}
    std::string msg(msg_p->get_payload());
    if (msg.size()< HEAD_LEN) RET(ERR_PROTOCOL);
    auto opcode= msg[0];
    id id_from; id_from.get_from(msg.begin()+1);
    switch (opcode)
    {
      case REQ_PING:
        RET(REQ_PONG);
      case REQ_PONG:
        on_pong(id_from);
        return;
      case REQ_DATA:
        on_data(id_from, msg);
        return;
      case SUCC_ID_REQ:
        on_setting_succ(id_from);
        return;
      case ERR_ID_CONFLIT:
      case ERR_ID_NOT_FOUND:
      case ERR_PROTOCOL:
        on_error(id_from, msg);
        return;
    }
    RET(ERR_PROTOCOL);
#undef RET
  }

  connection_ptr con_;
};

#endif
