#ifndef GapJunctionDriver
#define GapJunctionDriver

#include "uio.hpp"

struct MMIP{
  private:
    using memmaptype = volatile unsigned;
    uio<memmaptype> _uioIP;
    static const auto ipSizeOff = uint(0x10>>2);
    static const auto ipCtrlOff = uint(0x00>>2);
    static const auto ipfrOff = uint(0x18>>2);
    static const auto iplrOff = uint(0x20>>2);
  public:
    inline void start(uint nsize, uint firstRow,uint lastRow);
    void printStatus();
    MMIP(const std::string &name);
};
inline void MMIP::start(uint nsize, uint firstRow,uint lastRow){
  _uioIP[ipSizeOff] = nsize;
  _uioIP[ipfrOff] = firstRow;
  _uioIP[iplrOff] = lastRow;
  _uioIP[ipCtrlOff] = uint(0x01);
  _uioIP[ipCtrlOff] = uint(0x01);
}
void MMIP::printStatus(){
  std::cout<<"Size: "<<std::hex<< _uioIP[ipSizeOff]<<"\n";
  std::cout<<"FirstRow: "<<std::hex<< _uioIP[ipfrOff]<<"\n";
  std::cout<<"LastRow "<<std::hex<< _uioIP[iplrOff]<<"\n";
  std::cout<<"Status: "<<std::hex<< _uioIP[ipCtrlOff]<<"\n";
}

MMIP::MMIP(const std::string &name):_uioIP(name){}

struct AXIDMA{
  using memmaptype = volatile unsigned;
  private:
    uio<memmaptype> _uioDMA;
    const uint mm2sOff = uint(0x00>>2);
    const uint s2mmOff = uint(0x30>>2);
    const uint dmaCtrlOff = uint(0x00>>2);
    const uint dmaPhyOff = uint(0x18>>2);
    const uint dmaLenOff = uint(0x28>>2);
  public:
    inline void reset();
    inline void mm2s_start(uint nsize, uint phyaddr);
    inline void s2mm_start(uint nsize, uint phyaddr,uint outputOffset);
    void wait_interrupt();
    AXIDMA(const std::string &name);
};
inline void AXIDMA::reset(){
  _uioDMA[mm2sOff+dmaCtrlOff] = 0x100;
  _uioDMA[mm2sOff+dmaCtrlOff] = 0x000;
  _uioDMA[s2mmOff+dmaCtrlOff] = 0x100;
  _uioDMA[s2mmOff+dmaCtrlOff] = 0x000;
}
inline void AXIDMA::mm2s_start(uint nsize, uint phyaddr){
  _uioDMA[mm2sOff+dmaPhyOff] = phyaddr;
  _uioDMA[mm2sOff+dmaCtrlOff] = uint((0x1<<12)|1);
  _uioDMA[mm2sOff+dmaLenOff] = nsize*(nsize+1)*4;
}
inline void AXIDMA::s2mm_start(uint nsize, uint phyaddr,uint outputOffset){
  _uioDMA[s2mmOff+dmaPhyOff] = phyaddr+(outputOffset*4);
  _uioDMA[s2mmOff+dmaCtrlOff] = uint((0x1<<12)|1);
  _uioDMA[s2mmOff+dmaLenOff] = nsize*4;
}
void AXIDMA::wait_interrupt(){
  uint Finish = 0;
  //std::cout<<std::hex<< _uioDMA[mm2sOff+dmaCtrlOff+1]<<"\n";
  //std::cout<<std::hex<< _uioDMA[s2mmOff+dmaCtrlOff+1]<<"\n";
  
  while(Finish != 1) {
    Finish = _uioDMA[s2mmOff+dmaCtrlOff+1]>>1;
    Finish &= 1;
  }
}
AXIDMA::AXIDMA(const std::string &name):_uioDMA(name){}

template<typename T>
struct UDMABUFF{
  private:
    using Del = std::function<void(T*)>;
    std::unique_ptr<T,Del> _ptr;
    static const auto _size = uint(134217728);
    static const auto _cacheEnable = false;
    int _mode = O_RDWR | O_SYNC;
  public:
    static const auto _phyaddr = uint(0x10000000);
    UDMABUFF(const std::string &name);
    inline T& operator[](uint idx);
};

