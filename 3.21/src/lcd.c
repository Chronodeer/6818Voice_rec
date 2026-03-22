#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include "lcd.h"
#include <stdlib.h>


int *plcd;
int fd_lcd;

void lcd_init()
{
    fd_lcd = open("/dev/fb0", O_RDWR);
    if (fd_lcd < 0)
    {
        perror("open lcd error");
        return;
    }

    plcd = mmap(NULL, 800 * 480 * 4,
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                fd_lcd, 0);
}

void display_point(int x, int y, int color)
{
	if (x >= 0 && x < 800 && y >= 0 && y < 480) //	防憨批设置
	{
		*(plcd + 800 * y + x) = color;
	}
}

/*
	display_square:	在屏幕坐标(x0,y0)为 起点显示一个宽为 w,高为 h,颜色为 color的矩形
	@x0:			待显示矩形在显示屏起始横坐标
	@y0:			待显示矩形在显示屏起始纵坐标
	@w:				待显示矩形宽
	@h:				待显示矩形高
	@color:			待显示矩形的颜色
*/
void display_square(int x0, int y0, int w, int h, int color)
{
	for (int y = y0; y < y0 + h; y++) //	一行一行显示
	{
		for (int x = x0; x < x0 + w; x++) //	一行的每一个像素点显示
		{
			display_point(x, y, color);
		}
	}
}

/*
	display_circle:	在屏幕坐标(x0,y0)为圆心显示一个半径为 r,颜色为 color的圆
	@x0:			待显示圆圆心在显示屏起始横坐标
	@y0:			待显示圆圆心在显示屏起始纵坐标
	@r:				待显示圆的半径
	@color:			待显示圆的颜色
	@plcd:			第一个像素点的地址(mmap 的返回值)
*/
void display_circle(int x0, int y0, int r, int color)
{
	for (int y = y0 - r; y < y0 + r; y++) //	一行一行显示
	{
		for (int x = x0 - r; x < x0 + r; x++) //	一行的每一个像素点显示
		{
			if ((x - x0) * (x - x0) + (y - y0) * (y - y0) <= r * r) //	点到圆心的距离小于等于半径
			{
				display_point(x, y, color);
			}
		}
	}
}

/*
	display_bmp:	在屏幕坐标(x0,y0)为起始点绘制bmp图片
	@x0:			待显示BMP图片在显示屏起始横坐标
	@y0:			待显示BMP图片在显示屏起始纵坐标
	@bmp_path:		待显示BMP图片路径名(相对/绝对)
*/
void display_bmp(int x0, int y0, char *bmp_path)
{
	//	1、解析图片文件并获取像素数组
	//	1.1	打开图片文件
	int fd = open(bmp_path, O_RDWR); //	以读写方式打开图片文件
	if (fd == -1)
	{
		perror("opne bmp error");
		return;
	}

	//	1.2 判断是不是 BMP 图片
	char flag[2] = {0}; //	判断是否为 BMP
	read(fd, flag, 2);
	if (!(flag[0] == 'B' && flag[1] == 'M'))
	{
		printf("The pictrue not's BMP\n"); //	BMP 图片不是转换
		return;
	}

	//	1.3 解析得到相关的 位宽 位高 色深的信息
	int width, height;		   //	位宽 位高
	short depth;			   //	色深
	lseek(fd, 0x12, SEEK_SET); // 偏移对应位置,接下来的数据为位图宽度数据
	read(fd, &width, 4);
	lseek(fd, 0x16, SEEK_SET); // 偏移对应位置,接下来的数据为位图高度数据
	read(fd, &height, 4);
	lseek(fd, 0x1c, SEEK_SET); // 偏移对应位置,接下来的数据为位图色深数据
	read(fd, &depth, 2);
	//	获取成功,将相关数据进行打印
	// printf("%s : %d - %d - %d\n",bmp_path,width,height,depth);

	//	1.4 获取像素数组
	//	一行实际的大小:
	int line_bytes = abs(width) * depth / 8; //	一行实际的字节
	int laizi = 0;
	if (line_bytes % 4)
	{
		laizi = 4 - (line_bytes % 4); //	得到填充的字节数量
	}
	//	一行总的字节:	实际 + 癞子
	int linebytes = line_bytes + laizi;
	//	总大小为:	一行总的字节 * 行数
	int bytes = linebytes * abs(height);

	//	获取像素数组
	unsigned char buf[bytes];  //	保存像素数组
	lseek(fd, 0x36, SEEK_SET); //	偏移对应位置
	read(fd, buf, bytes);	   //	读取像素数组的每个字节

	//	1.5 关闭图片文件
	close(fd);

	//	2、通过像素数组显示
	unsigned char a, r, g, b;		  // 	每个像素点的颜色值分量信息
	int x, y;						  //	像素数组存储的像素点的坐标,不是显示的坐标
	int i = 0;						  // 	像素数组的下标(像素数组保存的每个元素都是颜色值分量)
	for (y = 0; y < abs(height); y++) //	一共有多少行像素点,一行一行的显示
	{
		for (x = 0; x < abs(width); x++) // 一行有多少个像素点,一行中一个个像素点显示
		{
			// 	依次得到每个像素点颜色值分量信息
			b = buf[i++];	 //	小端模式,先获取蓝色分量
			g = buf[i++];	 //	小端模式,再获取绿色分量
			r = buf[i++];	 //	小端模式,再获取红色分量
			if (depth == 32) // 	只有色深为 32 才有 a 分量
			{
				a = buf[i++]; //	小端模式,最后获取透明度分量
			}
			else
			{
				a = 0;
			}
			int color = a << 24 | r << 16 | g << 8 | b; //	当前像素点的颜色值
			display_point(width > 0 ? x + x0 : abs(width) - 1 - x + x0,
						  height > 0 ? height - 1 - y + y0 : y + y0, color);
			// 宽度 > 0 左到右保存,< 0 右到左保存
			// 高度 > 0 下到上保存,< 0 上到下保存
			// 从 0 计数
		}
		i += laizi; // 	存储时每一行会填充无效字节,跳过填充的数据,不需要显示
	}
}
