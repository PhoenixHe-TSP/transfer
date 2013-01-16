#ifndef _TF_CLIENT_HPP_
#define _TF_CLIENT_HPP_

#include <cstring>
#include <boost/thread/mutex.hpp>
#include <vector>
#include "common.hpp"

namespace transfer{

template<typename Tcon_ptr, size_t PRE_BYTES=0, size_t POST_BYTES=0>
class client
{
public:
  client(): con_(0){}

  virtual void raw_send(Tcon_ptr con, void *msg, size_t cnt)=0;
  virtual void on_setting_succ(const id){}
  virtual void on_data(const id, void *msg, size_t cnt){}
  virtual void on_pong(const id){}
  virtual void on_error(const id, void *msg, size_t cnt){}

  inline void set_id(const id to)
  { send_message(REQ_INIT, to); }

  inline void ping(const id to)
  { send_message(REQ_PING, to); }

  inline void send(const id to, void *msg, size_t cnt)
  { send_message(REQ_DATA, to, msg, cnt); }

  void on_established(Tcon_ptr con)
  { con_= con; }

  void on_message(void *in, size_t cnt)
  {
    static boost::mutex mtx;
    boost::mutex::scoped_lock lock(mtx);

    if (cnt == 0 || !con_) return;
    unsigned char *msg= (unsigned char*)in;
    if (cnt < HEAD_LEN)
    {
      msg[0]= ERR_PROTOCOL;
      safe_raw_send(msg, cnt);
      return;
    }
    opcode_t opcode= (opcode_t)msg[0];
    id id_from(msg+OP_LEN);
    switch (opcode)
    {
      case SUCC_ID_REQ:
        on_setting_succ(id_from);
        break;
      case REQ_PING:
        msg[0]= REQ_PONG;
        safe_raw_send(msg, cnt);
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
        safe_raw_send(msg, cnt);
        break;
    }
  }

private:
  void send_message(enum opcode_t op,const id to, 
                    void *msg=NULL, size_t cnt=0)
  {
    static boost::mutex mtx;
    boost::mutex::scoped_lock lock(mtx);

    unsigned char* buf= new unsigned char[PRE_BYTES + HEAD_LEN + cnt + POST_BYTES];
    unsigned char* b= buf + PRE_BYTES;
    b[0]= op;
    to.assign(b + OP_LEN);
    memcpy(b + HEAD_LEN, msg, cnt);
    safe_raw_send(b, HEAD_LEN + cnt);
    delete[] buf;
  }

  void safe_raw_send(void *msg, size_t cnt)
  {
    static boost::mutex mtx;
    boost::mutex::scoped_lock lock(mtx);

    raw_send(con_, msg, cnt);
  }

  Tcon_ptr con_;
};

} //namespace transfer
#endif
