#include <stdio.h>
#include <unistd.h>

#include "event.h"
#include "lcd.h"
#include "menu.h"
#include "dcim.h"
#include "beep_led.h"

int menu_exit = 0;
int interface_flag = 0;

void menu_init()
{
    interface_flag = 0;
    sleep(1);
    menu_exit = 0;
    display_bmp(0, 0, "welcome.bmp");
}

// 菜单点击
void menu_detect()
{
    struct point p = get_global_event();

    if (p.way == OTH)
    {
        if (p.x >= 154 && p.x <= 461 && p.y >= 50 && p.y <= 300)
            interface_flag = 1;
        else if (p.x >= 564 && p.x <= 871 && p.y >= 50 && p.y <= 300)
            interface_flag = 2;
        else if (p.x >= 154 && p.x <= 461 && p.y >= 350 && p.y <= 600)
            interface_flag = 3;
        else if (p.x >= 564 && p.x <= 871 && p.y >= 350 && p.y <= 600)
            interface_flag = 4;

        clear_global_event();
    }
}

// 状态机
void process_switch()
{
    switch (interface_flag)
    {
        case 0:
            display_bmp(0, 0, "zj.bmp");
            menu_detect();
            break;

        case 1:
            app_dcim_auto();
            if (exit_flag)
            {
                interface_flag = 0;
                exit_flag = 0;
            }
            break;

        case 2:
            app_dcim();
            if (exit_flag)
            {
                interface_flag = 0;
                exit_flag = 0;
            }
            break;

        case 3:
            app_beep_led();
            if (exit_flag)
            {
                interface_flag = 0;
                exit_flag = 0;
            }
            break;

        case 4:
            //app_dcim();
            if (exit_flag)
            {
                interface_flag = 0;
                exit_flag = 0;
            }
            break;
    }

    usleep(20000);
}