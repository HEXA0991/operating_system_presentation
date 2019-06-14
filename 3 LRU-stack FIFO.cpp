#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#define PAGENUM 4 //硬件支持的块数 
#define SEQLENGTH 20

using namespace std;

ofstream fout("log_FIFO&LRU.txt");

class sequence
{
	private:
		int seq[SEQLENGTH];//请求页号序列 
		int totalpagenum;//请求序列长度 
	public:
		sequence()
		{
			totalpagenum = 9;
			srand(int(time(0)));//get the seed ready
			for(int i = 0;i < SEQLENGTH;i++)
			{
				seq[i]=(rand() % totalpagenum + 1);
			}
		}
		int *g()
		{
			return seq;
		}
		int tpn()//返回请求序列长度 
		{
			return totalpagenum;
		}
};

class list
{
	private:
		int page;//储存的页号 
		list *pre;//前一个块 
		list *next;//n后一个块 
	public:
		list()
		{
			page = 0;
		}
		void init_set(list *pre_,list *next_)
		{
			pre = pre_;
			next = next_;
		}
		void set(int i)
		{
			page = i;
		}
		void append(list *block)//连接到尾指针 
		{
			next = block;
		}
		void frontie(list *block)//连接到头指针 
		{
			pre = block;
		}
		int g()//g返回块中储存的页号 
		{
			return page;
		}
		list *g_next()//返回下一个块的指针 
		{
			return next;
		}
};

class stack
{
	private:
		int s[PAGENUM];
	public:
		int top;
		stack()
		{
			for(int i = 0;i < PAGENUM;i++)
			{
				s[i]=0;
			}
			top = -1;
		}
		int search(int page)//输入要找的页号，返回储存该页储存的位置，如果没有找到，返回 -1
		{
			for(int i =0 ;i < top + 1;i++)
			{
				if(s[i] == page)
				{
					return i;
				}
			}
			return -1;
		}
		int move(int from)//将从栈顶到次=此位置储存的页全部往下移一个块 
		{
			int temp = s[from];
			for(int i = from + 1;i < PAGENUM;i++)
			{
				s[i - 1] = s[i];
			}
			return temp;
		}
		void push(int page)//将此页放入栈顶 
		{
			if(top < PAGENUM - 1)
			{
				s[++top] = page;
			}
			else
			{
				s[top] = page;
			}
		}
		int *g()//get the stack
		{
			return s;
		}
	
};

class LRU
{
	private:
		stack s;
		int replaced;//换出的页信息 
		int miss_cnt;//缺页计数器 
	public:
		void print(int i,sequence seq,int flag)//flag==0 miss flag==1 hit
		{
			fout<<setw(6)<<seq.g()[i];
			if(s.g()[0])
			{
				fout<<setw(11)<<s.g()[0];
			}
			else
			{
				fout<<setw(11)<<" ";
			}
			for(int j=1;j < PAGENUM;j++)
			{
				if(s.g()[j])
				{
					fout<<setw(9)<<s.g()[j];
				}
				else
				{
					fout<<setw(9)<<" ";
				}
			}
			if(flag)//if hit(no missing page)
			{
				fout<<setw(13)<<"N";
			}
			else
			{
				fout<<setw(13)<<"Y";
				fout<<setw(13)<<replaced;
			}
			fout<<endl;
		}
		LRU(sequence seq)
		{
			int temp;
			miss_cnt = 0;
			fout<<setw(10)<<"wanted_page ";
			for(int j=0;j < PAGENUM;j++)
			{
				if( j < 9 )
				{
					fout<<setw(8)<<"block#"<<j+1;
				} 
				else
				{
					fout<<setw(7)<<"block#"<<j+1;
				}
			}
			fout<<setw(15)<<"page_missing?";
			fout<<setw(15)<<"replaced_page\n";
			for(int i = 0;i < SEQLENGTH; i++)
			{
				temp = s.search(seq.g()[i]);//保存要寻找的页的位置
				if(temp != -1)//if hit
				{
					s.move(temp);
					s.push(seq.g()[i]);
					print(i,seq,1);
				}
				else//if miss
				{
					replaced = s.g()[0];//save the stack bottom
					if(s.top != PAGENUM -1)
					{
						replaced = 0;
					}
					if(s.top == PAGENUM -1)
					{
						s.move(0);
					}
					s.push(seq.g()[i]);
					print(i,seq,0);
					miss_cnt++;
				}
			}
			fout<<"miss time ="<<miss_cnt<<endl;
			fout<<"miss rate ="<<(((float) miss_cnt)/((float) SEQLENGTH))*100<<"%\n";
			cout<<"miss time ="<<miss_cnt<<endl;
			cout<<"miss rate ="<<(((float) miss_cnt)/((float) SEQLENGTH))*100<<"%\n";
		}
};

