#ifndef _COMMON_HPP_
#define _COMMPN_HPP_

#include <cstdint>
#include <iostream>

namespace transfer{

using namespace std;

/* my transfer protocol:
 * op_code id [data]
 * id: 4 bytes, server's id is 0
 * op_code: 1 byte
 *   I    init id
 *   D    send data to
 *   C    close connection
 *   P    ping
 *   p    pong
 *   s    successful requested one id
 *   c    id conflicts
 *   n    id not found
 *   t    protocol error
 */
enum opcode_t{ 
  REQ_INIT = 'I',
  REQ_DATA = 'D',
  REQ_CLOS = 'C',
  REQ_PING = 'P',
  REQ_PONG = 'p',
  SUCC_ID_REQ      = 's',
  ERR_ID_CONFLICT  = 'c',
  ERR_ID_NOT_FOUND = 'n',
  ERR_PROTOCOL     = 't'
};

class id{
public:
  enum{ LEN = 4 };

  id(uint32_t d): id_(d) {}

  id(): id((uint32_t)0) {}

  id(void* in)
  { id_= *((uint32_t*)in); }

  id(char* in): id((void*)in){}
  
  operator uint32_t()const
  { return id_; }

  void assign(void* out)const
  { *((uint32_t*)out)= id_; }

  friend ostream& operator<<(ostream& output, const id d)
  {
    char buf[5];
    d.assign(buf);
    buf[4]= 0;
    return output<< (char*)buf;
  }

private:
  uint32_t id_;
};

enum{ OP_LEN = 1,
      HEAD_LEN = OP_LEN+id::LEN };

} //namespace transfer
#endif
