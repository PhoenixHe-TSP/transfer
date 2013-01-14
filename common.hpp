#ifndef _COMMON_HPP_
#define _COMMPN_HPP_

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
enum{ 
  REQ_INIT = 'I',
  REQ_DATA = 'D',
  REQ_PING = 'P',
  REQ_PONG = 'p',
  SUCC_ID_REQ      = 0xf0,
  ERR_ID_CONFLICT  = 0xf1,
  ERR_ID_NOT_FOUND = 0xf2,
  ERR_PROTOCOL     = 0xf3
};

#endif
