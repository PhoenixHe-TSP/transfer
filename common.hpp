#ifndef _COMMON_HPP_
#define _COMMPN_HPP_

namespace transfer{
/* my transfer protocol:
 * op_code id [data]
 * id: 4 bytes, server's id is 0
 * op_code: 1 byte
 *   I    init id
 *   D    send data to
 *   P    ping
 *   p    pong
 *   0xf0 successful requested one id
 *   0xf1 id conflicts
 *   0xf2 id not found
 *   0xf3 protocol error
 */
enum opcode_t{ 
  REQ_INIT = 'I',
  REQ_DATA = 'D',
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

  id(const unsigned char* in)
  {
    id_= 0;
    for (int i=0;i<LEN;++i)
      id_= id_*256 + in[i];
  }
  
  operator uint32_t()const
  {
    return id_;
  }

  void assign(unsigned char* out)const
  {
    out[0]= (id_ & 0xff000000) >> 24;
    out[1]= (id_ & 0x00ff0000) >> 16;
    out[2]= (id_ & 0x0000ff00) >> 8;
    out[3]= (id_ & 0x000000ff);
  }

private:
  uint32_t id_;
};

enum{ OP_LEN = 1,
      HEAD_LEN = OP_LEN+id::LEN };

} //namespace transfer
#endif
