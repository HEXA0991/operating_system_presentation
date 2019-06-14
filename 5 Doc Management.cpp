#include <iostream>
#include <windows.h>
#include <fstream>
#include <cstring>
#include <cstdio> 
#include <ctime>
#define BLOCK_SIZE 20 // 每个块2kB 
using namespace std;

ofstream fout("log_doc_management.txt");

class blocks
{
	private:
		char path[13];//根目录路径 
		char temp[13];
		char append[5];
		char num[4];
		bool blank[500];//空白块的位示图
	public:
		
		int create_folder(int i)//创建第 i 号文件夹（模拟块） 
		{
			int flag;
			strcpy(temp,path);
			itoa(i,num,10);
		    strcat(temp,num);
		    flag = CreateDirectory(temp,NULL);
		    return flag;
		}
		
		blocks()
		{
			CreateDirectory("E:\\Folder\\",NULL);//创建工程需要的根目录 
			char p[13] = "E:\\Folder\\";
	        char t[13] = "E:\\Folder\\";
	        char a[5] = ".txt";
	        char n[4] = "";
	        strcpy(path,p);
	        strcpy(temp,t);
	        strcpy(append,a);
	        strcpy(num,n);//初始化类的字符串数组 
	        for(int i = 0 ; i < 500 ; i++)//init the blocks
	        {
	        	blank[i] = true;//将每个块都设为可用状态 
			} 
	        cout<<"creating 500 blocks, each volume = 2kB\n"<<endl;
	        for(int i = 1 ; i <= 500 ; i++ )//创建 500 个块 
	        {
	        	if(!create_folder(i))//如果创建第 i 个文件夹失败
	        	{
	        		fout<<"creating "<<temp<<" folder error"<<endl;
	        		cout<<"creating "<<temp<<" folder error"<<endl;
	        		exit(1);
				}
				else
				{
					fout<<"creating "<<temp<<" folder success"<<endl;
				}
			}
			cout<<"creating 500 blocks complete\n"<<endl;
		}
		
		char *g_blank()//返回一个空闲块号，以字符串的形式 
		{
			char *dummy = new char[4];//存储暂时存放空闲块号字符串的字符数组 
			for(int i = 0 ; i < 4 ; i++)
			{
				dummy[i] = '\0';
			}
			bool flag = false;//将是否找到空 block 的标志置 false 
			int current = 0 ;//存储空闲块 
			for(current = 0 ; current < 500 ; current++ )//搜寻在存储空间中有没有空闲的块
			{
				if(blank[current])//如果 current 号块中空闲 
				{
					flag = true;//出现空闲块 
					break;
				}
			}
			if(flag)//如果有空闲块
			{
				blank[current] = false;//将 current 号块置为不可用 
				itoa(current+1,dummy,10);//块是从1开始标号 
				return dummy;
			} 
			else//代表没有空闲的块了
			{
				cout<<"\n cannot find a blank block!\n"<<endl;
				fout<<"\n cannot find a blank block!\n"<<endl;
				exit(1);
			} 
		}
		
		bool clear(int i)
		{
			if(i == -1)
			{
				cout<<"error in file destruction\n"<<endl;
				fout<<"error in file destruction\n"<<endl;
				return false;
			}
			if(!blank[i])
			{
				blank[i] = true;
				return true;
			}
			else
			{
				cout<<"\n#"<<i<<" block is already blank!\n"<<endl;
				fout<<"\n#"<<i<<" block is already blank!\n"<<endl;
				return false;
			}
		}
		
		void dis()
		{
			for(int i = 0 ; i < 500 ; i++)
			{
				cout<<(int)blank[i]<<" ";
			}
		}
};


class file
{
	private:
		char *name; //文件名 
	    char *path[5]; //文件存在位置，可能存在多个块内所以是指针数组 
    	int size; //文件大小
	public:
	    file(char *n,int s,char *p[5])
		{
			name = n;
			size = s;
			for(int i = 0 ; i < 5 ; i++)
			{
				path[i] = p[i];
			}
			for(int i = 0 ; (path[i] != NULL && i < 5) ; i++)//当路径不为空时继续创建 
			{
				ofstream f(path[i]);//在 path[i] 上创建文件
				f<<"this is file "<<name<<endl;
				f<<"size = "<<(float) size/10<<"kB"<<endl;
				if(i < 4)
				{
					if(path[i+1])//如果后面还有路径，证明整个文件在下一个块中还有
				    {
				    	f<<"fraction = #"<<i<<endl;
				    	f<<"remaining size = "<<(float) (size - 20)/10<<" kB"<<endl; 
				    	f<<"next fraction ="<<path[i+1];
				    	size -= 20;//下一个文件将少 2kB
				    }
				    else
				    {
				    	if(i == 0)//如果第一次创建文件时后面就没有目录，代表不分片
				    	{
				    		f<<"fraction = no";
				    	} 
				    	else//如果不是第一次，代表分片结束 
				    	{
				    		f<<"fraction = #"<<i<<endl;
				    		f<<"last fraction"<<endl;
				    	}
				    }
				}
				else
				{
					if(i == 0)//如果第一次创建文件时后面就没有目录，代表不分片
				    {
						f<<"fraction = no";
					} 
				    else//如果不是第一次，代表分片结束 
				    {
				    	f<<"fraction = #"<<i<<endl;
				    	f<<"last fraction"<<endl;
				    }
				}
				f.close();
			}
		}
		
