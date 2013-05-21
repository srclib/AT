/*
 * Copyright (C) 2009 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//#define LOG_NDEBUG 0
#define LOG_TAG "srclib.huyanwei.AT"

#define TAG	LOG_TAG

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/statfs.h>
#include <sys/times.h> 
#include <sys/select.h> 

#include <linux/fb.h>
#include <linux/input.h>

#include <dlfcn.h>
#include <dirent.h>
#include <fcntl.h>
#include <pthread.h>
#include <math.h>


#if defined(ANDROID)
#include <utils/Log.h>
#endif

#ifdef __cplusplus 
extern "C" { 
#endif 

#ifdef __cplusplus 
}
#endif 

#include <termios.h>


pthread_mutex_t device_mutex = PTHREAD_MUTEX_INITIALIZER;

void usage(const char * name)
{
	printf("\r\nusage : %s -d device_path -t wait_second -c at_commmand \r\n",name);
	printf("%s -d /dev/ttyC0 -t 3 -c AT_CMD\r\n",name);
	printf("%s -d /dev/ttyC0 -t 3 -c at\r\n",name);
	printf("%s -d /dev/ttyC0 -t 3 -c ati\r\n",name);
	printf("\r\nAuthor:huyanwei\r\n");
	printf("Email :srclib@hotmail.com\r\n");
	return ;
}

int main(int argc, char **argv)
{
    int c = '?';

	int i   =  0 ;
	int fd  = -1 ;
	int ret = -1 ;

	int readable = 0 ;
	fd_set read_set;
	struct timeval timeout = 
	{
		.tv_sec  = 1,
		.tv_usec = 0
	};

    int retry_max_times = 3 ;
	int retry_times = 0 ;

	char device_name[128] = { 0 } ;
	char at_command[128]  = { 0 } ;

	char buffer[258];   // +2 for \r\n

	memset(device_name,0,sizeof(device_name));
	memset(at_command,0,sizeof(at_command));


    do {
        c = getopt(argc, argv, "d:t:c:h");

        #if 0
        printf("optarg=%s \n",optarg);
        printf("optopt=%c \n",optopt);
        printf("optind=%d \n",optind);
        printf("c=0x%x \n",c);
        #endif


        if (c == -1)
            break;

        switch (c)
        {
            case 'd':
				strcpy((char *)&device_name[0],optarg);
                break;
            case 't':
				retry_max_times = strtol(optarg, NULL, 0);
                break;
            case 'c':
				strcpy((char *)&at_command[0],optarg);
                break;
            case 'h':
                usage(argv[0]);
                return -1;
            case '?':
                fprintf(stderr, "%s: invalid option -%c\n",
                argv[0], optopt);
                usage(argv[0]);
                exit(1);
        }
    } while (1);

    if(optind > argc) {
        fprintf(stdout, "%s: too few arguments\n", argv[0]);
        exit(1);
    }

	if(device_name[0] == '\0')
	{
		strcpy((char *)&device_name[0],"/dev/ttyC0");
	}

	//printf("huyanwei debug device name is %s\r\n", device_name);

	fd = open(device_name,O_RDWR | O_NONBLOCK);
	if(fd < 0)
	{
	    printf("Fail to open device: %s\n", strerror(errno));
    	return -1;
    }

	for (i = 0; i<30; i++) 
		usleep(50000); //sleep 1s wait for modem bootup

	memset(buffer,0,sizeof(buffer));
	strncpy(buffer,at_command,(strlen(at_command)>127)?(127):(strlen(at_command)));
	strcat(buffer,"\r\n");

    if (pthread_mutex_lock (&device_mutex))
    {
        printf("pthread_mutex_lock ERROR!\n");
    }

	ret = write(fd,buffer,strlen(buffer));
	if(ret < 0)	
	{
		printf("write device failed .\r\n");
	}	
	else
	{
		printf("%s",buffer); // already attch \r\n
		printf("+----------------------------------------+\r\n");
	}

	while(1)
	{
		FD_ZERO(&read_set);
		FD_SET(fd,&read_set);

		ret = select(fd+1,&read_set,NULL,NULL,&timeout);
		switch (ret)	
		{
			case -1:
					printf("select state error.\r\n");
					break;
			case 0 :
					retry_times ++ ;
					break;
			default:
					if(FD_ISSET(fd,&read_set))
					{
						readable = 1 ;
						if(read(fd,buffer,sizeof(buffer)-1) >= 0 )
						{
							printf("%s\r\n",buffer);
						}
					}
					break;
		}
		
		if(readable == 1)
			break;

		if(retry_times > retry_max_times )
		{
			printf("read device time out.\r\n");
			break;
		}
	}

    if (pthread_mutex_unlock (&device_mutex))
    {
        printf("pthread_mutex_unlock ERROR!\n");
    }

	close(fd);
	fd = -1;

	return 0;
}




