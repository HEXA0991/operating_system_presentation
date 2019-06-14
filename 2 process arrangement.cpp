#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <fstream>
#define PNUM 10
using namespace std;

ofstream fout("log_Process.txt");

class list
{
	private:
		int data[3];//0:pid 1:arrive time 2:service time
		list *next;
		list *pre;
	public:
		list()
		{
			next=NULL;
			pre=NULL;
		}
		void set(int pid,int *a,list *pre_,list *next_)
		{
			data[0]=pid;
			data[1]=a[0];
			data[2]=a[1];
			pre=pre_;
			next=next_;
		}
		void append(list *next_)//append a list
		{
			next=next_;
		}
		void cut_in(list *pre_)//put a list in the front
		{
			pre=pre_;
		}
		int *get()
		{
			return data;
		}
		list *g_next()
		{
			return next;
		}
		list *g_pre()
		{
			return pre;
		}
};

class link//a round link contains process info
{
	private:
		list *front;
		list *rear;
		int total;
	public:
		link(int a[][2])
		{
			list *now;
			total=PNUM;
			front=new list;//initialize
			front->set(0,*a,front,front);
			rear = front;
			for(int i=1;i<total;i++)
			{
				now=new list;//create a new list
				now->set(i,*(a+i),rear,front);
				rear->append(now);//append the new list after the rear list
				front->cut_in(now);//let the forefront list know
				rear=now;//move on to the next insertion
			}
		/*	list *temp=front;
			for(int j=0;j<50;j++)
			{
				cout<<temp->get()[0]<<" ";
				temp=temp->g_next();
			}*/ // this is test code for a double-direction-round-linklist
		}
		list *get()
		{
			return front;
		}
		int g_total()
		{
			return total;
		}
		void set_front(list *new_front)
		{
			front=new_front;
		}
		void del_list(list *del)
		{
			list *pre,*next;
			pre=del->g_pre();
			next=del->g_next();
			pre->append(next);
			next->cut_in(pre);
			delete del;
		}
		list *g_front()
		{
			return front;
		}
};

class process
{
	private:
		int attribute[PNUM][2];//start time ont the front followed by service time
	public:
		process()
		{
			srand(int(time(0)));//get seed ready
			attribute[0][0]=0;//1st start time
			attribute[0][1]=(rand()%10+1);//1st service time
			for(int i=1,start_time=0;i<PNUM;i++)
			{
				attribute[i][0]=start_time+(rand()%10);//get a start time that other than 0
				start_time=attribute[i][0];//set the new start_time
				attribute[i][1]=(rand()%10+1);//get a service time 
			}
			cout<<"process   arrive_time    service_time\n";
			for(int i=0;i<PNUM;i++)
			{
				cout<<setw(4)<<i/*+1*/<<setw(12)<<attribute[i][0]<<setw(15)<<attribute[i][1]<<endl;
				fout<<setw(4)<<i/*+1*/<<setw(12)<<attribute[i][0]<<setw(15)<<attribute[i][1]<<endl;//print to the log.txt
			}
		}
		int (*get())[2]
		{
			return attribute;
		}
};