class FIFO
{
	private:
		list *front;
		list *rear;
		int i;//目前请求的页号 
	//	int log[SEQLENGTH];//page missing log
		int miss_cnt;//缺页信息 
		int r_pno;//换出的页号 
	public:
		void print(int flag,sequence s)
		{
			list *temp;
			temp = front;
			fout<<setw(6)<<s.g()[i];
			if(temp->g())
			{
				fout<<setw(11)<<temp->g();
			}
			else
			{
				fout<<setw(11)<<" ";
			}
			temp = temp->g_next();
			for(int j=0;j < PAGENUM - 1;j++)
			{
				if(temp->g())
				{
					fout<<setw(9)<<temp->g();
				    temp = temp->g_next();
				}
				else
				{
					fout<<setw(9)<<" ";
				    temp = temp->g_next();
				}
			}
			if(!flag)//if hit(no missing page)
			{
				fout<<setw(13)<<"Y";
				fout<<setw(13)<<r_pno;
			}
			else
			{
				fout<<setw(13)<<"N";
			}
			fout<<endl;
		}
		FIFO(sequence s)
		{
			miss_cnt = 0;//init the miss counter
			i=0;//init the current seq_no
			fout<<setw(10)<<"wanted_page ";
			for(int j=0;j < PAGENUM;j++)
			{
				if( j < 9 )
				{
					fout<<setw(8)<<"block#"<<j+1;
				} 
				else
				{
					fout<<setw(7)<<"block#"<<j+1;
				}
			}
			fout<<setw(15)<<"page_missing?";
			fout<<setw(15)<<"replaced_page\n";
			int f_hit=0;//hitted-or-not flag
			list *temp;
			front = new list;
			front->init_set(front,front);//init the first list
			rear = front;
			for(int j = 1;j < PAGENUM;j++)//create a linklist
			{
				temp = new list;
				front->frontie(temp);
				rear->append(temp);
				temp->init_set(rear,front);
				rear = temp;
			}
			for(i=0;i < SEQLENGTH;i++)//i == 目前进行到请求序列的第几个 
			{
				temp = front;
				f_hit = 0;//0:miss 
				for(int j=0;j < PAGENUM;j++)//寻找请求的页的位置 
				{
					if(temp->g() == s.g()[i])//if hit
					{
						f_hit = 1;
						break;
					}
					temp = temp->g_next();
				}
				if(f_hit == 1)///if hit
				{
					print(f_hit,s);
				}
				else
				{
					miss_cnt++;
					r_pno = front->g();
					front->set(s.g()[i]);
					front = front->g_next();
					rear = rear->g_next();
					print(f_hit,s);
				}
			}
			cout<<"miss time ="<<miss_cnt<<endl;
			cout<<"miss rate ="<<(((float) miss_cnt)/((float) SEQLENGTH))*100<<"%\n";
			fout<<"miss time ="<<miss_cnt<<endl;
			fout<<"miss rate ="<<(((float) miss_cnt)/((float) SEQLENGTH))*100<<"%\n";
		}
		~FIFO()
		{
			list *temp;
			if(front->g_next())
			{
				temp = front->g_next();
				delete front;
				front = temp;
			}
		}
};

int main()
{
	sequence s;
	cout<<"FIFO:\n";
	fout<<"FIFO:\n";
	FIFO f(s);
	cout<<"LRU:\n";
	fout<<"LRU:\n";
	LRU l(s);
}
