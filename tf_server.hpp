#ifndef _TF_SERVER_HPP_
#define _TF_SERVER_HPP_

#include <unordered_map>
#include <functional>
#include "common.hpp"

namespace transfer{

template<typename Tcon_ptr, typename Fun_send, typename Tcon_hasher=std::hash<Tcon_ptr> >
class server
{
public:
  void on_open(Tcon_ptr con)
  {
    cons_.insert(std::make_pair(con, 0));
  }

  void on_close(Tcon_ptr con)
  {
    auto it= cons_.find(con);
    if (it== cons_.end()) return;
    auto it2= ids_.find(it->second);
    std::cerr<< "one client is disconnected. id="
             << it->second<< std::endl;
    cons_.erase(it);
    if (it2== ids_.end()) return;
    ids_.erase(it2);
  }

  void on_message(Tcon_ptr con, unsigned char* msg, size_t cnt)
  {
#define RET(x) {*msg= x;send_(con, msg, cnt);return;}
    if (cnt < HEAD_LEN) RET(ERR_PROTOCOL);
    unsigned char opcode= msg[0];
    id id_to(msg+OP_LEN);
    auto it= cons_.find(con);
    if (id_to != 0)
    { //normal request
      switch (opcode)
      {
        case ERR_PROTOCOL:
          return;
        case REQ_INIT:
          if (it->second != 0 || ids_.find(id_to) != ids_.end()) 
            RET(ERR_ID_CONFLICT);
          it->second = id_to;
          ids_.insert(std::make_pair(it->second, it->first));
          std::cerr<< "id="<< id_to<<" say hi~"<< std::endl;
          RET(SUCC_ID_REQ);
        case REQ_PING:
        case REQ_PONG:
        case REQ_DATA:
          if (it->second == 0) RET(ERR_PROTOCOL);
          auto it2= ids_.find(id_to);
          if (it2 == ids_.end()) RET(ERR_ID_NOT_FOUND);
          it-> second.assign(msg + OP_LEN);
          send_(it2->second, msg, cnt);

          std::cerr<< "passed one package."<< std::endl;
          return;
      }
    }
    else
    { //request server
      switch (opcode)
      {
        case ERR_PROTOCOL:
          return;
        case REQ_DATA:
          //broadcast the data
          if (it->second == 0) RET(ERR_PROTOCOL);
          it->second.assign(msg+OP_LEN);
          for (auto i=cons_.begin();i!=cons_.end();++i)
            send_(i->first, msg, cnt);

          std::cerr<< "broadcasted one package."<< std::endl;
          return;
        case REQ_PING:
          RET(REQ_PONG);
      }
    }
    RET(ERR_PROTOCOL);
#undef RET
  }

private:

  struct id_hasher {
    size_t operator()(const id& d)const 
      { return static_cast<size_t>(d); }
  };

  std::unordered_map<id, Tcon_ptr, id_hasher> ids_;
  std::unordered_map<Tcon_ptr, id, Tcon_hasher> cons_;
  Fun_send send_;
};

} //namespace transfer

#endif
