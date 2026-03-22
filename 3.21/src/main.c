#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "lcd.h"
#include "event.h"
#include "menu.h"
#include "dcim.h"

int exit_flag = 0;
int exit_run = 1;

pthread_t tid_input;
pthread_t tid_exit;

// 退出线程（不再读设备！）
void* exit_thread(void* arg)
{
    while (exit_run)
    {
        // 只在子界面检测退出
        if (interface_flag != 0)
        {
            struct point p = get_global_event();

            if (p.way == OTH &&
                p.x >= 800 &&
                p.y <= 100)
            {
                exit_flag = 1;

                clear_global_event(); // 防止事件残留
            }
        }

        usleep(20000);
    }
    return NULL;
}

int main()
{
    lcd_init();
    event_init();
    menu_init();

    pthread_create(&tid_input, NULL, input_thread, NULL);
    pthread_create(&tid_exit, NULL, exit_thread, NULL);

    while (1)
    {
        process_switch();

        if (menu_exit)
            break;
    }

    exit_run = 0;

    pthread_join(tid_input, NULL);
    pthread_join(tid_exit, NULL);

    return 0;
}