template<typename T>
UDMABUFF<T>::UDMABUFF(const std::string &name){
  if(_cacheEnable){_mode = O_RDWR;};
  auto fd = open(("/dev/"+name).data(), _mode);
  auto deleter = [=](T* res){munmap((void *)res,_size);};
  auto ptr = std::unique_ptr<T,Del>{(T*)mmap(NULL,_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0),
      deleter};
  _ptr=std::move(ptr);
  close(fd);
}

template<typename T>
inline T& UDMABUFF<T>::operator[](uint idx){return _ptr.get()[idx];}


struct GapJunctionIP{
  private:
    using modprec = float;
    const std::string gjipName{"GapJunctionIP"};
    const std::string axidmaName{"dma"};
    const std::string udmabufName{"udmabuf0"};
    MMIP _gjip;
    AXIDMA _dma;
    UDMABUFF<modprec> _buff;
    const uint _nsize;
    const uint _frow;
    const uint _lrow;
    const uint _icOff;
    const uint _conducOff;
    static const uint BLOCK_SIZE = 4;

  public:
//TODO    GapJunctionIP(uint,uint,uint);
    GapJunctionIP(uint nsize);
    inline modprec& VdendBuff(uint idx);
    inline modprec& CondBuff(uint idx);
    inline modprec& IcBuff(uint idx);
    template<typename container>
    void copyCondMatrix(const container &cond);
    template<typename container>
    void copyVdend(const container &vdend);
    void run();
};
//TODO GapJunctionIP::GapJunctionIP(uint nsize, uint frow, uint lrow):
//   _nsize(nsize),_frow(frow),_lrow(lrow),
//   _gjip(gjipName),_dma(axidmaName), _buff(udmabufName){};

GapJunctionIP::GapJunctionIP(uint nsize):
  _gjip(gjipName),_dma(axidmaName),_buff(udmabufName),
  _nsize(nsize),_frow(0),_lrow(nsize),
  _icOff(nsize*(nsize+1)),
  _conducOff(nsize){}

inline GapJunctionIP::modprec& GapJunctionIP::VdendBuff(uint idx){return _buff[idx];}
inline GapJunctionIP::modprec& GapJunctionIP::CondBuff(uint idx){return _buff[_conducOff+idx];}
inline GapJunctionIP::modprec& GapJunctionIP::IcBuff(uint idx){return _buff[_icOff+idx];}

template<typename container>
void GapJunctionIP::copyCondMatrix(const container &cond){
  const uint BLOCK_NUMBERS = _nsize/BLOCK_SIZE;
  auto c_idx = uint(0);
  for(auto RowOfBlocks=uint(0); RowOfBlocks<BLOCK_NUMBERS; ++RowOfBlocks){
    for(auto block=uint(0); block<BLOCK_NUMBERS; ++block){
      for(auto row=RowOfBlocks*BLOCK_SIZE; row<RowOfBlocks*BLOCK_SIZE+BLOCK_SIZE; ++row){
        for(auto column=block*BLOCK_SIZE;column<block*BLOCK_SIZE+BLOCK_SIZE;++column){
             CondBuff(c_idx)=cond[(row*_nsize)+column];
             ++c_idx;
        }
      }
    }
  }
}

template<typename container>
void GapJunctionIP::copyVdend(const container &vdend){
  uint i = 0;
  for(auto &v : vdend){
    VdendBuff(i)=v; i++;
  }
}

void GapJunctionIP::run(){
//  _gjip.printStatus(); 
  printf("Run 1\n");
  _gjip.start(_nsize,0,_nsize);
  printf("Run 2\n");  
  _dma.reset();
  printf("Run 3\n");  
  _dma.mm2s_start(_nsize,_buff._phyaddr);
  printf("Run 4\n");  
  _dma.s2mm_start(_nsize,_buff._phyaddr,_icOff);
  printf("Run 5\n");  
//  _gjip.printStatus();
  _dma.wait_interrupt();
  printf("Run 6\n");  
}

#endif

