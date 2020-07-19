#include<iostream>
#include<graphics.h>
#include<conio.h> //其中定义了通过控制台进行数据输入和数据输出的函数，主要是一些用户通过按键盘产生的对应操作，比如getch()函数等等。
#include<mmsystem.h>//与多媒体有关的接口
#include<stdlib.h>
#include<vector>
#include<deque>
#include<ctime>
#include<math.h>
#pragma comment(lib,"winmm.lib")

//用到的常数
const double pi = acos(-1); //反余弦函数
const double gen2 = sqrt(2);

//游戏场地信息
constexpr int area = 40;
constexpr int size = 16; //常量表达式constexpr是指值不会改变且在编译过程中就能够得到计算结果的表达式，能在编译时求值的表达式



MOUSEMSG m; //管理鼠标消息

//outtextxy输出字符串
wchar_t str[128]; //宽字符16位

//记录资金
int money = 0;

//记录属性
int hp = 12;
int hplevel = 1; //
int speed = 3;
int speedlevel = 1;
int cd = 3; //冷却时间
//攻击力太难计算，所以放在可选的位置上
//int attack = 2;  
//int attacklevel = 1;

COLORREF     little = GREEN, normal = YELLOW, huge = MAGENTA, tank = BROWN, exploder = RED;//基本信息
/*  属性
  移动速度		  速		     中 			 低		        慢			  快
   生命值		  低			 中				 高			    高			  低        */

   //玩家与电脑的基类
class basic
{
public:
	int x;
	int y;
	int hp;
	int speed;
	int r;
	COLORREF color;
};

//玩家类
class player :public basic
{
public:
	player(int h, int s)
	{
		hp = h;
		speed = s;
		color = BLUE;
		r = int(round(size / 1.5)); //四舍五入。玩家自身半径
		x = area * size / 2;
		y = area * size / 2;
	}
};

//电脑类
class ai :public basic
{
public:
	ai(int posx, int posy, int h, int s, int r, COLORREF c)
	{
		x = posx;
		y = posy;
		hp = h;
		speed = s;
		this->r = r;
		color = c;
	}
};

class death
{
public:
	int x;
	int y;
	int r;
	int time;
	COLORREF color;
	death(int posx, int posy, int r, COLORREF c)
	{
		x = posx;
		y = posy;
		this->r = r;
		time = 10;
		color = c;
	}
};

//子弹类
class bullet
{
public:
	int x;
	int y;
	int time;
	int dx;
	int dy;
	bullet(int posx, int posy, int t, int speedx, int speedy)
	{
		x = posx;
		y = posy;
		time = t;
		dx = speedx;
		dy = speedy;
	}
};

//函数列表
void init();
void show();
void game();

int main()
{
	srand(int(time(0)));
	initgraph(size * area, size * area);
	BeginBatchDraw();//开启双缓冲

	init();
	show();
	closegraph();
	return 0;
}

