#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#define PAGENUM 4 //Ӳ��֧�ֵĿ��� 
#define SEQLENGTH 20

using namespace std;

ofstream fout("log_FIFO&LRU.txt");

class sequence
{
	private:
		int seq[SEQLENGTH];//����ҳ������ 
		int totalpagenum;//�������г��� 
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
		int tpn()//�����������г��� 
		{
			return totalpagenum;
		}
};

class list
{
	private:
		int page;//�����ҳ�� 
		list *pre;//ǰһ���� 
		list *next;//n��һ���� 
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
		void append(list *block)//���ӵ�βָ�� 
		{
			next = block;
		}
		void frontie(list *block)//���ӵ�ͷָ�� 
		{
			pre = block;
		}
		int g()//g���ؿ��д����ҳ�� 
		{
			return page;
		}
		list *g_next()//������һ�����ָ�� 
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
		int search(int page)//����Ҫ�ҵ�ҳ�ţ����ش����ҳ�����λ�ã����û���ҵ������� -1
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
		int move(int from)//����ջ������=��λ�ô����ҳȫ��������һ���� 
		{
			int temp = s[from];
			for(int i = from + 1;i < PAGENUM;i++)
			{
				s[i - 1] = s[i];
			}
			return temp;
		}
		void push(int page)//����ҳ����ջ�� 
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
		int replaced;//������ҳ��Ϣ 
		int miss_cnt;//ȱҳ������ 
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
				temp = s.search(seq.g()[i]);//����ҪѰ�ҵ�ҳ��λ��
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
		int i;//Ŀǰ�����ҳ�� 
	//	int log[SEQLENGTH];//page missing log
		int miss_cnt;//ȱҳ��Ϣ 
		int r_pno;//������ҳ�� 
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
			for(i=0;i < SEQLENGTH;i++)//i == Ŀǰ���е��������еĵڼ��� 
			{
				temp = front;
				f_hit = 0;//0:miss 
				for(int j=0;j < PAGENUM;j++)//Ѱ�������ҳ��λ�� 
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
