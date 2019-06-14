#include <iostream>
#include <graphics.h>
#include <ctime>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#define BNUM 70 //磁盘所拥有的磁道数 
#define BLENGTH 30   //请求序列长度 
#define PACE 20 //两个磁道之间的距离 
using namespace std;

ofstream fout("log_Hardrive.txt");

class sequence
{
	private:
		int seq[BLENGTH];
	public:
		int *g()
		{
			return seq;
		}
		int g(int i)
		{
			return seq[i];
		}
		sequence()
		{
			srand(int(time(0)));//get the seed ready
			for(int i = 0;i < BLENGTH;i++)
			{
				seq[i]=rand() % BNUM;
			}
		}
};

class SCAN
{
	private:
		int now;//目前磁头所在磁道位置 
		int close;//下一个要访问的磁道 
		int x_n;//目前绘图位置x坐标 
		int x_c;//下一个点的x坐标 
		int y;//y坐标 
		int log[BNUM];//在本序列中要被访问多少次  
		int seq[BLENGTH];//磁头访问顺序 
		int cnt;//磁头总移动距离 
		int dir;//磁头目前移动方向，0：向右 1：向左 
	public:
		void draw()//画磁头运行轨迹 
		{
			x_n = now * PACE + PACE;
			x_c = close * PACE + PACE;
			setfillcolor(LIGHTGRAY);
			fillellipse(x_n,y+2,3,3);
			setlinewidth(10);
			line(x_n,y,x_c,y + PACE);
			fillellipse(x_c,y + PACE+2,3,3);
			y += PACE;
			delay_ms(100);//延迟 
		}
		void search()//搜索并设定下一个要访问的磁道（找出距磁头最近并且在访问队列中存在的磁道） 
		{
			for(int i = 0 ; i < BNUM; i++)//顺序扫描离磁头从近到远的磁道是否要被访问 
			{
				if((now + i) == BNUM  - 1 && log[now + i] == 0 && dir == 0)//if 检测到最后一个磁道 && 最后一个磁道都没有访问记录 && 磁头向右移动 
				{
					dir = 1;//改变磁头移动方向 
				} 
				if((now - i) == 0 && log[now - i] == 0 && dir == 1)//if 检测到第一个磁道 && 第一个磁道都没有访问记录 && 磁头向左移动 
				{
					dir = 0;//改变磁头移动方向 
				}
				if(log[now + i] != 0 && (now + i) < BNUM && dir == 0)//如果磁头之后 i 个磁道要被访问 && 没有向右越界 && 磁头移动方向是向右，记录磁道位置并且结束循环 
				{
					close = now + i;
					log[close]--;//减去一次访问次数（因为访问即将要被处理)
					cnt += i;//累加磁头移动距离
					break;
				} 
				if(log[now - i] != 0 && (now - i) >= 0 && dir == 1)//如果磁头之前 i 个磁道~~~~~~~~ && 没有向左越界 && 磁头移动方向是向左，记录~~~~~~~~~~ 
				{
					close = now - i;
					log[close]--;//减去一次访问次数（因为访问即将要被处理)
					cnt += i;//累加磁头移动距离 
					break;
				}
			}
		}
		void print(int i)
		{
			fout<<setw(8)<<log[0];
			for(int j=1;j < BNUM;j++)
			{
				fout<<setw(11)<<log[j];
			}
			fout<<setw(16)<<now;fout<<setw(12)<<close;
			fout<<endl;
		}
		SCAN(sequence s,int init)
		{
			dir = 0;//初始时假设磁头向右移动 
			now = init;
			//now = rand() % BNUM;//随机指派磁头位置 
			cout<<"head position: "<<now<<endl; 
			fout<<"head position: "<<now<<endl;
			fout<<setw(10)<<"wanted_block seq:";
			cout<<setw(10)<<"wanted_block seq:";
			for(int i = 0; i < BLENGTH ; i++)
			{
				fout<<s.g(i)<<" ";
				cout<<s.g(i)<<" ";
			}
			fout<<endl;
			cout<<endl;
			for(int j=0;j < BNUM;j++)
			{
				if( j < 9 )
				{
					fout<<setw(10)<<"block#"<<j;
				} 
				else
				{
					fout<<setw(9)<<"block#"<<j;
				}
			}
			fout<<setw(17)<<"current_block"<<setw(15)<<"nearest_block\n";
			for(int i = 0 ; i < BNUM ; i++)//init the log
			{
				log[i] = 0;
			}
			for(int i = 0;i < BLENGTH - 1;i++)
			{
				seq[i] = 0;
			}
			x_c = 0;
			x_n = 0;
			close = 0;
			cnt = 0;
			y = PACE;//init the var
			for(int i = 0 ; i < BLENGTH ; i++)//计算每个磁道在这个序列中总共要被访问多少次 
			{
				log[s.g(i)]++;
			} 
			fout<<setw(8)<<log[0];//显示总的访问情况 
			for(int j=1;j < BNUM;j++)
			{
				fout<<setw(11)<<log[j];
			}
			fout<<endl;
			for(int i = 0 ;i < BLENGTH ; i++)//start the SSTF
			{
				if(i == 0)//如果是第一次运行，查找最近的请求磁道并且设定磁头运动方向 
				{
					for(int i = 0 ; i < BNUM; i++)//顺序扫描离磁头从近到远的磁道是否要被访问 
			        {
				        if(log[now + i] != 0 && (now + i) < BNUM)//如果磁头之后 i 个磁道要被访问，记录磁道位置并且结束循环 
				        {
				        	dir = 0;
					        close = now + i;
					        log[close]--;//减去一次访问次数（因为访问即将要被处理)
					        cnt += i;//累加磁头移动距离
					        break;
				        } 
				        if(log[now - i] != 0 && (now - i) >= 0)//如果磁头之前 i 个磁道~~~~~~~~，记录~~~~~~~~~~ 
				        {
				        	dir = 1;
					        close = now - i;
					        log[close]--;//减去一次访问次数（因为访问即将要被处理)
					        cnt += i;//累加磁头移动距离 
					        break;
				        }
			        }
				}
				else
				{
					search();//search for the next track
				}
				print(i);
				draw();
				now = close;
				seq[i] = close;
			}
			fout<<"access sequence:";
			cout<<"access sequence:";
			for(int i = 0 ; i < BLENGTH; i++)
			{
				fout<<seq[i]<<" ";
				cout<<seq[i]<<" ";
			}
			fout<<endl;
			cout<<endl;
			fout<<"total movement length = "<<cnt<<endl;
			cout<<"total movement length = "<<cnt<<endl;
		}
};

