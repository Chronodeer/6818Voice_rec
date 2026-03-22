#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdlib.h>
#include <pthread.h>

#include "event.h"

int fd_event;

// 全局事件
static struct point global_event = {OTH, -1, -1};
static pthread_mutex_t event_lock = PTHREAD_MUTEX_INITIALIZER;

// 初始化
void event_init()
{
    fd_event = open("/dev/input/event0", O_RDWR);
    if (fd_event < 0)
    {
        perror("open event error");
    }
}

// 输入线程（唯一读触摸）
void* input_thread(void* arg)
{
    while (1)
    {
        struct point p = get_event(fd_event);

        pthread_mutex_lock(&event_lock);
        global_event = p;
        pthread_mutex_unlock(&event_lock);
    }
    return NULL;
}

// 获取事件（线程安全）
struct point get_global_event()
{
    struct point p;

    pthread_mutex_lock(&event_lock);
    p = global_event;
    pthread_mutex_unlock(&event_lock);

    return p;
}

// 清除事件（避免重复触发）
void clear_global_event()
{
    pthread_mutex_lock(&event_lock);
    global_event.way = OTH;
    global_event.x = -1;
    global_event.y = -1;
    pthread_mutex_unlock(&event_lock);
}

// 原始读取函数（不改）
struct point get_event(int fd)
{
    struct point p1 = {OTH, -1, -1};
    struct point p2 = {OTH, -1, -1};
    struct input_event ev;

    while (1)
    {
        read(fd, &ev, sizeof(ev));

        if (ev.type == EV_ABS && ev.code == ABS_X)
        {
            if (p1.x == -1)
                p1.x = ev.value;
            p2.x = ev.value;
        }
        else if (ev.type == EV_ABS && ev.code == ABS_Y)
        {
            if (p1.y == -1)
                p1.y = ev.value;
            p2.y = ev.value;
        }
        else if (ev.type == EV_KEY && ev.code == BTN_TOUCH && ev.value == 0)
        {
            break;
        }
    }

    if ((abs(p1.x - p2.x) <= 40 && abs(p1.y - p2.y) <= 40))
    {
        return p2;
    }
    else
    {
        if (abs(p2.y - p1.y) > abs(p2.x - p1.x))
        {
            p2.way = (p2.y > p1.y) ? DOWN : UP;
        }
        else
        {
            p2.way = (p2.x > p1.x) ? RIGHT : LEFT;
        }
        return p2;
    }
}