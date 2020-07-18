#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <memory>
#include <algorithm>
#include <fcntl.h>
#include <sys/mman.h>
#include <iomanip>
#include <cmath>
#include <unistd.h>
#include <functional>

template <typename T>
struct uio{
  using typeDel = std::function<void(T*)>;
  private:
    std::unique_ptr<T,typeDel> _ptr;
    std::string _name, _path;
    uint _size;
    uint _index;
    bool checkFileEqualToString(const std::string &strCheck,const std::string &fileName) const;
    bool exists_test(const std::string &fileName);
    std::string buildPathName(uint idx) const;
    void findPath(const std::string &name);
    uint getSize ();
    int getBuff();
  public:
    uio(const std::string &name);
    inline T& operator[](T idx);
};

template <typename T>
bool uio<T>::checkFileEqualToString(const std::string &strCheck, const std::string &fileName) const{
  auto t = std::ifstream(fileName);
  auto strFromFile = std::string((std::istreambuf_iterator<char>(t)),std::istreambuf_iterator<char>());
  strFromFile.erase(std::remove(strFromFile.begin(), strFromFile.end(), '\n'), strFromFile.end());
  return strCheck==strFromFile;
}

template <typename T>
bool uio<T>::exists_test (const std::string &fileName){
  auto t = std::ifstream(fileName);
  return t.good();
}

template <typename T>
std::string uio<T>::buildPathName(uint idx) const{
  return std::string("/sys/class/uio/uio")+std::to_string(idx)+std::string("/name");
}

template <typename T>
void uio<T>::findPath(const std::string &name){
  uint indx_count=0; bool notFound=true;
  auto pathToName = buildPathName(indx_count);
  while(notFound){
    auto areEq = checkFileEqualToString(name,pathToName);
    auto fexist = exists_test(pathToName);
    if((areEq)||(!fexist)){
      _path = std::string("dev/uio")+std::to_string(indx_count);
      _index = indx_count;
      notFound=false;
    }else{
      indx_count++;
      pathToName = buildPathName(indx_count);
    }
  }
}

template <typename T>
uint uio<T>::getSize (){
  auto sizePath = std::string("/sys/class/uio/uio")+std::to_string(_index)+std::string("/maps/map0/size");
  if (exists_test(sizePath)){
    auto t = std::ifstream(sizePath);
    auto strFromFile =  std::string((std::istreambuf_iterator<char>(t)),std::istreambuf_iterator<char>());
    _size=std::stoi(strFromFile,0,16);
  }
  else _size = 0;
  return _size;
}

template <typename T>
int uio<T>::getBuff(){
  findPath(_name);
  getSize();
  int fd = 0;
  auto uiod = std::string("/dev/uio")+_path[7];
  fd = open(uiod.data(), O_RDWR);
  if (fd < 1) {
    std::cout<<"Error: Invalid UIO device file :"<<uiod<<std::endl;
    return -1;
  }
  typeDel deleter = [&](T* res){munmap((void*)res,_size);};
  auto myPtr = std::unique_ptr<T,typeDel>((T *)mmap(NULL,_size,PROT_READ|PROT_WRITE, MAP_SHARED, fd,0),deleter);
  _ptr = std::move(myPtr);
  close(fd);
  return 0;
}

template <typename T>
uio<T>::uio(const std::string &name) : _name(name){
  getBuff();
}

template <typename T>
inline T& uio<T>::operator[](T idx){return _ptr.get()[idx];}