class SSTF
{
	private:
		int now;//目前磁头所在磁道位置 
		int close;//下一个要访问的磁道 
		int x_n;//目前绘图位置x坐标 
		int x_c;//下一个点的x坐标 
		int y;//y坐标 
		int log[BNUM];//在本序列中要被访问多少次  
		int seq[BLENGTH];//磁头访问顺序 
		int cnt;//磁头总移动距离 
		int init;//磁头开始位置 
	public:
		void draw()//绘制磁头运行轨迹 
		{
			x_n = now * PACE + PACE;
			x_c = close * PACE + PACE;
			setfillcolor(LIGHTGRAY);
			fillellipse(x_n,y+2,3,3);
			setlinewidth(10);
			line(x_n,y,x_c,y + PACE);
			fillellipse(x_c,y + PACE+2,3,3);
			y += PACE;
			delay_ms(100);//延迟 
		}
		int g_init()//取得磁头开始位置的函数
		{
			return init;
		} 
		void search()//搜索并设定下一个要访问的磁道（找出距磁头最近并且在访问队列中存在的磁道） 
		{
			for(int i = 0 ; i < BNUM; i++)//顺序扫描离磁头从近到远的磁道是否要被访问 
			{
				if(log[now + i] != 0 && (now + i) < BNUM)//如果磁头之后 i 个磁道要被访问，记录磁道位置并且结束循环 
				{
					close = now + i;
					log[close]--;//减去一次访问次数（因为访问即将要被处理)
					cnt += i;//累加磁头移动距离
					break;
				} 
				if(log[now - i] != 0 && (now - i) >= 0)//如果磁头之前 i 个磁道~~~~~~~~，记录~~~~~~~~~~ 
				{
					close = now - i;
					log[close]--;//减去一次访问次数（因为访问即将要被处理)
					cnt += i;//累加磁头移动距离 
					break;
				}
			}
		}
		void print(int i)
		{
			fout<<setw(8)<<log[0];
			for(int j=1;j < BNUM;j++)
			{
				fout<<setw(11)<<log[j];
			}
			fout<<setw(16)<<now;fout<<setw(12)<<close;
			fout<<endl;
		}
		SSTF(sequence s)
		{
			now = rand() % BNUM;//随机指派磁头位置 
			init = now;
			cout<<"head position: "<<now<<endl; 
			fout<<"head position: "<<now<<endl;
			fout<<setw(10)<<"wanted_block seq:";
			cout<<setw(10)<<"wanted_block seq:";
			for(int i = 0; i < BLENGTH ; i++)
			{
				fout<<s.g(i)<<" ";
				cout<<s.g(i)<<" ";
			}
			fout<<endl;
			cout<<endl;
			for(int j=0;j < BNUM;j++)
			{
				if( j < 9 )
				{
					fout<<setw(10)<<"block#"<<j;
				} 
				else
				{
					fout<<setw(9)<<"block#"<<j;
				}
			}
			fout<<setw(17)<<"current_block"<<setw(15)<<"nearest_block\n";
			for(int i = 0 ; i < BNUM ; i++)//init the log
			{
				log[i] = 0;
			}
			for(int i = 0;i < BLENGTH - 1;i++)
			{
				seq[i] = 0;
			}
			x_c = 0;
			x_n = 0;
			close = 0;
			cnt = 0;
			y = PACE;//init the var
			for(int i = 0 ; i < BLENGTH ; i++)//计算每个磁道在这个序列中总共要被访问多少次 
			{
				log[s.g(i)]++;
			} 
			fout<<setw(8)<<log[0];//显示总的访问情况 
			for(int j=1;j < BNUM;j++)
			{
				fout<<setw(11)<<log[j];
			}
			fout<<endl;
			for(int i = 0 ;i < BLENGTH ; i++)//start the SSTF
			{
				search();//search for the next block
				print(i);
				draw();
				now = close;
				seq[i] = close;
			}
			fout<<"access sequence:";
			cout<<"access sequence:";
			for(int i = 0 ; i < BLENGTH; i++)
			{
				fout<<seq[i]<<" ";
				cout<<seq[i]<<" ";
			}
			fout<<endl;
			cout<<endl;
			fout<<"total movement length = "<<cnt<<endl;
			cout<<"total movement length = "<<cnt<<endl;
		}
};

int main()
{
	sequence s;
	cout<<"SSTF:\n";
	fout<<"SSTF:\n";
	initgraph(BNUM * PACE + PACE * 2, BLENGTH * PACE + PACE * 2);
	SSTF s_1(s);
	getch();
	closegraph();
    cout<<"SCAN:\n";
	fout<<"SCAN:\n";
    initgraph(BNUM * PACE + PACE * 2, BLENGTH * PACE + PACE * 2);
    SCAN s_2(s,s_1.g_init());
    getch();
    closegraph();
}

