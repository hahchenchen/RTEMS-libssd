/*
 * Copyright (c) 2014 embedded brains GmbH.  All rights reserved.
 *
 *  embedded brains GmbH
 *  Dornierstr. 4
 *  82178 Puchheim
 *  Germany
 *  <rtems@embedded-brains.de>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <mDNSEmbeddedAPI.h>
#include <mDNSPosix.h>

#include <sys/select.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define TEST_NAME "LIBBSD FOOBAR SERVER"

#define PORT 21  
    #define BUFFER_SIZE 1024  
    #define MAX_QUE_CONN_NM 5  
    struct sockaddr_in server_sockaddr,client_sockaddr;  
    int sin_size,recvbytes,sendbytes;  
    int sockfd, client_fd;  
    char buf[BUFFER_SIZE];  
    char message[72];  
    void *thrd_send()  
    {  
    while(1){  
        memset(buf, 0, sizeof(buf));  
        printf("please input message:\n");  
        scanf("%s",&message);  
        sprintf(buf, "%s", message);  
    /*发送消息给服务器端*/  
    if ((sendbytes = send(client_fd, buf, strlen(buf), 0)) == -1)  
            {  
            perror("send");  
            exit(1);  
            }  
         }  
    }  
    int test_main()  
    {  
        pthread_t thread[1];  
    /*建立 socket 连接*/  
    if ((sockfd = socket(AF_INET,SOCK_STREAM,0))== -1)  
               {  
                   perror("socket");  
                   exit(1);  
               }  
       
      printf("Socket id = %d\n",sockfd);  
    /*设置 sockaddr_in 结构体中相关参数*/  
      server_sockaddr.sin_family = AF_INET;  
      server_sockaddr.sin_port = htons(PORT);  
      server_sockaddr.sin_addr.s_addr = INADDR_ANY;  
      bzero(&(server_sockaddr.sin_zero), 8);  
      int i = 1;/* 允许重复使用本地地址与套接字进行绑定 */  
      setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));  
    /*绑定函数 bind()*/  
        if (bind(sockfd, (struct sockaddr *)&server_sockaddr,sizeof(struct  
    sockaddr)) == -1)  
               {  
                   printf("error bind");  
                   exit(1);  
               }  
        printf("Bind success!\n");  
    /*调用 listen()函数,创建未处理请求的队列*/  
       if (listen(sockfd, MAX_QUE_CONN_NM) == -1)  
              {  
              	  printf("error listen");  
                
                  exit(1);  
               }  
       printf("Listening....\n");  
    /*调用 accept()函数,等待客户端的连接*/  
       if ((client_fd = accept(sockfd,(struct sockaddr *)&client_sockaddr,  
    &sin_size)) == -1)  
               {  printf("error accept");  
                //  perror("accept");  
                  exit(1);  
               }  
    while(1){  
      memset(buf , 0, sizeof(buf));  
    /*调用 recv()函数接收客户端的请求*/  
      if ((recvbytes = recv(client_fd, buf, BUFFER_SIZE, 0)) == -1)  
             {  
                 printf("error recv");  
                 exit(1);  
             }  
       else if(recvbytes==0)  
      {  
      printf("失败\n");  
      exit(1);  
       }  
      printf("Received a message: %s\n", buf);  
      pthread_create(&thread[0],NULL,thrd_send,(void*)0);  
       
    }  
    close(sockfd);  
    exit(0);  
    }  

#define DEFAULT_NETWORK_DHCPCD_ENABLE
//#define DEFAULT_NETWORK_DHCPCD_NO_DHCP_DISCOVERY
#define DEFAULT_NETWORK_SHELL

#include <rtems/bsd/test/default-network-init.h>
