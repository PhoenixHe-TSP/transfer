#ifndef _TF_CLIENT_HPP_
#define _TF_CLIENT_HPP_

#include <cstring>
#include <vector>
#include "common.hpp"

namespace transfer{

template<typename Tcon_ptr, size_t PRE_BYTES=0, size_t POST_BYTES=0>
class client
{
public:
  virtual void raw_send(Tcon_ptr con, unsigned char *msg, size_t cnt)=0;
  virtual void on_setting_succ(const id){}
  virtual void on_data(const id, unsigned char *msg, size_t cnt){}
  virtual void on_pong(const id){}
  virtual void on_error(const id, unsigned char *msg, size_t cnt){}

  enum{ HEAD_LEN= 1+id::LEN };

  inline void set_id(const id to)
  { send_message(REQ_INIT, to); }

  inline void ping(const id to)
  { send_message(REQ_PING, to); }

  inline void send(const id to, unsigned char *msg, size_t cnt)
  { send_message(REQ_DATA, to, msg, cnt); }

  void on_message(Tcon_ptr con, unsigned char *msg, size_t cnt)
  {
    if (cnt < HEAD_LEN)
    {
      msg[0]= ERR_PROTOCOL;
      raw_send(con, msg, cnt);
      return;
    }
    con_= con;
    unsigned char opcode= msg[0];
    id id_from(msg+OP_LEN);
    switch (opcode)
    {
      case SUCC_ID_REQ:
        on_setting_succ(id_from);
        break;
      case REQ_PING:
        msg[0]= REQ_PONG;
        raw_send(con, msg, cnt);
        break;
      case REQ_PONG:
        on_pong(id_from);
        break;
      case REQ_DATA:
        on_data(id_from, msg+HEAD_LEN, cnt-HEAD_LEN);
        break;
      case ERR_ID_CONFLICT:
      case ERR_ID_NOT_FOUND:
      case ERR_PROTOCOL:
        on_error(id_from, msg, cnt);
        break;
      default:
        msg[0]= ERR_PROTOCOL;
        raw_send(con, msg, cnt);
        break;
    }
  }

private:
  void send_message(enum opcode_t op,const id to, 
                    unsigned char *msg=NULL, size_t cnt=0)
  {
    static std::vector<unsigned char> buf;
    buf.resize(PRE_BYTES + HEAD_LEN + cnt + POST_BYTES);
    unsigned char *b= buf.data() + PRE_BYTES;
    b[0]= op;
    to.assign(b + OP_LEN);
    if (msg) memcpy(b + PRE_BYTES + HEAD_LEN, msg, cnt);
    raw_send(con_, b, HEAD_LEN + cnt);
  }

  Tcon_ptr con_;
};

} //namespace transfer
#endif
