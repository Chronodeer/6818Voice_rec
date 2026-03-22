#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "lcd.h"
#include "event.h"
#include "beep_led.h"
#include "menu.h"
#include "dcim.h"
/* ================== 私有变量 ================== */

static int beep_fd = -1;
static int led_d7  = -1;
static int led_d8  = -1;
static int led_d9  = -1;
static int led_d10 = -1;
static int led_all = -1; // 如果存在，控制所有LED

/* ================== 私有函数 ================== */

// 内部：写设备
static void write_dev(int fd, int val)
{
    if (fd != -1)
        write(fd, &val, sizeof(val));
}

/* ================== 对外接口 ================== */

// 初始化
int beep_led_init()
{
    beep_fd = open("/sys/kernel/gec_ctrl/beep", O_RDWR);
    led_d7  = open("/sys/kernel/gec_ctrl/led_d7", O_RDWR);
    led_d8  = open("/sys/kernel/gec_ctrl/led_d8", O_RDWR);
    led_d9  = open("/sys/kernel/gec_ctrl/led_d9", O_RDWR);
    led_d10 = open("/sys/kernel/gec_ctrl/led_d10", O_RDWR);
    led_all = open("/sys/kernel/gec_ctrl/led_all", O_RDWR);

    if (beep_fd == -1)
    {
        perror("beep open error");
        return -1;
    }

    // 默认关闭
    beep_led_off();
    return 0;
}

// 蜂鸣器响 + LED状态
void beep_led_on()
{
    write_dev(beep_fd, 1);

    write_dev(led_d7, 1);   // D7亮
    write_dev(led_d10, 0);  // D10灭
}

// 蜂鸣器关闭
void beep_led_off()
{
    write_dev(beep_fd, 0);

    write_dev(led_d7, 0);
    write_dev(led_d10, 1);  // D10亮
}

// 释放资源
void beep_led_exit()
{
    beep_led_off();

    if (beep_fd != -1) close(beep_fd);
    if (led_d7  != -1) close(led_d7);
    if (led_d8  != -1) close(led_d8);
    if (led_d9  != -1) close(led_d9);
    if (led_d10 != -1) close(led_d10);
    if (led_all != -1) close(led_all);
}

void app_beep_led()
{
    display_bmp(0, 0, "fmq.bmp");
    beep_led_init();
    while (1)
    {
       if (exit_flag)
       {   
            beep_led_exit();
           break;
       }

       struct point p = get_global_event();
       if (p.x >= 99 && p.x <= 281 && p.y >= 454 && p.y <= 544) 
         {
              beep_led_on();
         }
         else if (p.x >= 765 && p.x <= 935 && p.y >= 452 && p.y <= 539) 
         {
              beep_led_off();
         }
        
    }   
}