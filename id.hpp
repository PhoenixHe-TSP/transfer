#ifndef _ID_HPP_
#define _ID_HPP_

class id{
public:
  enum{ LEN = 4 };

  id(uint32_t d= 0): id_(d)
  {
  }

  template<typename Iter>
  void get_from(Iter i)
  {
    id_= (*i)|(*(i+1)<<8)|(*(i+2)<<16)|(*(i+3)<<24);
  }
  
  operator uint32_t()const
  {
    return id_;
  }

  template<typename Iter>
  Iter assign(Iter i)
  {
    *i++= id_ & 0x000000ff;
    *i++= id_ & 0x0000ff00;
    *i++= id_ & 0x00ff0000;
    *i++= id_ & 0xff000000;
    return i;
  }


private:
  uint32_t id_;
}i;

#endif //_ID_HPP_
