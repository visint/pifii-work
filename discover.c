#include <stdio.h>  
#include <sys/socket.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <netdb.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <string.h>  
  
#include <sys/types.h>
 
//using namespace std;  
  
int main()  
{  
    setvbuf(stdout, NULL, _IONBF, 0);   
    fflush(stdout);   
  
    // 绑定地址  
    struct sockaddr_in addrto;  
    bzero(&addrto, sizeof(struct sockaddr_in));  
    addrto.sin_family = AF_INET;  
    addrto.sin_addr.s_addr = htonl(INADDR_ANY);  
    addrto.sin_port = htons(10001);  
      
    // 广播地址  
    struct sockaddr_in from;  
    bzero(&from, sizeof(struct sockaddr_in));  
    from.sin_family = AF_INET;  
    from.sin_addr.s_addr = htonl(INADDR_ANY);  
    from.sin_port = htons(10001);  
      
    int sock = -1;  
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)   
    {     
//        cout<<"socket error"<<endl;   
        return 1;  
    }     
  
    const int opt = 1;  
    //设置该套接字为广播类型，  
    int nb = 0;  
    nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));  
    if(nb == -1)  
    {  
  //      cout<<"set socket error..."<<endl;  
        return 1;  
    }  
  
    if(bind(sock,(struct sockaddr *)&(addrto), sizeof(struct sockaddr_in)) == -1)   
    {     
    //    cout<<"bind error..."<<endl;  
        return 1;  
    }  
  
    int len = sizeof(struct sockaddr_in);  
    int nlen = len;
    char smsg[1500] = {0};  

    char pifiimsg[] = {"{ \"Factory\":\"pifii\"}"};
  
    while(1)  
    {  
        //从广播地址接受消息  
        int ret=recvfrom(sock, smsg, 1500, 0, (struct sockaddr*)&from,(socklen_t*)&len);  
        if(ret<=0)  
        {  
      //      cout<<"read error...."<<sock<<endl;  
        }  
        else  
        {   
            
            printf("jiangyibo %s\t", smsg);     
            int ret=sendto(sock, pifiimsg, strlen(pifiimsg), 0, (struct sockaddr*)&from, nlen);        
        }  
  
        sleep(1);  
    }  
  
    return 0;  
}  

