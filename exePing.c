#include <stdio.h>   
#include <string.h>   
#include <pthread.h>
/***********************************************

mipsel-openwrt-linux-gcc -o softprobe exePing.c -lpthread

************************************************/

static pthread_mutex_t cyassl_ctx_mutex = PTHREAD_MUTEX_INITIALIZER;

#define LOCK_CYASSL_CTX() do { \
	pthread_mutex_lock(&cyassl_ctx_mutex); \
} while (0)

#define UNLOCK_CYASSL_CTX() do { \
	pthread_mutex_unlock(&cyassl_ctx_mutex); \
} while (0)



void *threadTcp(void *ip)
{
    FILE *fstream=NULL;     
    char buff[1024];  
	char buffstr[4096]; 
    int  errno = 0;   
    memset(buff,0,sizeof(buff));   
	memset(buffstr,0,sizeof(buffstr));   
	pthread_t id1,id2;

	FILE *fp; 
	char ch;
	char bufexe[128];

	sprintf(bufexe,"ping -c 5 %s",ip);
	
   
    if(NULL==(fstream=popen(bufexe,"r")))     
    {    
        fprintf(stderr,"execute command failed: %s",strerror(errno));     
        return -1;     
    }    
	int m=0;
	buff[0]='-';
	buff[1]='@';
	int index = 0;
//	strcpy( buffstr ,"@@ www.baidu.com\n");
//	m= strlen("@@ www.baidu.com\n");
    while(NULL!=fgets(buff+2, sizeof(buff)-2, fstream))    
    {  
       if(strstr(buff,"bad address") != NULL)
       	{
         	strcpy( buffstr + m,buff);
			m =m+ strlen(buff);
			strcpy( buffstr + m,"-@\n-@\n");
            break;
       	}
       if(buff[2]=='-'&&buff[3]=='-')
       {
          index ++ ;
       }
	   if(index > 0)
	   	{
	       strcpy( buffstr + m,buff);
	       m =m+ strlen(buff);
	   	}

	   	buff[0]='-';
	    buff[1]='@';
	
       // printf("%s",buff);  
		
    }

	buffstr[0]='@';
	buffstr[1]='@';
	
//	printf("%s",buffstr);   
    LOCK_CYASSL_CTX();

	if((fp=fopen("/tmp/softprobe_result","a+"))== NULL ) {
		printf("file cannot be opened/n"); 
		exit(1); 
	} 

	fwrite (buffstr,  strlen(buffstr) ,1, fp);

	fflush(fp);
    
	fclose(fp); 

    UNLOCK_CYASSL_CTX();
//     printf("ok\n");
	 pthread_exit((void*)0);

}


int main(int argc,char*argv[]){   


    pthread_t dbthread[10];
	 int ret;
	 
    int i;
    int rslt = 1;
    int threadid[10]={0,1,2,3,4,5,6,7,8,9};
	 char *str ;
    char ips[][256]={ "www.baidu.com", "www.qq.com", "www.163.com","www.fang.com", "www.sina.com", \
		 "www.taobao.com","www.tianya.com", "www.10001.cn", "www.10000.cn","www.zhihu.com"};
    if(argc < 2) 
    {
	   str  = strdup( "www.baidu.com,www.qq.com,www.163.com");
    } 
	else {
       str  = strdup( argv[1]);
    }
    char *p;
    char *bufftt;
    bufftt=str;
 //   p = strsep(&bufftt, "|");
	int index=0;
    while((p = strsep(&bufftt, ",")) != NULL)        /*???????????, ????????????(??)???????‘/0’*/ 
    {
        printf("%s", p);
		memset(ips[index],0,256);
		strcpy(ips[index++],p);
    }
	free(str);

	
	

 //   db_init();  //³õÊ¼»¯Ò»Ð©È«¾ÖÊý¾Ý

 
    for (i = 0; i<index ; i++)
    {
         //´´½¨10¸ö×ÓÏß³Ì
         rslt = pthread_create(&dbthread[i], NULL,threadTcp, ips[i]);
           

        if(rslt == -1)
        {
            printf("create [%d] thread FAILED\n", i);
            return 0;
        }
    }
    remove("/tmp/softprobe_result");


    //join10¸öthread
    for(i=0;i<index;i++)
    {
        //need some join-thread op;
        pthread_join(dbthread[i], NULL);
    }
	
	printf("jiangyibo");
    return 0;

}  