class HRN
{
	private:
	    list *l;
		int log[PNUM][3];//[0]:total time [1]:service time [2]:turnaround time
		float pri[PNUM];//priority of processes
		float TTR[PNUM];//Turnaround Time with Rights
		int total;//the time that plugged in
		int p_total;//the number of process that exist right now
		int service;//the service time of current process
		int i;//a temp container for pid
		float max;//the maximum value of priority
		int pid_max;//the pid of maximum priority process
		list *p_max;//the ptr of MPP
	public:
		void p_finish(int current,int total,int service,int turnaround,float ttr,float pri)//print the finish info
		{
			cout<<"\n#"<<current<<" process finished after "<<total<<" cycle(s) "; 
			cout<<"service time is "<<service<<" cycle(s)\n";
			cout<<"priority is "<<pri;
			cout<<" turnaround time is "<<turnaround<<" turnaround time with right is "<<ttr<<endl;
		}
		void set(int pid,int total_time,int service,int arrive)
		{
			log[pid][0]=total_time;
			log[pid][1]=service;
			log[pid][2]=(total_time - arrive);
		}
		void up_pri(int pid,int arrive,int serve)//update the priority
		{
			pri[pid]=((float)((total-arrive)+serve)/(float)serve);
		}
		HRN(link li)
		{
			int flag=0;//whether a new process appeared or not
			int flag_compared=0;//to detect if the front ptr was compared(avoid all process arrived and dead-loop in comparison)
			service=0;
			max=0.0;//init the maximum
			p_max=0;
			list *front;
			l=li.g_front();
			front=l;
			p_max=l;
			p_total=PNUM;
			total=0;
			list *temp;//update ptr
			for(int j=0;j < p_total;j++)//init the priority
			{
				pri[j]=1.0;
			}
			for(int j=0;j<p_total;j++)//init the log
			{
				for(int k=0;k<3;k++)
				{
					log[j][k]=0;
				}
			}
			while(p_total != 0)//if there still some processes left
			{
				max=0.0;//init the maximum
				service=0;
				flag=0;
				flag_compared=0;
				for(int j=0;p_total != 0;j++)//search for the pri max process
				{
					if(l->get()[1] > total || flag_compared >= 2)//arrive time > total || the front ptr was compared twice(that means all process was arrived)
					{
						if(flag == 0)//if no process was chose in this loop
					    {
					//	    cout<<"time="<<total<<" idle"<<endl;
					        fout<<"time="<<total<<" idle"<<endl;
						    total++;
					    	continue;
					    }
					    i=p_max->get()[0];
						l=p_max;//choose the pri max process
						break;
					}
					i=l->get()[0];
					if(pri[i] > max)//if pri > max
					{
						flag=1;//a pri-greater process that appeared
						p_max=l;
						pid_max=i;
						max=pri[i];
					}
					if(l == front)
					{
						flag_compared++;
					}
					l=l->g_next();
				}
				for(;service < l->get()[2] ;service++)//serve the process
				{
			//		cout<<"time="<<total<<" serve time= "<<service<<" process #"<<i<<endl;
					fout<<"time="<<total<<" serve time= "<<service<<" process #"<<i<<endl;
					total++;
				}
			//	cout<<"time="<<total<<" serve time= "<<service<<" process #"<<i<<" finished\n"<<endl;
				fout<<"time="<<total<<" serve time= "<<service<<" process #"<<i<<" finished\n"<<endl;
				set(i,total,service,l->get()[1]);//save the scene
				if(p_max == front)//if the process that need to be deleted is the front
				{
					front=front->g_next();
				}
				l=front;//point the current process to the head
				li.del_list(p_max);//delete the finished process
				p_total--;
				temp=front;
				for(int j=0;j < p_total;j++)//update all the pri of processes which meet the request
				{
					if(temp->get()[1] <= total)
					{
						up_pri(temp->get()[0],temp->get()[1],temp->get()[2]);
					}
					if(temp->get()[1] > total)//if update procedure is finished
					{
						break;
					}
					temp=temp->g_next();
				}
			}
			for(int j=0;j<PNUM;j++)//calc the turnaround time with right
			{
				TTR[j]=((float)log[j][1]/(float)log[j][2]);
			}
			for(int j=0;j<PNUM;j++)//print the result
			{
				p_finish(j,log[j][0],log[j][1],log[j][2],TTR[j],pri[j]);
			}
		}
};