		void destruction(blocks &b)
		{
			char temp[4] = "\0";
			int block_no = -1;
			for(int i = 0 ; i < 5 ; i++)
			{
				if(path[i])//如果有路径信息 
				{
					if(path[i][10] >= '1' && path[i][10] <= '9')//使用自动机来判断字符，并生成此文件的块号 
					{
						temp[0] = path[i][10];
						if(path[i][11] >= '0' && path[i][11] <= '9')
						{
							temp[1] = path[i][11];
							if(path[i][12] >= '0' && path[i][12] <= '9')
							{
								temp[2] = path[i][12];
								temp[3] = '\0';
							}
						}
					}
					block_no = atoi(temp);//将块号转换为 int 
					b.clear(block_no-1);//将此块状态改写为空 
					DeleteFile(path[i]);//删除该块下的文件分片
				}
			}
		}
		
		void dis()
		{
			char temp[4] = "\0";
			cout<<"\nname = "<<name<<endl;
			cout<<"size = "<<(float)size/10<<"kB"<<endl;
			cout<<"stored in block ";
			for(int i = 0 ; i < 5 ; i++)
			{
				temp[0] = '\0';
				if(path[i])//如果有路径信息 
				{
					if(path[i][10] >= '1' && path[i][10] <= '9')//使用自动机来判断字符
					{
						temp[0] = path[i][10];
						if(path[i][11] >= '0' && path[i][11] <= '9')
						{
							temp[1] = path[i][11];
							if(path[i][12] >= '0' && path[i][12] <= '9')
							{
								temp[2] = path[i][12];
								temp[3] = '\0';
							}
							else
							{
								temp[2] = '\0';
							}
						}
						else
						{
							temp[1] = '\0';
						}
					}
					cout<<"#"<<temp<<" ";
				}
			}
			cout<<endl;
		}
};



class index //所有 FileControlBlock 的索引
{
	private:
		char *name[55];//文件名称索引 
		file *ind[55];//控制块索引
		char path[13];//文件的根目录路径 
		char append[5];//.txt后缀 
		char num[4]; //存储文件名的临时变量
		char *temp;//存放路径的临时字符串指针 
	public:
		index()
		{
			char p[13] = "E:\\Folder\\";
	        char t[20] = "E:\\Folder\\";
	        char a[5] = ".txt";
	        char n[4] = "";
	        strcpy(path,p);
	        strcpy(append,a);
	        strcpy(num,n);//初始化所有路径变量 
			for(int i = 0 ; i < 55 ; i++)
			{
				name[i] = NULL;
			}
			for(int i = 0 ; i < 55 ; i++)
			{
				ind[i] = NULL;
			}
		}
		
