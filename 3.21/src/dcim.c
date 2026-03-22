#include <unistd.h>
#include "event.h"
#include "lcd.h"
#include "dcim.h"

extern int exit_flag;

void app_dcim_auto()
{
    char *bmp[] = {"1.bmp", "2.bmp", "3.bmp"};
    int i = 0;

    while (1)
    {
        if (exit_flag)
            break;

        display_bmp(0, 0, bmp[i]);
        
        for (int t = 0; t < 200; t++)   // 200 * 10ms = 2秒
        {
            if (exit_flag)
                break;

            usleep(10000);  // 10ms
        }

        if (exit_flag)
            break;

        i = (i + 1) % 3;
    }
}

void app_dcim()
{
    char *bmp[] = {"1.bmp", "2.bmp", "3.bmp"};
    int i = 0;

    display_bmp(0, 0, bmp[i]);

    while (1)
    {
        if (exit_flag)
            break;

        struct point p = get_global_event();

        if (p.way == RIGHT)
        {
            i = (i + 1) % 3;
            display_bmp(0, 0, bmp[i]);
            clear_global_event();
        }
        else if (p.way == LEFT)
        {
            i = (i + 2) % 3;
            display_bmp(0, 0, bmp[i]);
            clear_global_event();
        }

        usleep(20000);
    }
}