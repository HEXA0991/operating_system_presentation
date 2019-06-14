#include <iostream>
#include <windows.h>
#include <fstream>
#include <cstring>
#include <cstdio> 
#include <ctime>
#define BLOCK_SIZE 20 // ÿ����2kB 
using namespace std;

ofstream fout("log_doc_management.txt");

class blocks
{
	private:
		char path[13];//��Ŀ¼·�� 
		char temp[13];
		char append[5];
		char num[4];
		bool blank[500];//�հ׿��λʾͼ
	public:
		
		int create_folder(int i)//������ i ���ļ��У�ģ��飩 
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
			CreateDirectory("E:\\Folder\\",NULL);//����������Ҫ�ĸ�Ŀ¼ 
			char p[13] = "E:\\Folder\\";
	        char t[13] = "E:\\Folder\\";
	        char a[5] = ".txt";
	        char n[4] = "";
	        strcpy(path,p);
	        strcpy(temp,t);
	        strcpy(append,a);
	        strcpy(num,n);//��ʼ������ַ������� 
	        for(int i = 0 ; i < 500 ; i++)//init the blocks
	        {
	        	blank[i] = true;//��ÿ���鶼��Ϊ����״̬ 
			} 
	        cout<<"creating 500 blocks, each volume = 2kB\n"<<endl;
	        for(int i = 1 ; i <= 500 ; i++ )//���� 500 ���� 
	        {
	        	if(!create_folder(i))//��������� i ���ļ���ʧ��
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
		
		char *g_blank()//����һ�����п�ţ����ַ�������ʽ 
		{
			char *dummy = new char[4];//�洢��ʱ��ſ��п���ַ������ַ����� 
			for(int i = 0 ; i < 4 ; i++)
			{
				dummy[i] = '\0';
			}
			bool flag = false;//���Ƿ��ҵ��� block �ı�־�� false 
			int current = 0 ;//�洢���п� 
			for(current = 0 ; current < 500 ; current++ )//��Ѱ�ڴ洢�ռ�����û�п��еĿ�
			{
				if(blank[current])//��� current �ſ��п��� 
				{
					flag = true;//���ֿ��п� 
					break;
				}
			}
			if(flag)//����п��п�
			{
				blank[current] = false;//�� current �ſ���Ϊ������ 
				itoa(current+1,dummy,10);//���Ǵ�1��ʼ��� 
				return dummy;
			} 
			else//����û�п��еĿ���
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
		char *name; //�ļ��� 
	    char *path[5]; //�ļ�����λ�ã����ܴ��ڶ������������ָ������ 
    	int size; //�ļ���С
	public:
	    file(char *n,int s,char *p[5])
		{
			name = n;
			size = s;
			for(int i = 0 ; i < 5 ; i++)
			{
				path[i] = p[i];
			}
			for(int i = 0 ; (path[i] != NULL && i < 5) ; i++)//��·����Ϊ��ʱ�������� 
			{
				ofstream f(path[i]);//�� path[i] �ϴ����ļ�
				f<<"this is file "<<name<<endl;
				f<<"size = "<<(float) size/10<<"kB"<<endl;
				if(i < 4)
				{
					if(path[i+1])//������滹��·����֤�������ļ�����һ�����л���
				    {
				    	f<<"fraction = #"<<i<<endl;
				    	f<<"remaining size = "<<(float) (size - 20)/10<<" kB"<<endl; 
				    	f<<"next fraction ="<<path[i+1];
				    	size -= 20;//��һ���ļ����� 2kB
				    }
				    else
				    {
				    	if(i == 0)//�����һ�δ����ļ�ʱ�����û��Ŀ¼��������Ƭ
				    	{
				    		f<<"fraction = no";
				    	} 
				    	else//������ǵ�һ�Σ������Ƭ���� 
				    	{
				    		f<<"fraction = #"<<i<<endl;
				    		f<<"last fraction"<<endl;
				    	}
				    }
				}
				else
				{
					if(i == 0)//�����һ�δ����ļ�ʱ�����û��Ŀ¼��������Ƭ
				    {
						f<<"fraction = no";
					} 
				    else//������ǵ�һ�Σ������Ƭ���� 
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
				if(path[i])//�����·����Ϣ 
				{
					if(path[i][10] >= '1' && path[i][10] <= '9')//ʹ���Զ������ж��ַ��������ɴ��ļ��Ŀ�� 
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
					block_no = atoi(temp);//�����ת��Ϊ int 
					b.clear(block_no-1);//���˿�״̬��дΪ�� 
					DeleteFile(path[i]);//ɾ���ÿ��µ��ļ���Ƭ
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
				if(path[i])//�����·����Ϣ 
				{
					if(path[i][10] >= '1' && path[i][10] <= '9')//ʹ���Զ������ж��ַ�
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



class index //���� FileControlBlock ������
{
	private:
		char *name[55];//�ļ��������� 
		file *ind[55];//���ƿ�����
		char path[13];//�ļ��ĸ�Ŀ¼·�� 
		char append[5];//.txt��׺ 
		char num[4]; //�洢�ļ�������ʱ����
		char *temp;//���·������ʱ�ַ���ָ�� 
	public:
		index()
		{
			char p[13] = "E:\\Folder\\";
	        char t[20] = "E:\\Folder\\";
	        char a[5] = ".txt";
	        char n[4] = "";
	        strcpy(path,p);
	        strcpy(append,a);
	        strcpy(num,n);//��ʼ������·������ 
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
			char *temp_path[5];//��ʱ�洢�ļ����·��
			for(int j = 0 ; j < 5 ; j++)
			{
				temp_path[j] = NULL;
			}
			if(s == 0)//�������� size == 0���������ó����������һ����С 
			{
				size =rand()%80 + 20;//������� 2kB - 10kB ���ļ���� 
			}
			else
			{
				size = s;
			}
			int i = 0 ;//��ſհ�������λ�� 
			for(int j = 0 ; j < 4 ; j++)//���ļ�����ʱ�洢
			{
				num[j] = *(ch+j);
		    }
			for(i = 0 ; i < 55 ; i++)//Ѱ�ҿհ�����ָ���λ�� 
			{
				if(ind[i] == NULL)//����ҵ��հ�����ָ�룬��ô���Ӧ���ļ���ҲӦ���ǿհ׵� 
				{
					break;
				}
			}
			if(i != 55)//����ҵ��հ�����ָ�룬�����ļ� 
			{
				name[i] = ch;//���ļ���
				int j = 0;
				for(int t_size = size ; t_size > 0 ; t_size-=20)//���ļ�ʣ����� > 0 ʱ������Ѱ����һ��·�� 
			    {
			    	temp = new char[20];
					*temp = '\0';
				    strcpy(temp,path);//��ʼ���м������м����
			        strcat(temp,b.g_blank());//���հ׿��·�������м����
			        strcat(temp,"\\"); 
			        strcat(temp,ch);//���ļ��������м���� 
			        strcat(temp,append);
			        temp_path[j] = temp;//�����ɵĵ���·���������·������ʱ���� 
			        j++;
			    }
			    file *f= new file(ch,size,temp_path);//�����ļ� i�ڵ� 
			    ind[i] = f;//�洢�ļ� i�ڵ�������
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
			for(i = 0 ; i < 55 ; i++)//ѭ��������Ҫɾ�����ļ������ڵ�λ�� 
			{
				if(name[i])//if(name[i] != NULL) 
				{
					if(!strcmp(name[i],s))//if name[i] == s
				    {
			    		ind[i]->destruction(b);//ɾ���ļ� 
			    		delete[] ind[i];//ɾ���ļ� i_node 
			    		ind[i] = NULL;//�����ÿ� 
			    		name[i] = NULL;
			    		break;
			    	}
				}
			}
			if(i == 55)//���ѭ��������ִ�� 55 �Σ�����û���ҵ���Ӧ�ļ������ļ����׳�������Ϣ 
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
			for(i = 0 ; i < 55 ; i++)//ѭ�������ļ������ڵ�λ�� 
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
			if(i == 55)//���ѭ��������ִ�� 55 �Σ�����û���ҵ���Ӧ�ļ������ļ����׳�������Ϣ 
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
					//delete[] name[i];    //����Ϊɶ���У����� 
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