class SJF
{
	private:
	    list *l;
		int log[PNUM][3];//[0]:total time [1]:service time [3]:turnaround time
		float TTR[PNUM];//Turnaround Time with Rights
		int total;//the time that plugged in
		int p_total;//the number of process that exist right now
		int service;//the service time of current process
		int i;//a temp container for pid
		int min;//the minimum value of service time
		int pid_min;//the pid of minimum service time process
		list *p_min;//the ptr of MSTP
	public:
		void p_finish(int current,int total,int service,int turnaround,float ttr)//print the finish info
		{
			cout<<"\n#"<<current<<" process finished after "<<total<<" cycle(s) "; 
			cout<<"service time is "<<service<<" cycle(s)\n";
			cout<<"turnaround time is "<<turnaround<<" turnaround time with right is "<<ttr<<endl;
		}
		void set(int pid,int total_time,int service,int arrive)
		{
			log[pid][0]=total_time;
			log[pid][1]=service;
			log[pid][2]=(total_time - arrive);
		}
		SJF()
		{
			l=NULL;
			p_min=NULL;
		}
		SJF(link li)
		{
			int flag=0;//whether a new process appeared or not
			int flag_compared=0;//to detect if the front ptr was compared(avoid all process arrived and dead-loop in comparison)
			service=0;
			min=9999;//init the minimum
			p_min=0;
			list *front;
			l=li.g_front();
			front=l;
			p_min=l;
			p_total=PNUM;
			total=0;
			for(int j=0;j<p_total;j++)
			{
				for(int k=0;k<3;k++)
				{
					log[j][k]=0;
				}
			}
			while(p_total != 0)//if there still some processes left
			{
				min=9999;//init the minimum
				service=0;
				flag=0;
				flag_compared=0;
				for(int j=0;p_total != 0;j++)//search for the shortest process
				{
					if(l->get()[1] > total || flag_compared >= 2)//arrive time > total || the front ptr was compared twice(that means all process was arrived)
					{
						if(flag == 0)//if no process was chose in this loop
					    {
					//	    cout<<"time="<<total<<" idle"<<endl;
						    fout<<"time="<<total<<" idle"<<endl;
						    total++;
					    	continue;
					    }
					    i=p_min->get()[0];
						l=p_min;//choose the min-service process
						break;
					}
					i=l->get()[0];
					if(l->get()[2] < min)//if service time < min 
					{
						flag=1;//a smaller process that appeared
						p_min=l;
						pid_min=i;
						min=l->get()[2];
					}
					if(l == front)
					{
						flag_compared++;
					}
					l=l->g_next();
				}
				for(;service < min ;service++)//serve the process
				{
			//		cout<<"time="<<total<<" serve time= "<<service<<" process #"<<i<<endl;
					fout<<"time="<<total<<" serve time= "<<service<<" process #"<<i<<endl;
					total++;
				}
			//	cout<<"time="<<total<<" serve time= "<<service<<" process #"<<i<<" finished\n"<<endl;
				fout<<"time="<<total<<" serve time= "<<service<<" process #"<<i<<" finished\n"<<endl;
				set(i,total,service,l->get()[1]);//save the scene
				if(p_min == front)//if the process that need to be deleted is the front
				{
					front=front->g_next();
				}
				l=front;//point the current process to the head
				li.del_list(p_min);//delete the finished process
				p_total--;
			}
			for(int j=0;j<PNUM;j++)//calc the turnaround time with right
			{
				TTR[j]=((float)log[j][1]/(float)log[j][2]);
			}
			for(int j=0;j<PNUM;j++)//print the result
			{
				p_finish(j,log[j][0],log[j][1],log[j][2],TTR[j]);
			}
			
		}
};

