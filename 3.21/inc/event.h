#ifndef __EVENT_H__
#define __EVENT_H__

enum WAY
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    OTH
};

struct point
{
    enum WAY way;
    int x;
    int y;
};

extern int fd_event;

// 原始读取（仅输入线程使用）
struct point get_event(int fd);

// 新接口（所有地方用这个）
struct point get_global_event();

// 初始化
void event_init();

// 输入线程
void* input_thread(void* arg);

void clear_global_event();



#endif