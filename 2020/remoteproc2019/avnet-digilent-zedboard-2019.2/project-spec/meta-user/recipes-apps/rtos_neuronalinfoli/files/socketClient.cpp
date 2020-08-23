#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

struct socketComm{
  int socket_fd;
  bool good=false;
  socketComm(std::string &ip, int port){
    struct sockaddr_in server_info;
    struct hostent *he = gethostbyname(ip.data());
    if(he != NULL){
      socket_fd = socket(AF_INET, SOCK_STREAM, 0);
      if(socket_fd != -1){
        server_info.sin_family = AF_INET;
        server_info.sin_port = htons(port);
        server_info.sin_addr = *((struct in_addr *)he->h_addr_list[0]);
        if(connect(socket_fd, (struct sockaddr *)&server_info, sizeof(struct sockaddr))>-1){
          good=true;
        }
      }
    }
  }
  void sendBuff(std::vector<float> copyData){
    if(good){
      if ((send(socket_fd,copyData.data(),copyData.size()*sizeof(float),0))== -1){}
    }
  }
  void closefd(){
    close(socket_fd);
  }
  bool isGood(){
    return good;
  }
};

