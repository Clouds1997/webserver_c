#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <event2/event.h>

using namespace std;

void write_cd(evutil_socket_t fd, short what, void *arg){
    char buf[1024] = {0};
    static int num = 0;
    sprintf(buf, "hello world %d \n", num ++);
    write(fd, buf, sizeof(buf));
    sleep(1);
}

int main(int argc, char *argv[]){

    int fd = open("myfifo", O_WRONLY | O_NONBLOCK);
    if(fd == -1){
        perror("open");
        exit(1);
    }

    // 创建event_base
    struct event_base* base =  event_base_new();
    
    // 创建事件
    struct event* ev = NULL;
    ev = event_new(base, fd, EV_WRITE | EV_PERSIST, write_cd, NULL);

    //添加事件
    event_add(ev, NULL);

    // 事件循环
    event_base_dispatch(base);

    // 释放资源
    event_free(ev);
    event_base_free(base);
    close(fd);

    return 0;
}