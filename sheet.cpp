#include<iostream>
#include<graphics.h>
#include<conio.h> //���ж�����ͨ������̨���������������������ĺ�������Ҫ��һЩ�û�ͨ�������̲����Ķ�Ӧ����������getch()�����ȵȡ�
#include<mmsystem.h>//���ý���йصĽӿ�
#include<stdlib.h>
#include<vector>
#include<deque>
#include<ctime>
#include<math.h>
#pragma comment(lib,"winmm.lib")

//�õ��ĳ���
const double pi = acos(-1); //�����Һ���
const double gen2 = sqrt(2);

//��Ϸ������Ϣ
constexpr int area = 40;
constexpr int size = 16; //�������ʽconstexpr��ֵָ����ı����ڱ�������о��ܹ��õ��������ı��ʽ�����ڱ���ʱ��ֵ�ı��ʽ



MOUSEMSG m; //���������Ϣ

//outtextxy����ַ���
wchar_t str[128]; //���ַ�16λ

//��¼�ʽ�
int money = 0;

//��¼����
int hp = 12;
int hplevel = 1; //
int speed = 3;
int speedlevel = 1;
int cd = 3; //��ȴʱ��
//������̫�Ѽ��㣬���Է��ڿ�ѡ��λ����
//int attack = 2;  
//int attacklevel = 1;

COLORREF     little = GREEN, normal = YELLOW, huge = MAGENTA, tank = BROWN, exploder = RED;//������Ϣ
/*  ����
  �ƶ��ٶ�		  ��		     �� 			 ��		        ��			  ��
   ����ֵ		  ��			 ��				 ��			    ��			  ��        */

   //�������ԵĻ���
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

//�����
class player :public basic
{
public:
	player(int h, int s)
	{
		hp = h;
		speed = s;
		color = BLUE;
		r = int(round(size / 1.5)); //�������롣�������뾶
		x = area * size / 2;
		y = area * size / 2;
	}
};

//������
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

//�ӵ���
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

//�����б�
void init();
void show();
void game();

int main()
{
	srand(int(time(0)));
	initgraph(size * area, size * area);
	BeginBatchDraw();//����˫����

	init();
	show();
	closegraph();
	return 0;
}

void init()
{
	setbkcolor(WHITE);//��ʼ���汳����ɫ
	cleardevice();//����
	setlinecolor(BLACK);//����������ɫΪ��ɫ

	setfillcolor(0xff0000);//area=40,size=16�����������ɫ ��������������Ϊ�����������Ƕ�ױ߿�
	fillrectangle(area * size / 5 - size / 2, area * size / 6 - size / 2, area * size / 5 * 4 - size / 2, area * size / 6 * 2 - size / 2);
	
	setfillcolor(0xff00);
	fillrectangle(area * size / 5 - size / 4, area * size / 6 - size / 4, area * size / 5 * 4 - size / 4, area * size / 6 * 2 - size / 4);
	
	setfillcolor(0xff);
	fillrectangle(area * size / 5, area * size / 6, area * size / 5 * 4, area * size / 6 * 2);

	setbkmode(TRANSPARENT);//���ñ���ģʽΪ͸��
	settextstyle(area * size / 6, area * size / 20, L"��������");//���õ�ǰ������ʽ
	outtextxy(area * size / 4 - size / 8, area * size / 6 - size / 8, L"�����Ϸ");//��ָ��λ������ַ���
	settextcolor(0xf02010); //����������ɫ,�����Ϸ����ɫ
	outtextxy(area * size / 4, area * size / 6, L"�����Ϸ");

	setfillcolor(YELLOW);//Programming For VS2019 And EasyX
	settextstyle(size * 3, size * 3 / 2, L"��������");
	fillrectangle(area * size / 5, area * size / 6 * 5 - 2 * size, area * size / 5 * 4, area * size / 6 * 5 + 2 * size);
	outtextxy(area * size / 5 + size / 2, area * size / 6 * 5 - 2 * size, L"�����������");

	settextstyle(size, size / 4, L"��������");
	settextcolor(0xaa00ff);
	outtextxy(area * size / 5 * 2, area * size / 10 * 9, L"");

	settextcolor(0xeeaa33);
	LOGFONT f; //LOGFONT��Windows�ڲ�������߼��ṹ����Ҫ�������������ʽ���䶨�����£�
	gettextstyle(&f);
	f.lfEscapement = 150; //���������ʽ
	f.lfOrientation = 150;
	settextstyle(&f);
	outtextxy(area * size / 3 * 2, area * size / 3 + size * 4, L"Version 0.1.2 By��ZZC1024");

	f.lfEscapement = 0;
	f.lfOrientation = 0;
	settextstyle(&f);//�������ʽ��ΪĬ��ֵ

	FlushBatchDraw();
	system("pause");
	setlinecolor(WHITE);
	for (int i = 0; i < area * size; i++)
	{
		line(0, i, area * size, i);//�л�����ˢ������
		Sleep(1);
		FlushBatchDraw();
	}
	setlinecolor(BLACK);
}

