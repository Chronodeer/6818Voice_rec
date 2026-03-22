#ifndef __LCD_H__
#define __LCD_H__

extern int *plcd;  // 只声明，不定义
extern int fd_lcd;
extern int interface_flag;

extern unsigned char Char0[];
extern unsigned char Char1[];
extern unsigned char Char2[];
extern unsigned char Char3[];
extern unsigned char Char4[];
extern unsigned char Char5[];
extern unsigned char Char6[];
extern unsigned char Char7[];
extern unsigned char Char8[];
extern unsigned char Char9[];


void lcd_init();
void display_point(int x,int y,int color);
void display_square(int x0,int y0,int w,int h,int color);
void display_circle(int x0,int y0,int r,int color);
void display_bmp(int x0,int y0,char *bmp_path);

#endif