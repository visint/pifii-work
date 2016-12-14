#include <stdio.h>   
#include <string.h>   
/***********************************************

mipsel-openwrt-linux-gcc  -fPIC -shared -o libopplug.so  bianxian.c
mipsel-openwrt-linux-gcc   bianxian.c

************************************************/

int get_devid(unsigned int *devid)
{
    FILE *fp= NULL; 
	char ch;

	char buffstr[32]; 
		
	if((fp=fopen("/dev/mtdblock2","r"))== NULL ) {
		printf("file cannot be opened/n"); 
	} 
	fgets(buffstr, 32, fp);
	

	*devid = *((int *)(buffstr+10));
	fclose(fp);
	fp=NULL;
	return 0;
}

int set_devid(unsigned int hwid)
{
    FILE *fp= NULL; 
	char ch;
	char bufexe[128];
	memset(bufexe,0,128);
	*(int *)bufexe = hwid;
	
	if((fp=fopen("/dev/mtdblock2","wb"))== NULL ) {
		printf("file cannot be opened\n"); 
	} 
	fseek(fp,10L,0);
	fwrite(bufexe,  4 ,1, fp);
	fclose(fp);
	fp=NULL;		

	return 0;
}

int read_mac(unsigned char *mac)
{
    FILE *fp= NULL; 
	char ch;
	char bufexe[128];
	char buffstr[4096]; 
		
	if((fp=fopen("/dev/mtdblock2","r"))== NULL ) {
		printf("file cannot be opened/n"); 
	} 
	fgets(buffstr, 32, fp);
//	printf("jiang %02X %02X %02X %02X %02X %02X\n",buffstr[6],buffstr[7],buffstr[8],buffstr[9],buffstr[10],buffstr[11]);

	
	mac[0] = buffstr[4];
	mac[1] = buffstr[5];
	mac[2] = buffstr[6];
	mac[3] = buffstr[7];
	mac[4] = buffstr[8];
	mac[5] = buffstr[9];
	fclose(fp);
	fp=NULL;
	
	return 0;
}

char *read_firmware(char *key)
{
	if (!strcmp(key, "CURVER"))
		return "1.6.9";
	else if (!strcmp(key, "VENDOR"))
		return "BLINK";
	else if (!strcmp(key, "PRODUCT"))
		return "WR4000";
	else
		return NULL;
}


int get_cpuinfo(char *data, int data_len)
{
	//cpu type
	strncpy(data, "MT7620", data_len);
	return strlen(data);
}

int get_confinfo(char *data, int data_len)
{
	//flash size + mem size
	strncpy(data, "8M_64M", data_len);
	return strlen(data);
}

/*

int main(int argc,char*argv[]){   

	int device = 16788;
	int tmp ;
	set_devid(device);
	get_devid(&tmp);
	printf("jiangyibo %d\n",tmp);
	
   
   return 0;
}
*/
