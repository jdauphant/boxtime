#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

void launchNetworkSetup(char * argv[])
{
	pid_t pid;
  	int status, died;
    switch(pid=fork()){
    	case -1: puts("Can't fork");
     	case 0 : 
     		execv("/usr/sbin/networksetup",argv);
            exit(0); 
     	default: died= wait(&status);
    }
}

void changeProxy(char * portString)
{
	unsigned int port = 0;
	sscanf(portString, "%u", &port);
	if(port<=1024 || port>=65536)
	{
	 	printf("Port %u incorrect\n", port);
	 	exit(2);
	}
	printf("New port : %u\n", port);

	char * argv1[]={"networksetup","-setwebproxy", "Ethernet", "localhost", portString,  "off", NULL};
	launchNetworkSetup(argv1);
	char * argv2[]={"networksetup","-setwebproxy", "Wi-Fi", "localhost", portString,  "off", NULL};
	launchNetworkSetup(argv2);
	char * argv3[]={"networksetup","-setsecurewebproxy", "Ethernet", "localhost", portString,  "off", NULL};
	launchNetworkSetup(argv3);
	char * argv4[]={"networksetup","-setsecurewebproxy", "Wi-Fi", "localhost", portString,  "off", NULL};
	launchNetworkSetup(argv4);
}

void removeProxy()
{
	char * argv1[]={"networksetup","-setwebproxystate", "Ethernet", "off", NULL};
	launchNetworkSetup(argv1);
	char * argv2[]={"networksetup","-setwebproxystate", "Wi-Fi", "off", NULL};
	launchNetworkSetup(argv2);
	char * argv3[]={"networksetup","-setsecurewebproxystate", "Ethernet", "off", NULL};
	launchNetworkSetup(argv3);
	char * argv4[]={"networksetup","-setsecurewebproxystate", "Wi-Fi", "off", NULL};
	launchNetworkSetup(argv4);
}

int main(int argc, char *argv[])
{
 	if(argc<=1)  
 	{
 		puts("Need on|off");
 		puts("Usage: changelocalproxy on|off [port]");
 		exit(1);
 	}
 	if(0==strncmp(argv[1],"on",4))
 	{
 		if(3!=argc)
 		{
 			puts("Need port when on");
 			puts("Usage: changelocalproxy on|off [port]");
 			exit(2);
 		}
 		puts("Change proxy");
 		changeProxy(argv[2]);
 	}
 	else if(0==strncmp(argv[1],"off",4))
 	{
 		puts("Disable proxy");
 		removeProxy();
 	}
 	exit(3);
    return 0;	
}
