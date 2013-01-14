#ifndef _H_TF_SERVER_
#define _H_TF_SERVER_

#include <iostream>
#include <websocketpp/websocketpp.hpp>
#include <unordered_map>
#include "id.hpp"
#include "common.hpp"

using websocketpp::server;

class tf_server_handler : public server::handler
{
public:
  void on_open(connection_ptr con)
  {
    cons_.insert(std::make_pair(con, 0));
  }

  void on_close(connection_ptr con)
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

  void on_message(connection_ptr con, message_ptr msg_p)
  {
#define RET(x) {msg[0]= x;con->send(msg, websocketpp::frame::opcode::BINARY);return;}
    std::string msg(msg_p->get_payload());
    if (msg.size()<1+id::LEN) RET(ERR_PROTOCOL);
    auto opcode= msg[0];
    id id_to; id_to.get_from(msg.begin()+1);
    auto it= cons_.find(con);
    if (id_to != 0)
    { //normal request    
      switch (opcode)
      {
        case REQ_INIT:
          RET(req_id(it, id_to)? SUCC_ID_REQ: ERR_ID_CONFLICT);
        case REQ_PING:
        case REQ_PONG:
        case REQ_DATA:
          if (it->second == 0) RET(ERR_PROTOCOL);
          auto it2= ids_.find(id_to);
          if (it2 == ids_.end()) RET(ERR_ID_NOT_FOUND);
          it->second.assign(msg.begin()+1);
          it2->second->send(msg, websocketpp::frame::opcode::BINARY);

          std::cerr<< "passed one package."<< std::endl;
          return;
        case ERR_PROTOCOL:
          return;
      }
    }
    else
    { //request server
      switch (opcode)
      {
        case REQ_DATA:
          //broadcast the data
          if (it->second == 0) RET(ERR_PROTOCOL);
          it->second.assign(msg.begin()+1);
          for (auto i=cons_.begin();i!=cons_.end();++i)
            i->first->send(msg, websocketpp::frame::opcode::BINARY);

          std::cerr<< "broadcasted one package."<< std::endl;
          return;
        case REQ_PING:
          RET(REQ_PONG);
        case ERR_PROTOCOL:
          return;
      }
    }
    RET(ERR_PROTOCOL);
#undef RET
  }

private:

  template<typename Iter>
  bool req_id(Iter it, const id& d)
  {
    if (it->second != 0) return 0;
    if (ids_.find(d) != ids_.end()) return 0;
    it->second = d;
    ids_.insert(std::make_pair(it->second, it->first));
    std::cerr<< "id="<< d<<" say hi~"<< std::endl;
    return 1;
  }

  struct con_hasher {
    size_t operator()(const server::handler::connection_ptr& con)const {
      return reinterpret_cast<size_t>(con.get()); }
  };
  struct id_hasher {
    size_t operator()(const id& id)const {
      return static_cast<size_t>(id); }
  };

  std::unordered_map<id, connection_ptr, id_hasher> ids_;
  std::unordered_map<connection_ptr, id, con_hasher> cons_;
};

#endif