class RR
{
	private:
		list *l;
		int log[PNUM][3];//[0]:total time [1]:service time [3]:turnaround time
		float TTR[PNUM];//Turnaround Time with Rights
		int i;//the pid of current running process
		int total;//the time that plugged in
		int p_total;
	public:
	    void p_finish(int current,int total,int service,int turnaround,float ttr)//print the finish info
		{
			cout<<"\n#"<<current<<" process finished after "<<total<<" cycle(s) "; 
			cout<<"service time is "<<service<<" cycle(s)\n";
			cout<<"turnaround time is "<<turnaround<<" turnaround time with right is "<<ttr<<endl;
		}
		void set(int pid,int total_time,int arrive)
		{
			log[pid][0]=total_time;
			log[pid][2]=(total_time - arrive);
		}
		void inc(int pid)
		{
			log[pid][1]++;
		}
		RR()
		{
			l=NULL;
		}
		RR(link li)
		{
			int flag=0;//whether the current process was deleted or not
			int flag_processed=0;//to detect that is there a process which is processed in last cycle
			list *front;
			l=li.g_front();
			front=l;
			p_total=PNUM;
			total=0;
			for(int j=0;j<p_total;j++)//init the log
			{
				for(int k=0;k<3;k++)
				{
					log[j][k]=0;
				}
			}
			while(p_total!=0)//if there still some processes left
			{
				flag=0;
				i=l->get()[0];
				if(total < l->get()[1])//if no process arrive on this time
				{
					if(flag_processed == 0)
					{
						total++;
				//		cout<<"time="<<total<<" idle"<<endl;
						fout<<"time="<<total<<" idle"<<endl;
					}
					l=front;
					flag_processed=0;
				}
				else
				{
					flag_processed=1;
				//	cout<<"time="<<total<<" serve time= "<<log[i][1]+1<<" process #"<<i<<endl;
					fout<<"time="<<total<<" serve time= "<<log[i][1]+1<<" process #"<<i<<endl;
				    total++;
				    inc(l->get()[0]);//serve the process
				    if(log[i][1] == l->get()[2])//if the process finished
				    {
				//    	cout<<"process #"<<i<<" finished\n\n";
				    	fout<<"process #"<<i<<" finished\n\n";
				    	flag=1;
				    	list *temp;
				    	set(i,total,l->get()[1]);//save the scene
				    	temp=l;
				    	l=l->g_next();
				    	if(temp == front)//if the process that need to be deleted is the front
				    	{
				    		front=front->g_next();
						}
				    	li.del_list(temp);//delete the finished process
				    	p_total--;
					}
					if(flag != 1)
					{
						l=l->g_next();
					}
				}
			}
			for(int j=0;j<PNUM;j++)//calc the turnaround time with right
			{
				TTR[j]=((float)log[j][1]/(float)log[j][2]);
			}
			for(int j=0;j<PNUM;j++)//print the result
			{
				p_finish(j,log[j][0],log[j][1],log[j][2],TTR[j]);
			}
		}
		
};

class FCFS
{
	private:
		int log[PNUM][3];//[0]:total time [1]:service time [3]:turnaround time
		float TTR[PNUM];//Turnaround Time with Rights
		int i;//the pid of current running process
		int total;//the time that plugged in
		int service;//current process time
	public:
		void p_finish(int current,int total,int service,int turnaround,float ttr)//print the finish info
		{
			cout<<"\n#"<<current+1<<" process finished after "<<total<<" cycle(s) \n"; 
			cout<<"turnaround time is "<<turnaround<<" turnaround time with right is "<<ttr<<endl;
		}
		FCFS(int array[][2])
		{
			i=0;
			total=0;
			service=0;//initialize
			for(i=0;i<PNUM;i++)//big loop that changes between processes
			{
				if(total < **(array+i))//if the total cycle < arrive time, fast forward to it
				{
					total=**(array+i);
			//		cout<<"\nfast forward to total="<<total<<endl;
					fout<<"\nfast forward to total="<<total<<endl;
				}
			//	cout<<"\nnow start to serve process #"<<i+1<<endl;
				fout<<"\nnow start to serve process #"<<i+1<<endl;
				for(service=0;service < *(*(array+i)+1);service++)//provide service
				{
			//		cout<<"time="<<total<<" serve time= "<<service<<" process #"<<i+1<<endl;
					fout<<"time="<<total<<" serve time= "<<service<<" process #"<<i+1<<endl;
					total++;
				}
			//	cout<<"time="<<total<<" serve time= "<<service<<" process #"<<i+1<<" finished"<<endl;
				fout<<"time="<<total<<" serve time= "<<service<<" process #"<<i+1<<" finished"<<endl;
				log[i][0]=total;//save the scene
				log[i][1]=service;
				log[i][2]=(total-**(array+i));
				TTR[i]=((float)service/(float)(total-**(array+i)));	
			}
			for(int j=0;j<PNUM;j++)//print the result
			{
				p_finish(j,log[j][0],log[j][1],log[j][2],TTR[j]);
			}
		}	
};

int main()
{
	process p1;
	cout<<"\nFCFS:\n";
	fout<<"\nFCFS:\n";
	FCFS(p1.get());
	link l1(p1.get()),l2(p1.get()),l3(p1.get());
	cout<<"\nRR:\n";
	fout<<"\nRR:\n";
	RR r1(l1);
	cout<<"\nSJF:\n";
	fout<<"\nSJF:\n";
	SJF s1(l2);
	cout<<"\nHRN\n";
	fout<<"\nHRN\n";
	HRN h1(l3);
	fout.close();
}