void show()
{
	while (true)
	{
		//��ʼ���˵�
		constexpr int w = area * size;
		constexpr int h = w /30;
		settextstyle(h, h / 3, L"����");
		setbkcolor(WHITE);
		cleardevice();

		//��ʾ���ӵ�е��ʽ�
		setbkmode(OPAQUE);
		setbkcolor(YELLOW);
		settextcolor(BLACK);
		wsprintf(str, L"��Ǯ��%d", money);
		outtextxy(h * 6, h, str);
		setbkmode(TRANSPARENT);

		//������Կ���
		if (hplevel < 10)
		{
			if (m.x >= w / 5 && m.x <= w / 5 * 2 && m.y >= h * 3 && m.y <= h * 6)//�˵���������ֵ���ɫ
			{
				setfillcolor(0xff0000);
			}
			else
			{
				setfillcolor(0xff);
			}
			//��������ֵ��
			fillrectangle(w / 5, h * 3, w / 5 * 2, h * 6);
			wsprintf(str, L" ����ֵ��(%-2d/10)", hplevel);
			outtextxy(w / 5, h * 3, str);

			wsprintf(str, L"%-3d->%-3d cost:%-3d", hp, hp * 130 / 100, hplevel * hplevel + hplevel * 2);
			outtextxy(w/5, h * 5, str);
		}
		else
		{
			setfillcolor(0xff);
			fillrectangle(w / 5, h * 3, w / 5 * 2, h * 6);
			wsprintf(str, L" ����ֵ: (10/10)");
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
			wsprintf(str, L" �ٶ�ֵ��(%-2d/6)", speedlevel);
			outtextxy(w / 5 * 3, h * 3, str);
			wsprintf(str, L"%-3d->%-3d cost:%-3d", speed, speed + 1, speedlevel * speedlevel * speedlevel * 9 / 10 + speedlevel);
			outtextxy(w / 5 * 3, h * 5, str);
		}
		else
		{
			setfillcolor(0xff);
			fillrectangle(w / 5 * 3, h * 3, w / 5 * 4, h * 6);
			wsprintf(str, L" �ٶ�ֵ: (6/6)");
			outtextxy(w / 5 * 3, h * 3, str);
			wsprintf(str, L"    Max:%d", speed);
			outtextxy(w / 5 * 3, h * 5, str);
		}

		//��Ϸ�˵����������߿�
		setfillcolor(0xff0000);
		fillrectangle(w / 5 - size / 2, w / 3 - size / 2, area * size / 5 * 4 - size / 2, w / 2 - size / 2);
		setfillcolor(0xff00);
		fillrectangle(w / 5 - size / 4, w / 3 - size / 4, w / 5 * 4 - size / 4, w / 2 - size / 4);
		setfillcolor(0xff);
		fillrectangle(w / 5, w / 3, w / 5 * 4, w / 2);

		setbkmode(TRANSPARENT);
		settextstyle(area * size / 6, area * size / 20, L"����");
		settextcolor(DARKGRAY);
		outtextxy(area * size / 3 - size / 8, area * size / 3 - size / 8, L"�����Ϸ");

		settextcolor(0xf02010);
		outtextxy(area * size / 3, area * size / 3, L"�����Ϸ");
		settextstyle(h, h / 3, L"����");

		if (m.x >= area * size / 2 - 2 * h && m.x <= area * size / 2 + h && m.y >= area * size * 2 / 3 && m.y <= area * size * 2 / 3 + h)
			setfillcolor(YELLOW);
		else
			setfillcolor(0x00ff00);
		fillrectangle(w / 2 - 2 * h, w * 2 / 3, area * size / 2 + h, w * 2 / 3 + h);
		outtextxy(w / 2 - 2 * h, w * 2 / 3, L"��ʼð��");
		if (m.x >= area * size / 2 - 2 * h && m.x <= area * size / 2 + 2 * h && m.y >= area * size * 3 / 4 && m.y <= area * size * 3 / 4 + h)
			setfillcolor(YELLOW);
		else
			setfillcolor(0x00ff00);
		fillrectangle(area * size / 2 - 2 * h, w * 3 / 4, area * size / 2 + h, w * 3 / 4 + h);
		outtextxy(area * size / 2 - 2 * h, area * size / 4 * 3, L"������Ϸ");
		//�������ݾ�Ϊ�˵����ĳ�ʼ��

		FlushBatchDraw();

		fflush(stdin);
		FlushMouseMsgBuffer();
		m = GetMouseMsg();

		//һ�����������ѡ����
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
				//�����Ϸ��������
			else if (m.x >= area * size / 2 - 2 * h && m.x <= area * size / 2 + 2.5 * h && m.y >= area * size * 2 / 3 && m.y <= area * size * 2 / 3 + h)
			{
				game();
			}
			//�����Ϸ��������
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
	//��ʼ����Ϣ
	player p(hp, speed);
	std::vector<ai> a;
	std::vector<bullet> b;
	std::deque<death> d;
	int score = 0;
	int cd = 0;
	while (true)
	{
		//��ջ���
		setbkcolor(WHITE);
		cleardevice();

		//����ƶ���ͨ��short GetKeyState()�ķ���ֵ���ж�
		if (GetKeyState('W') < 0 && GetKeyState('S') >= 0 && GetKeyState('A') < 0 && GetKeyState('D') >= 0)
		{    //�������ƶ�
			p.y -= int(round(speed * size / 16 / gen2));
			p.x -= int(round(speed * size / 16 / gen2));
		}
		else if (GetKeyState('W') < 0 && GetKeyState('S') >= 0 && GetKeyState('A') >= 0 && GetKeyState('D') < 0)
		{   //������
			p.y -= int(round(speed * size / 16 / gen2));
			p.x += int(round(speed * size / 16 / gen2));
		}
		else if (GetKeyState('W') >= 0 && GetKeyState('S') < 0 && GetKeyState('A') < 0 && GetKeyState('D') >= 0)
		{      //����
			p.y += int(round(speed * size / 16 / gen2));
			p.x -= int(round(speed * size / 16 / gen2));
		}
		else if (GetKeyState('W') >= 0 && GetKeyState('S') < 0 && GetKeyState('A') >= 0 && GetKeyState('D') < 0)
		{    //����
			p.y += int(round(speed * size / 16 / gen2));
			p.x += int(round(speed * size / 16 / gen2));
		}
		else if (GetKeyState('W') < 0 && GetKeyState('A') * GetKeyState('D') >= 0 && GetKeyState('S') >= 0)
		{       //����
			p.y -= speed * size / 16;
		}
		else if (GetKeyState('S') < 0 && GetKeyState('A') * GetKeyState('D') >= 0 && GetKeyState('W') >= 0)
		{       //����
			p.y += speed * size / 16;
		}
		else if (GetKeyState('A') < 0 && GetKeyState('W') * GetKeyState('S') >= 0 && GetKeyState('D') >= 0)
		{    //����
			p.x -= speed * size / 16;
		}
		else if (GetKeyState('D') < 0 && GetKeyState('W') * GetKeyState('S') >= 0 && GetKeyState('A') >= 0)
		{     //����
			p.x += speed * size / 16;
		}

		//��������
		if (p.x < 0)
			p.x = 0;
		else if (p.x > size * area)
			p.x = size * area;
		if (p.y < 0)
			p.y = 0;
		else if (p.y > area * size)
			p.y = area * size;

		//��ʾ���
		setfillcolor(p.color);
		fillcircle(p.x, p.y, p.r);

		//�����������
		if (rand() % (2000 - score) < 150)//������ҵĵ÷����ߣ����˳��ֵĿ�����Ҳ������
		{
			//���Ʋ�ͬ���˳��ֵĿ�����
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

		//��ҹ���
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
					radian = atan((1.0 * m.y - 1.0 * p.y) / (1.0 * m.x - 1.0 * p.x));//�����к���
					if (m.x < p.x)
					{
						radian += pi;
					}
				}
				//���ݷ�������µ��ӵ�����
				bullet tmpb(p.x, p.y, 14+hplevel, int(round(size * cos(radian))), int(round(size * sin(radian))));//time����Ϊ�ӵ�����ʱ��
				b.push_back(tmpb);
			}
		cd++;
		if (cd == ::cd)//��ȫ�ֱ������Ƚ�
			cd = 0;

		//����ж��ӵ��Ƿ���������Ƿ���е���
		for (int i = 0; i < int(b.size()); i++)
		{
			b[i].x += b[i].dx;//dx��dy����÷����ϵ��ƶ����� x:0,1��-1��y��0,1,-1
			b[i].y += b[i].dy;
			b[i].time--;//ÿ�ƶ�һ�δ���ʱ�����
			if (b[i].time <= 0)
			{
				b.erase(b.begin() + i);
				i--;//�ӵ���ʧ
				continue;
			}
			for (int j = 0; j < int(a.size()); j++)
			{
				int dx = b[i].x - a[j].x;//�����ӵ������֮��ľ���
				int dy = b[i].y - a[j].y;
				int dis = size/8  + a[j].r;//�涨������Ч��Χ
				if (dx * dx + dy * dy <= dis * dis)
				{
					a[j].hp--;
					if (a[j].hp <= 0)
					{
						death tmp(a[j].x, a[j].y, a[j].r, a[j].color);
						d.push_back(tmp);//������������
						a.erase(a.begin() + j);//�Ƴ�����
						score++;
					}
					b.erase(b.begin() + i);//�Ƴ��ӵ�
					i--;
					break;
				}
			}
		}

		//�жϵ����Ƿ��˺�����Ҳ��ƶ����˵�λ��
		for (int i = 0; i < int(a.size()); i++) // a��ai�Ķ��󣬱���ai�Ķ���
		{
			int dx = a[i].x - p.x;      //������˺����֮��ľ���
			int dy = a[i].y - p.y;
			int dis = a[i].r + p.r;     //��Ҫ��������İ뾶
			if (dx * dx + dy * dy < dis * dis)  //������˺����֮��ľ���С������֮��İ뾶�ͣ���ʾ������Ӵ�
			{
				p.hp -= a[i].hp * a[i].speed;      //��ҵ�����ֵ��ȥai������ֵ���Լ����ٶ�
				death tmp(a[i].x, a[i].y, a[i].r, a[i].color); //����������������������
				d.push_back(tmp);  //����������ѹ�����
				a.erase(a.begin() + i); //���ٶ�Ӧ��ai
				i--; //vector�Ĵ�С�ı䣬����i��ֵ
				continue;
			}

			//������˵�����ҵ����λ�ã�����׷����ң�ת�ƽǶ�
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

		//��������
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

		//����ÿһ���ӵ�
		setfillcolor(BLACK);
		for (int i = 0; i < int(b.size()); i++)
		{
			fillcircle(b[i].x, b[i].y, size / 8);
		}

		//����ÿһ������
		for (int i = 0; i < int(a.size()); i++)
		{
			setfillcolor(a[i].color);
			fillcircle(a[i].x, a[i].y, a[i].r);
		}

		//ʵʱ��ʾ���
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		wsprintf(str, L"����ֵ��%d", p.hp);
		outtextxy(0, 0, str);
		wsprintf(str, L"�÷֣�%d", score);
		outtextxy(0, int(round(size * 1.5)), str);
		setbkmode(OPAQUE);

		FlushBatchDraw();
		FlushMouseMsgBuffer();
		fflush(stdin);

		Sleep(50);

		//����������ֵ���㣬��������Ϸ
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
	//��Ϸ����
	money += score;
}