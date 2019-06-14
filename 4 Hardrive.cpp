#include <iostream>
#include <graphics.h>
#include <ctime>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#define BNUM 70 //������ӵ�еĴŵ��� 
#define BLENGTH 30   //�������г��� 
#define PACE 20 //�����ŵ�֮��ľ��� 
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
		int now;//Ŀǰ��ͷ���ڴŵ�λ�� 
		int close;//��һ��Ҫ���ʵĴŵ� 
		int x_n;//Ŀǰ��ͼλ��x���� 
		int x_c;//��һ�����x���� 
		int y;//y���� 
		int log[BNUM];//�ڱ�������Ҫ�����ʶ��ٴ�  
		int seq[BLENGTH];//��ͷ����˳�� 
		int cnt;//��ͷ���ƶ����� 
		int dir;//��ͷĿǰ�ƶ�����0������ 1������ 
	public:
		void draw()//����ͷ���й켣 
		{
			x_n = now * PACE + PACE;
			x_c = close * PACE + PACE;
			setfillcolor(LIGHTGRAY);
			fillellipse(x_n,y+2,3,3);
			setlinewidth(10);
			line(x_n,y,x_c,y + PACE);
			fillellipse(x_c,y + PACE+2,3,3);
			y += PACE;
			delay_ms(100);//�ӳ� 
		}
		void search()//�������趨��һ��Ҫ���ʵĴŵ����ҳ����ͷ��������ڷ��ʶ����д��ڵĴŵ��� 
		{
			for(int i = 0 ; i < BNUM; i++)//˳��ɨ�����ͷ�ӽ���Զ�Ĵŵ��Ƿ�Ҫ������ 
			{
				if((now + i) == BNUM  - 1 && log[now + i] == 0 && dir == 0)//if ��⵽���һ���ŵ� && ���һ���ŵ���û�з��ʼ�¼ && ��ͷ�����ƶ� 
				{
					dir = 1;//�ı��ͷ�ƶ����� 
				} 
				if((now - i) == 0 && log[now - i] == 0 && dir == 1)//if ��⵽��һ���ŵ� && ��һ���ŵ���û�з��ʼ�¼ && ��ͷ�����ƶ� 
				{
					dir = 0;//�ı��ͷ�ƶ����� 
				}
				if(log[now + i] != 0 && (now + i) < BNUM && dir == 0)//�����ͷ֮�� i ���ŵ�Ҫ������ && û������Խ�� && ��ͷ�ƶ����������ң���¼�ŵ�λ�ò��ҽ���ѭ�� 
				{
					close = now + i;
					log[close]--;//��ȥһ�η��ʴ�������Ϊ���ʼ���Ҫ������)
					cnt += i;//�ۼӴ�ͷ�ƶ�����
					break;
				} 
				if(log[now - i] != 0 && (now - i) >= 0 && dir == 1)//�����ͷ֮ǰ i ���ŵ�~~~~~~~~ && û������Խ�� && ��ͷ�ƶ����������󣬼�¼~~~~~~~~~~ 
				{
					close = now - i;
					log[close]--;//��ȥһ�η��ʴ�������Ϊ���ʼ���Ҫ������)
					cnt += i;//�ۼӴ�ͷ�ƶ����� 
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
			dir = 0;//��ʼʱ�����ͷ�����ƶ� 
			now = init;
			//now = rand() % BNUM;//���ָ�ɴ�ͷλ�� 
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
			for(int i = 0 ; i < BLENGTH ; i++)//����ÿ���ŵ�������������ܹ�Ҫ�����ʶ��ٴ� 
			{
				log[s.g(i)]++;
			} 
			fout<<setw(8)<<log[0];//��ʾ�ܵķ������ 
			for(int j=1;j < BNUM;j++)
			{
				fout<<setw(11)<<log[j];
			}
			fout<<endl;
			for(int i = 0 ;i < BLENGTH ; i++)//start the SSTF
			{
				if(i == 0)//����ǵ�һ�����У��������������ŵ������趨��ͷ�˶����� 
				{
					for(int i = 0 ; i < BNUM; i++)//˳��ɨ�����ͷ�ӽ���Զ�Ĵŵ��Ƿ�Ҫ������ 
			        {
				        if(log[now + i] != 0 && (now + i) < BNUM)//�����ͷ֮�� i ���ŵ�Ҫ�����ʣ���¼�ŵ�λ�ò��ҽ���ѭ�� 
				        {
				        	dir = 0;
					        close = now + i;
					        log[close]--;//��ȥһ�η��ʴ�������Ϊ���ʼ���Ҫ������)
					        cnt += i;//�ۼӴ�ͷ�ƶ�����
					        break;
				        } 
				        if(log[now - i] != 0 && (now - i) >= 0)//�����ͷ֮ǰ i ���ŵ�~~~~~~~~����¼~~~~~~~~~~ 
				        {
				        	dir = 1;
					        close = now - i;
					        log[close]--;//��ȥһ�η��ʴ�������Ϊ���ʼ���Ҫ������)
					        cnt += i;//�ۼӴ�ͷ�ƶ����� 
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
		int now;//Ŀǰ��ͷ���ڴŵ�λ�� 
		int close;//��һ��Ҫ���ʵĴŵ� 
		int x_n;//Ŀǰ��ͼλ��x���� 
		int x_c;//��һ�����x���� 
		int y;//y���� 
		int log[BNUM];//�ڱ�������Ҫ�����ʶ��ٴ�  
		int seq[BLENGTH];//��ͷ����˳�� 
		int cnt;//��ͷ���ƶ����� 
		int init;//��ͷ��ʼλ�� 
	public:
		void draw()//���ƴ�ͷ���й켣 
		{
			x_n = now * PACE + PACE;
			x_c = close * PACE + PACE;
			setfillcolor(LIGHTGRAY);
			fillellipse(x_n,y+2,3,3);
			setlinewidth(10);
			line(x_n,y,x_c,y + PACE);
			fillellipse(x_c,y + PACE+2,3,3);
			y += PACE;
			delay_ms(100);//�ӳ� 
		}
		int g_init()//ȡ�ô�ͷ��ʼλ�õĺ���
		{
			return init;
		} 
		void search()//�������趨��һ��Ҫ���ʵĴŵ����ҳ����ͷ��������ڷ��ʶ����д��ڵĴŵ��� 
		{
			for(int i = 0 ; i < BNUM; i++)//˳��ɨ�����ͷ�ӽ���Զ�Ĵŵ��Ƿ�Ҫ������ 
			{
				if(log[now + i] != 0 && (now + i) < BNUM)//�����ͷ֮�� i ���ŵ�Ҫ�����ʣ���¼�ŵ�λ�ò��ҽ���ѭ�� 
				{
					close = now + i;
					log[close]--;//��ȥһ�η��ʴ�������Ϊ���ʼ���Ҫ������)
					cnt += i;//�ۼӴ�ͷ�ƶ�����
					break;
				} 
				if(log[now - i] != 0 && (now - i) >= 0)//�����ͷ֮ǰ i ���ŵ�~~~~~~~~����¼~~~~~~~~~~ 
				{
					close = now - i;
					log[close]--;//��ȥһ�η��ʴ�������Ϊ���ʼ���Ҫ������)
					cnt += i;//�ۼӴ�ͷ�ƶ����� 
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
			now = rand() % BNUM;//���ָ�ɴ�ͷλ�� 
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
			for(int i = 0 ; i < BLENGTH ; i++)//����ÿ���ŵ�������������ܹ�Ҫ�����ʶ��ٴ� 
			{
				log[s.g(i)]++;
			} 
			fout<<setw(8)<<log[0];//��ʾ�ܵķ������ 
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