void init()
{
	setbkcolor(WHITE);//初始界面背景颜色
	cleardevice();//清屏
	setlinecolor(BLACK);//设置线条颜色为黑色

	setfillcolor(0xff0000);//area=40,size=16，设置填充颜色 填充矩阵，以下六行为设置填充矩阵的嵌套边框
	fillrectangle(area * size / 5 - size / 2, area * size / 6 - size / 2, area * size / 5 * 4 - size / 2, area * size / 6 * 2 - size / 2);
	
	setfillcolor(0xff00);
	fillrectangle(area * size / 5 - size / 4, area * size / 6 - size / 4, area * size / 5 * 4 - size / 4, area * size / 6 * 2 - size / 4);
	
	setfillcolor(0xff);
	fillrectangle(area * size / 5, area * size / 6, area * size / 5 * 4, area * size / 6 * 2);

	setbkmode(TRANSPARENT);//设置背景模式为透明
	settextstyle(area * size / 6, area * size / 20, L"华文宋体");//设置当前字体样式
	outtextxy(area * size / 4 - size / 8, area * size / 6 - size / 8, L"射击游戏");//在指定位置输出字符串
	settextcolor(0xf02010); //设置字体颜色,设计游戏的颜色
	outtextxy(area * size / 4, area * size / 6, L"射击游戏");

	setfillcolor(YELLOW);//Programming For VS2019 And EasyX
	settextstyle(size * 3, size * 3 / 2, L"华文宋体");
	fillrectangle(area * size / 5, area * size / 6 * 5 - 2 * size, area * size / 5 * 4, area * size / 6 * 5 + 2 * size);
	outtextxy(area * size / 5 + size / 2, area * size / 6 * 5 - 2 * size, L"按任意键继续");

	settextstyle(size, size / 4, L"华文宋体");
	settextcolor(0xaa00ff);
	outtextxy(area * size / 5 * 2, area * size / 10 * 9, L"");

	settextcolor(0xeeaa33);
	LOGFONT f; //LOGFONT是Windows内部字体的逻辑结构，主要用于设置字体格式，其定义如下：
	gettextstyle(&f);
	f.lfEscapement = 150; //设置字体格式
	f.lfOrientation = 150;
	settextstyle(&f);
	outtextxy(area * size / 3 * 2, area * size / 3 + size * 4, L"Version 0.1.2 By：ZZC1024");

	f.lfEscapement = 0;
	f.lfOrientation = 0;
	settextstyle(&f);//让字体格式变为默认值

	FlushBatchDraw();
	system("pause");
	setlinecolor(WHITE);
	for (int i = 0; i < area * size; i++)
	{
		line(0, i, area * size, i);//切换界面刷新所用
		Sleep(1);
		FlushBatchDraw();
	}
	setlinecolor(BLACK);
}

