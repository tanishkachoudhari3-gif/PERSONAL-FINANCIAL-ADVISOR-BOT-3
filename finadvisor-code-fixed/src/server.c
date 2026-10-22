#define _GNU_SOURCE
#include "engine.h"
#include <errno.h>
#include <math.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 3000
#define BUF 8192
static void send_text(int fd,const char*status,const char*type,const char*body){dprintf(fd,"HTTP/1.1 %s\r\nContent-Type: %s\r\nCache-Control: no-cache\r\nAccess-Control-Allow-Origin: *\r\nX-Content-Type-Options: nosniff\r\nContent-Length: %zu\r\nConnection: close\r\n\r\n%s",status,type,strlen(body),body);}
static int key_equal(const char*a,size_t n,const char*b){return strlen(b)==n&&memcmp(a,b,n)==0;}
static double query_num(const char*q,const char*key,double fallback){while(q&&*q){const char*end=strchr(q,'&');size_t len=end?(size_t)(end-q):strlen(q);const char*eq=memchr(q,'=',len);if(eq&&key_equal(q,(size_t)(eq-q),key)){char value[64];size_t n=len-(size_t)(eq+1-q);if(!n||n>=sizeof value)return fallback;memcpy(value,eq+1,n);value[n]=0;char*tail;errno=0;double v=strtod(value,&tail);return errno||tail==value||*tail||!isfinite(v)?fallback:v;}q=end?end+1:NULL;}return fallback;}
static double query_range(const char*q,const char*key,double fallback,double lo,double hi){double v=query_num(q,key,fallback);return v<lo?lo:(v>hi?hi:v);}
static void handle(int fd){char req[BUF]={0};ssize_t n=read(fd,req,sizeof(req)-1);if(n<=0)return;char method[8]={0},path[2048]={0};if(sscanf(req,"%7s %2047s",method,path)!=2){send_text(fd,"400 Bad Request","text/plain; charset=utf-8","Bad request\n");return;}if(strcmp(method,"GET")){send_text(fd,"405 Method Not Allowed","text/plain; charset=utf-8","GET only\n");return;}
 if(!strncmp(path,"/api/advice",11)&&(path[11]=='?'||path[11]==0)){const char*q=strchr(path,'?');q=q?q+1:"";FinanceInput in={query_range(q,"income",120000,0,1000000000),query_range(q,"expenses",55000,0,1000000000),query_range(q,"emi",20000,0,1000000000),query_range(q,"emergency",240000,0,1000000000),query_range(q,"age",29,18,100),(int)query_range(q,"risk",2,1,3),(int)query_range(q,"goal",0,0,3)};Advice a;char json[2048];calculate_advice(&in,&a);advice_json(&in,&a,json,sizeof json);send_text(fd,"200 OK","application/json; charset=utf-8",json);return;}
 if(!strcmp(path,"/manus-routes.json")){send_text(fd,"200 OK","application/json; charset=utf-8","{\"routes\":[{\"path\":\"/\",\"title\":\"FinAdvisor C\"}]}");return;}
 char*q=strchr(path,'?');if(q)*q=0;if(strstr(path,"..")||strchr(path,'%')){send_text(fd,"400 Bad Request","text/plain; charset=utf-8","Bad path\n");return;}const char*clean=!strcmp(path,"/")?"/index.html":path;char rel[2100];snprintf(rel,sizeof rel,"public%s",clean);FILE*f=fopen(rel,"rb");if(!f){send_text(fd,"404 Not Found","text/plain; charset=utf-8","Not found\n");return;}if(fseek(f,0,SEEK_END)){fclose(f);send_text(fd,"500 Internal Server Error","text/plain","Read error\n");return;}long size=ftell(f);if(size<0||size>10*1024*1024||fseek(f,0,SEEK_SET)){fclose(f);send_text(fd,"500 Internal Server Error","text/plain","Invalid file size\n");return;}char*body=malloc((size_t)size+1);if(!body){fclose(f);send_text(fd,"500 Internal Server Error","text/plain","Out of memory\n");return;}size_t got=fread(body,1,(size_t)size,f);fclose(f);body[got]=0;const char*type=strstr(rel,".css")?"text/css; charset=utf-8":strstr(rel,".js")?"application/javascript; charset=utf-8":"text/html; charset=utf-8";send_text(fd,"200 OK",type,body);free(body);}
int main(void){signal(SIGPIPE,SIG_IGN);int s=socket(AF_INET,SOCK_STREAM,0);if(s<0){perror("socket");return 1;}int one=1;setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&one,sizeof one);struct sockaddr_in a={.sin_family=AF_INET,.sin_port=htons(PORT),.sin_addr.s_addr=INADDR_ANY};if(bind(s,(struct sockaddr*)&a,sizeof a)<0||listen(s,32)<0){perror("bind/listen");close(s);return 1;}printf("FinAdvisor C server listening on http://0.0.0.0:%d\n",PORT);fflush(stdout);for(;;){int c=accept(s,0,0);if(c>=0){handle(c);close(c);}else if(errno!=EINTR)perror("accept");}}