		int create_file(char *ch,blocks &b,int s = 0)
		{
			int size;
			char *temp_path[5];//暂时存储文件存放路径
			for(int j = 0 ; j < 5 ; j++)
			{
				temp_path[j] = NULL;
			}
			if(s == 0)//如果传入的 size == 0，代表想让程序随机生成一个大小 
			{
				size =rand()%80 + 20;//随机生成 2kB - 10kB 的文件体积 
			}
			else
			{
				size = s;
			}
			int i = 0 ;//存放空白索引的位置 
			for(int j = 0 ; j < 4 ; j++)//将文件名暂时存储
			{
				num[j] = *(ch+j);
		    }
			for(i = 0 ; i < 55 ; i++)//寻找空白索引指针的位置 
			{
				if(ind[i] == NULL)//如果找到空白索引指针，那么相对应的文件名也应该是空白的 
				{
					break;
				}
			}
			if(i != 55)//如果找到空白索引指针，创建文件 
			{
				name[i] = ch;//赋文件名
				int j = 0;
				for(int t_size = size ; t_size > 0 ; t_size-=20)//当文件剩余体积 > 0 时，继续寻找下一个路径 
			    {
			    	temp = new char[20];
					*temp = '\0';
				    strcpy(temp,path);//初始化中间数组中间变量
			        strcat(temp,b.g_blank());//将空白块的路径赋给中间变量
			        strcat(temp,"\\"); 
			        strcat(temp,ch);//将文件名赋给中间变量 
			        strcat(temp,append);
			        temp_path[j] = temp;//将生成的单个路径交给存放路径的临时变量 
			        j++;
			    }
			    file *f= new file(ch,size,temp_path);//创建文件 i节点 
			    ind[i] = f;//存储文件 i节点至索引
			    fout<<"create file "<<ch<<".txt success"<<endl;
			}
			else
			{
				cout<<"\n cannot find a blank index!\n"<<endl;
				cout<<"create file "<<ch<<".txt failed"<<endl;
				fout<<"\n cannot find a blank index!\n"<<endl;
				fout<<"create file "<<ch<<".txt failed"<<endl;
				exit(1);
			}	
		}
		
		bool delete_file(char *s,blocks &b)
		{
			int i = 0;
			for(i = 0 ; i < 55 ; i++)//循环查找与要删除的文件名所在的位置 
			{
				if(name[i])//if(name[i] != NULL) 
				{
					if(!strcmp(name[i],s))//if name[i] == s
				    {
			    		ind[i]->destruction(b);//删除文件 
			    		delete[] ind[i];//删除文件 i_node 
			    		ind[i] = NULL;//索引置空 
			    		name[i] = NULL;
			    		break;
			    	}
				}
			}
			if(i == 55)//如果循环被完整执行 55 次，代表没有找到相应文件名的文件，抛出错误信息 
			{
				cout<<"\ncannot find file "<<s<<endl<<endl;
				fout<<"\ncannot find file "<<s<<endl<<endl;
				return false;
			}
			else
			{
			    fout<<"delete file "<<s<<".txt success"<<endl;
				return true;
			}
		}
		
		bool dis(char *s)
		{
			int i = 0;
			for(i = 0 ; i < 55 ; i++)//循环查找文件名所在的位置 
			{
				if(name[i])//if(name[i] != NULL) 
				{
					if(!strcmp(name[i],s))//if name[i] == s
				    {
			    		ind[i]->dis();
			    		break;
			    	}
				}
			}
			if(i == 55)//如果循环被完整执行 55 次，代表没有找到相应文件名的文件，抛出错误信息 
			{
				cout<<"\ncannot find file "<<s<<endl<<endl;
				fout<<"\ncannot find file "<<s<<endl<<endl;
				return false;
			}
			else
			{
				return true;
			}
		}
		
		~index()
		{
			for(int i = 0 ; i < 55 ; i++)
			{
				if(ind[i])
				{
					//delete[] name[i];    //这里为啥不行？？？ 
					delete[] ind[i];
				}
			}
		}
}; 



int  main()
{
	srand(int(time(0)));//get the seed ready
	blocks b;
	index j;
	cout<<"\ncreating 50 files"<<endl;
	for(int i = 0 ; i < 50 ; i++)
	{
		char *name = new char[3];
		for(int k = 0 ; k < 3 ; k++)
		{
			*(name + k) = '\0';
		}
		itoa (i+1,name,10);
		j.create_file(name,b);
	}
	cout<<"create complete\n"<<endl;
	cout<<"memory status:"<<endl;
	b.dis();
	cout<<endl;
	system("pause");
	cout<<"delete the odd-number-file"<<endl;
	for(int i = 1 ; i <= 49 ; i += 2)
	{
		char *name = new char[3];
		for(int k = 0 ; k < 3 ; k++)
		{
			*(name + k) = '\0';
		}
		itoa (i,name,10);
		j.delete_file(name,b);
	}
	cout<<"delete complete\n"<<endl;
	cout<<"memory status:"<<endl;
	b.dis();
	cout<<endl;
	system("pause");
	cout<<"create A.txt"<<endl;
	j.create_file("A",b,70);
	cout<<"create B.txt"<<endl;
	j.create_file("B",b,50);
	cout<<"create C.txt"<<endl;
	j.create_file("C",b,20);
	cout<<"create D.txt"<<endl;
	j.create_file("D",b,90);
	cout<<"create E.txt"<<endl;
	j.create_file("E",b,35);
	j.dis("A");
	j.dis("B");
	j.dis("C");
	j.dis("D");
	j.dis("E");
	cout<<"memory status:"<<endl;
	b.dis();
}