void show()
{
	while (true)
	{
		//初始化菜单
		constexpr int w = area * size;
		constexpr int h = w /30;
		settextstyle(h, h / 3, L"楷书");
		setbkcolor(WHITE);
		cleardevice();

		//显示玩家拥有的资金
		setbkmode(OPAQUE);
		setbkcolor(YELLOW);
		settextcolor(BLACK);
		wsprintf(str, L"金钱：%d", money);
		outtextxy(h * 6, h, str);
		setbkmode(TRANSPARENT);

		//玩家属性控制
		if (hplevel < 10)
		{
			if (m.x >= w / 5 && m.x <= w / 5 * 2 && m.y >= h * 3 && m.y <= h * 6)//菜单界面生命值框变色
			{
				setfillcolor(0xff0000);
			}
			else
			{
				setfillcolor(0xff);
			}
			//设置生命值框
			fillrectangle(w / 5, h * 3, w / 5 * 2, h * 6);
			wsprintf(str, L" 生命值：(%-2d/10)", hplevel);
			outtextxy(w / 5, h * 3, str);

			wsprintf(str, L"%-3d->%-3d cost:%-3d", hp, hp * 130 / 100, hplevel * hplevel + hplevel * 2);
			outtextxy(w/5, h * 5, str);
		}
		else
		{
			setfillcolor(0xff);
			fillrectangle(w / 5, h * 3, w / 5 * 2, h * 6);
			wsprintf(str, L" 生命值: (10/10)");
			outtextxy(w / 5, h * 3, str);
			wsprintf(str, L"    Max:%d", hp);
			outtextxy(w / 5, h * 5, str);
		}

		if (speedlevel < 6)
		{
			if (m.x >= w / 5 * 3 && m.x <= w / 5 * 4 && m.y >= h * 3 && m.y <= h * 6)
			{
				setfillcolor(0xff0000);
			}
			else
			{
				setfillcolor(0xff);
			}
			fillrectangle(w / 5 * 3, h * 3, w / 5 * 4, h * 6);
			wsprintf(str, L" 速度值：(%-2d/6)", speedlevel);
			outtextxy(w / 5 * 3, h * 3, str);
			wsprintf(str, L"%-3d->%-3d cost:%-3d", speed, speed + 1, speedlevel * speedlevel * speedlevel * 9 / 10 + speedlevel);
			outtextxy(w / 5 * 3, h * 5, str);
		}
		else
		{
			setfillcolor(0xff);
			fillrectangle(w / 5 * 3, h * 3, w / 5 * 4, h * 6);
			wsprintf(str, L" 速度值: (6/6)");
			outtextxy(w / 5 * 3, h * 3, str);
			wsprintf(str, L"    Max:%d", speed);
			outtextxy(w / 5 * 3, h * 5, str);
		}

		//游戏菜单界面的三层边框
		setfillcolor(0xff0000);
		fillrectangle(w / 5 - size / 2, w / 3 - size / 2, area * size / 5 * 4 - size / 2, w / 2 - size / 2);
		setfillcolor(0xff00);
		fillrectangle(w / 5 - size / 4, w / 3 - size / 4, w / 5 * 4 - size / 4, w / 2 - size / 4);
		setfillcolor(0xff);
		fillrectangle(w / 5, w / 3, w / 5 * 4, w / 2);

		setbkmode(TRANSPARENT);
		settextstyle(area * size / 6, area * size / 20, L"楷体");
		settextcolor(DARKGRAY);
		outtextxy(area * size / 3 - size / 8, area * size / 3 - size / 8, L"射击游戏");

		settextcolor(0xf02010);
		outtextxy(area * size / 3, area * size / 3, L"射击游戏");
		settextstyle(h, h / 3, L"楷书");

		if (m.x >= area * size / 2 - 2 * h && m.x <= area * size / 2 + h && m.y >= area * size * 2 / 3 && m.y <= area * size * 2 / 3 + h)
			setfillcolor(YELLOW);
		else
			setfillcolor(0x00ff00);
		fillrectangle(w / 2 - 2 * h, w * 2 / 3, area * size / 2 + h, w * 2 / 3 + h);
		outtextxy(w / 2 - 2 * h, w * 2 / 3, L"开始冒险");
		if (m.x >= area * size / 2 - 2 * h && m.x <= area * size / 2 + 2 * h && m.y >= area * size * 3 / 4 && m.y <= area * size * 3 / 4 + h)
			setfillcolor(YELLOW);
		else
			setfillcolor(0x00ff00);
		fillrectangle(area * size / 2 - 2 * h, w * 3 / 4, area * size / 2 + h, w * 3 / 4 + h);
		outtextxy(area * size / 2 - 2 * h, area * size / 4 * 3, L"结束游戏");
		//以上内容均为菜单栏的初始化

		FlushBatchDraw();

		fflush(stdin);
		FlushMouseMsgBuffer();
		m = GetMouseMsg();

		//一下内容是玩家选择功能
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (hplevel - 10 && money - hplevel * hplevel - hplevel * 2 >= 0 && m.x >= w / 5 && m.x <= w / 5 * 2 && m.y >= h * 3 && m.y <= h * 6)
			{
				hp = hp * 130 / 100;
				money -= hplevel * hplevel + hplevel * 2;
				hplevel++;
			}

			else if (speedlevel - 6 && money - (speedlevel * speedlevel * speedlevel * 9 / 10 + speedlevel) >= 0 && m.x >= w / 5 * 3 && m.x <= w / 5 * 4 && m.y >= h * 3 && m.y <= h * 6)
			{
				speed++;
				money -= speedlevel * speedlevel * speedlevel * 9 / 10 + speedlevel;
				speedlevel++;
			}
				//点击游戏启动区域
			else if (m.x >= area * size / 2 - 2 * h && m.x <= area * size / 2 + 2.5 * h && m.y >= area * size * 2 / 3 && m.y <= area * size * 2 / 3 + h)
			{
				game();
			}
			//点击游戏结束区域
			else if (m.x >= area * size / 2 - 2 * h && m.x <= area * size / 2 + 2 * h && m.y >= area * size * 3 / 4 && m.y <= area * size * 3 / 4 + h)
			{
				break;
			}
		}
	}//end while
	EndBatchDraw();
}



