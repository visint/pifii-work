#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<arpa/inet.h>
#define MAXBUF 1500
#define BUFFER_SIZE 1024 
#define DEV_SIZE 20

/* 包头 */
typedef struct 
{ 
  int id; 
  int buf_size; 
}PackInfo; 
  
/* 接收包 */
typedef struct RecvPack 
{ 
  PackInfo head; 
  char devId[DEV_SIZE];
  char buf[BUFFER_SIZE]; 
} RecvPack; 

char cliBuff[1024];  

char * exeShell(char *comm){   
    FILE *fstream=NULL;     
 
    int  errno = 0;   
    memset(cliBuff,0,sizeof(cliBuff));   
   
    if(NULL==(fstream=popen("ls -l","r")))     
    {    
        fprintf(stderr,"execute command failed: %s",strerror(errno));     
        return NULL;     
    }    
    if(NULL!=fgets(cliBuff, sizeof(cliBuff), fstream))    
    {    
       // printf("%s",cliBuff);   
    }    
    else   
    {   
        pclose(fstream);   
        return NULL;   
    }   
    pclose(fstream);   

	return cliBuff;

}  




int main()
{
   int ssock;
   int clen;
   struct sockaddr_in server_addr;
   char buf[MAXBUF];
   char cliBuf[MAXBUF];
   RecvPack pack_info; 
   char *p;
	  
   if((ssock=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP))<0){
   perror("socket error:");
   exit(1);
   }
   clen = sizeof(server_addr);
   memset(&server_addr,0,sizeof(server_addr));
   server_addr.sin_family     =AF_INET;
   server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
   server_addr.sin_port       =htons(9527);
   if(connect(ssock,(struct sockaddr *)&server_addr,clen)<0){
   perror("connect error:");
   exit(1);
   }

   memset(&pack_info,0,sizeof(pack_info));
    
   pack_info.head.id = 1;
   memcpy(pack_info.devId,"jiangyibo",strlen("jiangyibo"));
   memcpy(pack_info.buf  ,"jiang"  ,strlen("jiang"));
	
   send(ssock, (char*)&pack_info, sizeof(pack_info),0);

   
   memset(buf,0,MAXBUF);
   if(read(ssock,buf,MAXBUF)<=0)
   {
      perror("read error:");
      exit(1);
   }

   printf("jiangyibo 1 %s\n",buf);

   while (1){
	   if(read(ssock,buf,MAXBUF)<=0)
	   {
	      perror("read error:");
	      return ;
	   }
	   printf("jiangyibo %s \n",buf);
       p = exeShell(buf);
       send(ssock, (char*)p, 1024,0);
   }
   
   close(ssock);
   printf("\nread: %s\n",buf);
   return 0;
}