void game()
{
	//初始化信息
	player p(hp, speed);
	std::vector<ai> a;
	std::vector<bullet> b;
	std::deque<death> d;
	int score = 0;
	int cd = 0;
	while (true)
	{
		//清空画布
		setbkcolor(WHITE);
		cleardevice();

		//玩家移动，通过short GetKeyState()的返回值来判断
		if (GetKeyState('W') < 0 && GetKeyState('S') >= 0 && GetKeyState('A') < 0 && GetKeyState('D') >= 0)
		{    //向左上移动
			p.y -= int(round(speed * size / 16 / gen2));
			p.x -= int(round(speed * size / 16 / gen2));
		}
		else if (GetKeyState('W') < 0 && GetKeyState('S') >= 0 && GetKeyState('A') >= 0 && GetKeyState('D') < 0)
		{   //向右上
			p.y -= int(round(speed * size / 16 / gen2));
			p.x += int(round(speed * size / 16 / gen2));
		}
		else if (GetKeyState('W') >= 0 && GetKeyState('S') < 0 && GetKeyState('A') < 0 && GetKeyState('D') >= 0)
		{      //左下
			p.y += int(round(speed * size / 16 / gen2));
			p.x -= int(round(speed * size / 16 / gen2));
		}
		else if (GetKeyState('W') >= 0 && GetKeyState('S') < 0 && GetKeyState('A') >= 0 && GetKeyState('D') < 0)
		{    //右下
			p.y += int(round(speed * size / 16 / gen2));
			p.x += int(round(speed * size / 16 / gen2));
		}
		else if (GetKeyState('W') < 0 && GetKeyState('A') * GetKeyState('D') >= 0 && GetKeyState('S') >= 0)
		{       //向上
			p.y -= speed * size / 16;
		}
		else if (GetKeyState('S') < 0 && GetKeyState('A') * GetKeyState('D') >= 0 && GetKeyState('W') >= 0)
		{       //向下
			p.y += speed * size / 16;
		}
		else if (GetKeyState('A') < 0 && GetKeyState('W') * GetKeyState('S') >= 0 && GetKeyState('D') >= 0)
		{    //向左
			p.x -= speed * size / 16;
		}
		else if (GetKeyState('D') < 0 && GetKeyState('W') * GetKeyState('S') >= 0 && GetKeyState('A') >= 0)
		{     //向右
			p.x += speed * size / 16;
		}

		//修正坐标
		if (p.x < 0)
			p.x = 0;
		else if (p.x > size * area)
			p.x = size * area;
		if (p.y < 0)
			p.y = 0;
		else if (p.y > area * size)
			p.y = area * size;

		//显示玩家
		setfillcolor(p.color);
		fillcircle(p.x, p.y, p.r);

		//随机产生敌人
		if (rand() % (2000 - score) < 150)//随着玩家的得分升高，敌人出现的可能性也会提升
		{
			//控制不同敌人出现的可能性
			int choice = rand() % 10;
			switch (choice)
			{
			case 0:
			case 1:
			case 2:
				a.push_back(ai(rand() % area * size, -size, score / 75 + 1, score / 50 + 4, size / 2, little));//x,y,hp,speed,r,color
				break;
			case 3:
			case 4:
			case 5:
				a.push_back(ai(rand() % area * size, -size, score / 75 + 2, score / 75 + 4, int(round(size / 1.5)), normal));
				break;
			case 6:
				a.push_back(ai(rand() % area * size, size, score / 75 + 3, score / 75 + 3, size, huge));
				break;
			case 7:
				a.push_back(ai(rand() % area * size, size, score / 50 + 4, score / 100 + 3, size, tank));
				break;
			case 8:
			case 9:
				a.push_back(ai(rand() % area * size, size, 1, score / 30 + 4, size / 2, exploder));
				break;
			}
		}

		//玩家攻击
		if (cd == 0)
			if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? 1 : 0)
			{
				if (MouseHit())
					m = GetMouseMsg();
				FlushMouseMsgBuffer();
				double radian = 0;
				if (m.x == p.x)
				{
					if (m.y >= p.y)
					{
						radian = pi / 2;
					}
					else
					{
						radian = -pi / 2;
					}
				}
				else
				{
					radian = atan((1.0 * m.y - 1.0 * p.y) / (1.0 * m.x - 1.0 * p.x));//反正切函数
					if (m.x < p.x)
					{
						radian += pi;
					}
				}
				//根据方向产生新的子弹对象
				bullet tmpb(p.x, p.y, 14+hplevel, int(round(size * cos(radian))), int(round(size * sin(radian))));//time抽象为子弹存在时间
				b.push_back(tmpb);
			}
		cd++;
		if (cd == ::cd)//与全局变量作比较
			cd = 0;

		//逐个判断子弹是否到最大距离或是否击中敌人
		for (int i = 0; i < int(b.size()); i++)
		{
			b[i].x += b[i].dx;//dx，dy代表该方向上的移动距离 x:0,1，-1，y：0,1,-1
			b[i].y += b[i].dy;
			b[i].time--;//每移动一次存在时间减少
			if (b[i].time <= 0)
			{
				b.erase(b.begin() + i);
				i--;//子弹消失
				continue;
			}
			for (int j = 0; j < int(a.size()); j++)
			{
				int dx = b[i].x - a[j].x;//计算子弹与敌人之间的距离
				int dy = b[i].y - a[j].y;
				int dis = size/8  + a[j].r;//规定攻击有效范围
				if (dx * dx + dy * dy <= dis * dis)
				{
					a[j].hp--;
					if (a[j].hp <= 0)
					{
						death tmp(a[j].x, a[j].y, a[j].r, a[j].color);
						d.push_back(tmp);//加入死亡队列
						a.erase(a.begin() + j);//移除敌人
						score++;
					}
					b.erase(b.begin() + i);//移除子弹
					i--;
					break;
				}
			}
		}

		//判断敌人是否伤害到玩家并移动敌人的位置
		for (int i = 0; i < int(a.size()); i++) // a是ai的对象，保存ai的队列
		{
			int dx = a[i].x - p.x;      //计算敌人和玩家之间的距离
			int dy = a[i].y - p.y;
			int dis = a[i].r + p.r;     //还要加上自身的半径
			if (dx * dx + dy * dy < dis * dis)  //如果敌人和玩家之间的距离小于他们之间的半径和，表示两者想接触
			{
				p.hp -= a[i].hp * a[i].speed;      //玩家的生命值减去ai的生命值加自己的速度
				death tmp(a[i].x, a[i].y, a[i].r, a[i].color); //敌人死亡，调用死亡函数
				d.push_back(tmp);  //缓存死亡，压入队列
				a.erase(a.begin() + i); //销毁对应的ai
				i--; //vector的大小改变，修正i的值
				continue;
			}

			//计算敌人的与玩家的相对位置，用于追击玩家，转移角度
			double radian = 0;
			if (a[i].x == p.x)
			{
				if (a[i].x > p.x)
				{
					radian = -pi / 2;
				}
				else
				{
					radian = pi / 2;
				}
			}
			else
			{
				radian = atan((1.0 * a[i].y - 1.0 * p.y) / (1.0 * a[i].x - 1.0 * p.x));  
				if (p.x < a[i].x)
				{
					radian += pi;
				}
			}
			a[i].x += int(round(a[i].speed * cos(radian)));
			a[i].y += int(round(a[i].speed * sin(radian)));
		}

		//死亡动画
		for (int i = 0; i < int(d.size()); i++)
		{
			d[i].time--;
			if (d[i].time == 0)
			{
				d.pop_front();
				i--;
			}
			else
			{
				setfillcolor(d[i].color);
				fillcircle(d[i].x, d[i].y, d[i].r * d[i].time / 10);
			}
		}

		//画出每一个子弹
		setfillcolor(BLACK);
		for (int i = 0; i < int(b.size()); i++)
		{
			fillcircle(b[i].x, b[i].y, size / 8);
		}

		//画出每一个敌人
		for (int i = 0; i < int(a.size()); i++)
		{
			setfillcolor(a[i].color);
			fillcircle(a[i].x, a[i].y, a[i].r);
		}

		//实时显示情况
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		wsprintf(str, L"生命值：%d", p.hp);
		outtextxy(0, 0, str);
		wsprintf(str, L"得分：%d", score);
		outtextxy(0, int(round(size * 1.5)), str);
		setbkmode(OPAQUE);

		FlushBatchDraw();
		FlushMouseMsgBuffer();
		fflush(stdin);

		Sleep(50);

		//如果玩家生命值不足，将结束游戏
		if (p.hp <= 0)
			break;
	}//end while

	setlinecolor(WHITE);
	for (int i = 0; i < area * size; i++)
	{
		line(0, i, area * size, i);
		Sleep(1);
		FlushBatchDraw();
	}
	setlinecolor(BLACK);
	//游戏结算
	money += score;
}