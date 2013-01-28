#include<iostream>
#include<time.h>
#include<map>

#include<iomanip>
#include "struct.h"
#include "conf.h"
#include "init.h"
#include "others.h"
using namespace std;
//1.��ȡ�ļ���2000*512B=1000K�ļ�ϵͳ

/*2.��ʼ��
		2.1 0#
		2.2 1# 51#
		2.3
*/
void init()
{
	
	fp=fopen("1.txt","r+b");
	if(!fp)
	{
		FILE *fp1=fopen("1.txt","w+b");
		Fnode *f_system=(Fnode *)filesystem;
		f_system->fsname = "zjutsystem";
		f_system->fssize = 512*2*1024;
		f_system->empty = 512*2*1024;
		f_system->usersnum = 2;
		int i,j;
		//��ʼ��bitmapλʾͼ
		for(i=0; i<23;i++)
		{
			f_system->bitmap[i]=1;
		}
		for(i=23; i<512;i++)
		{
			f_system->bitmap[i]=0;
		}
		///��ʼ���ļ������̿�ջ
		/*
			0-2023
			
			0 1-64 65-87 88-2047 = 10*196 ����23���̶��̿�
		*/
		for(i=88,j=0; i<98; i++)
		{
			f_system->blockstack[j++]=i;
		}
		
		//����һЩ��Ҫ���ļ�,��Щ�ļ���/ /bin /etc /etc/passwd /etc/etc/shadow /home
		init_dir();
	
		fwrite(filesystem,sizeof(char),512*2*1024,fp1);
		fclose(fp1);
		cout<<"���³�ʼ������,ԭ�������Ѷ�ʧ\n";
		fp=fopen("1.txt","r+b");
		init_block_stack();
		
	}
	else
	{
		cout<<"��ʼ������ *_*\n";
		init_block_stack();
		
	
	}
}

/*
	sync()
	���ڴ��е�����д�ش���
*/
void sync()
{
	delete [] files;
	delete [] filesystem;
	delete [] inodeblock;
	delete [] blockstack;
	delete [] directory;
	
	fclose(fp);
}

/*3.�û���¼
		3.1 ��ȡ/etc/passwd �ж��Ƿ��������û�����������ʾû�д��û�
		3.2 ��ȡ/etc/shadow �ж������Ƿ���ȷ
		3.2 �����û��������û���Ŀ¼
			3.2.1 root��¼ ����/home/root Ȼ���ȡ/home/root/.bashrc �ļ� ����֪��root ��shell ϵͳ��ʾ��ͳһȱʡΪ"#"��
			3.2.2 suzie��¼ ����/home/suzie Ȼ��/home/suzie/.bashrc ȱʡΪ"$",��B Shell��Bourne-again Shell��ȱʡ��ʾ��Ϊ"$"
*/
int login()
{
	string username;
	string password;
	int i;
	cout<<"Welcome to use zjut's filesystem!\n";
	while(1)
	{	
		cout<<"Login as: ";
		cin>>username;
		cout<<username<<"@zjut's password: ";
		cin>>password;

		fseek(fp,0,SEEK_SET);
		fread(files,sizeof(char),512,fp);
		Fnode *f_system =(Fnode *)files;
		int user_num =f_system->usersnum;

		for(i=0; i<user_num; i++)
		{
			fseek(fp,512*68+64*i,SEEK_SET);
		///���ﲻ�ܶ�filesystem���в��������߱��ٳ�ʼ��֮���filesystem���в���
			fread(files,sizeof(User),1,fp);
			User *user =(User *)files;
			if(user->username==username && user->password==password)
			{
			//PWD = "/home/"+USERNAME;
				PWD = user->pwd;
				cout<<"welcome!\n";
				USERNAME = user->username;
				GROUPID = user->group_id;
				USERID = user->user_id;
				if(USERNAME == "root")
				{
					PS1="#";
				}
				df();
				time_t b=time(0);
				cout<<"�㵱ǰ��¼ʱ��Ϊ:"<<ctime(&b)<<endl;
				return 0;
			}
			else
			{
				
			}
		}
		cout<<"�������\n";

	}
}
/*
	���ļ�ϵͳ��ֵ
*/
void ff()
{
	Fnode *f_system=(Fnode *)filesystem;
	f_system->fsname = "zjutsystem";
	f_system->fssize = 512*2000;
	f_system->empty = 512*2000;
	fwrite(filesystem,sizeof(char),512*2000,fp);
}
/*
	df[display filesystem]
	��ʾ�ļ�ϵͳ����ϸ��Ϣ
*/
void df()
{
	 
	//FILE *fp2 = fopen("1.txt","r");
	cin.clear();
	///һ��Ϊfreadǰ�����fseek
	fseek(fp,0,SEEK_SET); 
	fread(files,sizeof(char),512,fp);
	Fnode *f_system=(Fnode *)files;
	cout<<"�ļ�ϵͳ����:"<<f_system->fsname<<endl;
	cout<<"�ļ�ϵͳ��С:"<<f_system->fssize<<endl;
	cout<<"�ļ������̿���:"<<f_system->empty<<endl;
	 
	//fclose(fp2);
}
/*
	int get_inode()
	�����ȡinode�ڵ�ţ�ƫ�Ƶ�ַ���ݽڵ�������

*/
int  get_inode()
{
	 fseek(fp,0,SEEK_SET); 
	 fread(files,sizeof(char),512,fp);
	 Fnode *f_system=(Fnode *)files;
	 //cout<<sizeof(Fnode);
	 int i,j;
	 //f_system->bitmap[0]=1;
	 for(i=0; i<64; i++)
	 {
		//cout<<"λʾͼ��"<<i<<"��:";
		for(j=0;j<8;j++)
		{
			if(f_system->bitmap[i*8+j] == 0)
			{
				
				f_system->bitmap[i*8+j] = 1;
				fseek(fp,0,SEEK_SET); 
				fwrite(files,sizeof(char),512,fp);
				return i*8+j;
			}
			
		}
		
	 }
	 return -1;
	 
}
/*
	��ʾ��ǰinode�ڵ�����
*/
void di(int num =64)
{
	 fseek(fp,0,SEEK_SET); 
	 fread(files,sizeof(char),512,fp);
	 Fnode *f_system=(Fnode *)files;
	 //cout<<sizeof(Fnode);
	 int i,j;
	 //f_system->bitmap[0]=1;
	 cout<<"inode�ڵ�����:\n";
	 for(i=0; i<num; i++)
	 {
		cout<<"λʾͼ��"<<i<<"��:";
		for(j=0;j<8;j++)
		{
			cout<<f_system->bitmap[i*8+j];	
		}
		cout<<"\n";
	 }
	 //fclose(fp);
	
	 
}

////������̿�
int get_block()
{
	int block_num;
	int temp,i,j;
	if(block_stack.size() > 1)
	{
		block_num = block_stack.top();
		block_stack.pop();
		return block_num;

	}
	///��ջʣ��ֻ��1����ʱ��
	if(block_stack.size() == 1)
	{
		fseek(fp,512*block_stack.top(),SEEK_SET);
		fread(blockstack,sizeof(BlockStack),1,fp);
		BlockStack *bs = (BlockStack *)blockstack;
		if(bs->blockstack[0] < 0)
		{
			
			temp =block_stack.top();
			block_stack.pop();
			for(i=0,j=temp+10;i<10;i++)
			{
				block_stack.push(j--);
			}
			return temp;

		}
		else
		{
		
			temp =block_stack.top();
			block_stack.pop();
			for(i=0;i<10;i++)
			{
				block_stack.push(bs->blockstack[i]);
			}
			return temp;
		}
	}
	///��ʾȫ�������̿��Ѿ�ȫ�����������
	return -1;
}
/*
	void db()
	��ʾ�����̿�ջ�����
*/
void dbs()
{
	cout<<"Ŀǰ�Ŀ����̿�ջ�����: ";
	while(!block_stack.empty())
	{
		cout<<block_stack.top()<<" ";
		block_stack.pop();
	}
	cout<<endl;
}
/*
	��ʼ�������̿�ջ
*/
void init_block_stack()
{
	cin.clear();
	fseek(fp,0,SEEK_SET);	
	fread(files,sizeof(char),512,fp);
	Fnode *f_system=(Fnode *)files;
	int i;
	for(i=9;i>=0;i--)
	{
		if(f_system->blockstack[i] != 0)
		{
		//	cout<<f_system->blockstack[i];
			block_stack.push(f_system->blockstack[i]);
		}
	}
}
/*
	flush�����̿�ջ���ļ����������ʾ
*/
void exit_block_stack()
{
	fseek(fp,0,SEEK_SET);	
	fread(files,sizeof(char),512,fp);
	Fnode *f_system=(Fnode *)files;
	int i,temp = 0;
	while(!block_stack.empty())
	{
		f_system->blockstack[temp]=block_stack.top();
		//cout<<block_stack.top()<<" ";
		block_stack.pop();
		temp++;
	}
	for(i=temp;i<10;i++)
	{
		f_system->blockstack[i] =0;
	}
	/*for(i=0;i<10;i++)
	{
		cout<<f_system->blockstack[i]<<" ";
	}*/
//	cout<<f_system->fsname<<endl;
	fseek(fp,0,SEEK_SET);
	fwrite(f_system,sizeof(Fnode),1,fp);
}
/*
	ls
	�г��ļ�Ŀ¼
*/
void ls(int inodeid,string filename="/")
{
	Inode *inode;

	fseek(fp,512+64*23,SEEK_SET);
	fread(inodeblock,sizeof(Inode),1,fp);
	inode =(Inode *)inodeblock;
	//cout<<inode->blocknum<<" "<<inode->filesize<<endl;

	cin.clear();
	fseek(fp,512+inodeid*64,SEEK_SET);
	fread(inodeblock,sizeof(Inode),1,fp);
	inode=(Inode *)(inodeblock);
	cout<<"Ȩ��      inode  links �̿��С  �û�  �û���                      �޸�ʱ��           �ļ���\n";
	int i,ds,blockaddress;
	Directory *dir;
	ds = inode->ds;
	blockaddress =inode->blockaddress[0];
/*	*/
	if(inode->permissions[0] == 'd')
	for(i=0; i<ds;i++)
	{
		fseek(fp,512*blockaddress+i*32,SEEK_SET);
		fread(directory,sizeof(char),32,fp);
		dir=(Directory *)(directory);
		//cout<<dir->inode_id<<endl;
		char *xx ="";
		//cout<<dir->parentfilename<<dir->parent_id<<endl;
		if(strcmp(dir->parentfilename,xx) !=0 || dir->parent_id < 0)
		{
		fseek(fp,512+64*dir->inode_id,SEEK_SET);
		//fseek(fp,512+64*23,SEEK_SET);
		fread(inodeblock,sizeof(Inode),1,fp);
		inode=(Inode *)(inodeblock);
		
		cout<<setw(10)<<inode->permissions;
		char tempchar=inode->permissions[0];
		int tempid =inode->ds;
		cout<<setw(4)<<inode->inode_id;
		cout<<setw(4)<<inode->links;
		
		cout<<setw(10)<<inode->filesize<<"B";
		cout<<setw(10)<<get_username(inode->user_id);
		cout<<setw(8)<<get_groupname(inode->group_id);
		
		//cout<<"  "<<inode->blocknum;
		get_time(inode->mtime);
		if(tempchar !='l')
		{
			cout<<setw(15)<<dir->filename;
			
		}
		else
		{
			cout<<setw(15)<<dir->filename<<" --> "<<dir->parentfilename;
			
		}
		cout<<endl;
		}
	}
	else if(inode->permissions[0] == '-')
	{
		cout<<setw(10)<<inode->permissions;
		cout<<setw(4)<<inode->inode_id;
		

		cout<<setw(8)<<get_username(inode->user_id);
		cout<<setw(8)<<get_groupname(inode->group_id);
		cout<<setw(10)<<inode->filesize<<"B";

		get_time(inode->mtime);

		
		cout<<setw(15)<<filename;
		cout<<endl;
	}
	
	cout<<endl;
}
/*
	�������ַ���inode�ڵ��id
*/
int findinode(string filename)
{
	

	//����/��ͷ���Ǿ���·��
	//cout<<filename<<endl;
	if(filename.find_first_of("/") == 0)
	{
		if(filename == "/")
			return 0;
		int position=0;
		int temp=1;
		int num=0;
		int inodeid=0;
		int i,ds,blockaddress;
		Directory *dir;
		
		string tempfile;
		while((position=filename.find_first_of("/",position))!=string::npos)  
		{
			//ѡ�񵽵����ĸ���
			fseek(fp,512+inodeid*64,SEEK_SET);
			fread(inodeblock,sizeof(Inode),1,fp);
			Inode *inode=(Inode *)(inodeblock);
			
			//cout<<inode->permissions[0]<<endl;
			if(inode->permissions[0] != 'd')
			{
				cout<<"����Ŀ¼d\n";
				return -2;
			}
			ds = inode->ds;
			blockaddress =inode->blockaddress[0];
			//cout<<ds<<endl;
			if(num == 0)
			{
				
			}
			else
			{
				//cout<<filename.substr(temp+1,position-temp-1)<<endl;
				tempfile=filename.substr(temp+1,position-temp-1);
				//cout<<ds;
				for(i=0; i<ds;i++)
				{
					fseek(fp,512*blockaddress+i*32,SEEK_SET);
					fread(directory,sizeof(char),32,fp);
					dir=(Directory *)(directory);
				//	cout<<directory->filename<<endl;
					if(dir->filename == tempfile)
					{
					
						inodeid = dir->inode_id;
						//cout<<inodeid<<endl;
						break;
					}
				//	cout<<"û�ҵ��˲�"<<tempfile<<"aaa�м�Ŀ¼"<<endl;
				//	return -1;					
				}
				
			}
			temp=position;
			position++;
			num++;
		}
		//cout<<filename.substr(temp+1,filename.length()-temp-1)<<endl;
		tempfile = filename.substr(temp+1,filename.length()-temp-1);
		//cout<<inodeid<<endl;
		fseek(fp,512+inodeid*64,SEEK_SET);
		fread(inodeblock,sizeof(Inode),1,fp);
		Inode *inode=(Inode *)(inodeblock);		
		//cout<<inode->inode_id<<endl;
		//cout<<inode->permissions[0]<<endl;
		if(inode->permissions[0] != 'd')
		{
				cout<<"����Ŀ¼d\n";
				return -2;
		}
		ds = inode->ds;
		blockaddress =inode->blockaddress[0];
	//	cout<<ds<<endl;
		
		for(i=0; i<ds;i++)
		{
			fseek(fp,512*blockaddress+i*32,SEEK_SET);
			
			//fseek(fp,512*69,SEEK_SET);
			fread(directory,sizeof(char),32,fp);
			dir=(Directory *)(directory);
		//	cout<<directory->filename<<endl;
			if(dir->filename == tempfile)
			{
					
				inodeid = dir->inode_id;
				//cout<<"�ҵ���"<<inodeid<<directory->filename<<endl;
				return inodeid;
				break;
			}				
		}
		//cout<<"û�ҵ���"<<tempfile<<"Ŀ¼"<<endl;
		return -1;	
	}
	if(filename.find_first_of("/") ==string::npos)
	{
		string temp = PWD+"/"+filename;
		//cout<<temp<<endl;
		return findinode(temp);
	}
	return -1;
}
void pwd()
{
	cout<<PWD<<endl;
}
//�鿴�ļ�����
int cat(int inodeid)
{
	/*
		0.���ж���ľ��r��Ȩ��
	*/
//	cout<<get_auth(inodeid,4)<<endl;
	if(get_auth(inodeid,4) != 0)
	{
		cout<<"Permission Limited!! ��ľ��Ȩ�޲鿴����ļ�"<<endl;
		return -1;
	}
	


	/*�ж������ӵ�Դ�ļ��Ƿ����*/
	
	fseek(fp,512+64*inodeid,SEEK_SET);
	fread(inodeblock,sizeof(Inode),1,fp);
	Inode *inode=(Inode *)(inodeblock);
	if(inode->inode_id == -2)
	{
		cout<<"�ļ�������"<<endl;
		return -1;
	}
	if(inode->permissions[0] == 'l')
	{
	//	cout<<inode->ds<<endl;
	//	cout<<isInode(inode->ds);
		if(isInode(inode->ds) == 0)
		{
			cout<<"Դ�ļ��Ѿ���ɾ����\n";
			return -2;
		}
		
		fseek(fp,512+64*inode->ds,SEEK_SET);
		fread(tempinodeblock,sizeof(Inode),1,fp);
		inode=(Inode *)(tempinodeblock);
		//cout<<inode->inode_id<<endl;
		//cout<<inode->permissions[0]<<endl;
		if(inode->permissions[0]=='x')
		{
			cout<<"Դ�ļ��Ѿ���ɾ����\n";
			return -2;
		}

	}
	fseek(fp,512+64*inodeid,SEEK_SET);
	fread(inodeblock,sizeof(Inode),1,fp);
	inode=(Inode *)(inodeblock);
	if(isInode(inode->inode_id)==0)
	{
		cout<<"ľ������ļ�\n";
		return -1;
	}
	//cout<<inode->blockaddress[2]<<endl;
	//cout<<inode->blocknum<<endl;
	File *file;
	int i,j;
	int needblocknum =inode->blocknum;
	//ֻ��ֱ�ӵ�ַ��
	if(inode->blocknum <=4)
	{
		for(i=0; i<inode->blocknum; i++)
		{
			cout<<inode->blockaddress[i]<<" : ";
			fseek(fp,512*inode->blockaddress[i],SEEK_SET);
			fread(files,sizeof(File),1,fp);
			file =(File *)files;
			for(j=0; j<512;j++)
			{
				cout<<file->filechar[j];
			}
			cout<<endl;
		}
	}
	//�õ�һ�ε�
	int index1,temp2;
	if(inode->blocknum <=128+4 && inode->blocknum >4)
	{
		for(i=0; i<4; i++)
		{
			cout<<inode->blockaddress[i]<<" : ";
			fseek(fp,512*inode->blockaddress[i],SEEK_SET);
			fread(files,sizeof(File),1,fp);
			file =(File *)files;
			for(j=0; j<512;j++)
			{
				cout<<file->filechar[j];
			}
			cout<<endl;
		}
		index1=inode->blockaddress[4];
		temp2=inode->blocknum-4;
		fseek(fp,512*index1,SEEK_SET);
		fread(index,sizeof(Index),1,fp);
		Index *index1 =(Index *)index;
		for(i=0; i<temp2; i++)
		{
			cout<<index1->blockaddress[i]<<" : ";
			fseek(fp,512*index1->blockaddress[i],SEEK_SET);			
			fread(files,sizeof(File),1,fp);
			file =(File *)files;
			for(j=0; j<512;j++)
			{
				cout<<file->filechar[j];
			}
			cout<<endl;
		}

	}
	//��ȡ�õ�2�μ�ַ���ļ�
	int index_first;
	if(inode->blocknum <=128*128+128+4 && inode->blocknum >4+128)
	{
		for(i=0; i<4; i++)
		{
			cout<<inode->blockaddress[i]<<" : ";
			fseek(fp,512*inode->blockaddress[i],SEEK_SET);
			fread(files,sizeof(File),1,fp);
			file =(File *)files;
			for(j=0; j<512;j++)
			{
				cout<<file->filechar[j];
			}
			cout<<endl;
		}
		index_first=inode->blockaddress[4];
	//	temp2=inode->blocknum-4;
		fseek(fp,512*index_first,SEEK_SET);
		fread(index,sizeof(Index),1,fp);
		Index *index1 =(Index *)index;
		for(i=0; i<128; i++)
		{
			cout<<index1->blockaddress[i]<<" : ";
			fseek(fp,512*index1->blockaddress[i],SEEK_SET);			
			fread(files,sizeof(File),1,fp);
			file =(File *)files;
			for(j=0; j<512;j++)
			{
				cout<<file->filechar[j];
			}
			cout<<endl;
		}
		//2��������ȡ
		//2.1�ȶ���һ��
		int index_first = inode->blockaddress[5];
		fseek(fp,512*index_first,SEEK_SET);
		fread(files,sizeof(Index),1,fp);
		index1 =(Index *)files;
		
		int temp2 = needblocknum-128-4;
		int temp3 =	(temp2-1)/128+1;			//��Ҫ2���������̿���

		Index *index2;
		int k,h=0;
		for(j=0;j<temp3-1; j++)
		{			
			fseek(fp,index1->blockaddress[j]*512,SEEK_SET);
			fread(index,sizeof(Index),1,fp);
			index2 =(Index *)index;
			for(i=0;i<128;i++)
			{
				cout<<index2->blockaddress[i]<<" : ";
				fseek(fp,512*index2->blockaddress[i],SEEK_SET);			
				fread(files,sizeof(File),1,fp);
				file =(File *)files;
				for(k=0; k<512;k++)
				{
					cout<<file->filechar[k];
				}
				cout<<endl;
			}
			h++;
			
		}
		fseek(fp,index1->blockaddress[temp3-1]*512,SEEK_SET);
		fread(index,sizeof(Index),1,fp);
		index2 =(Index *)index;
		for(i=0;i<temp2-h*128;i++)
		{
			cout<<index2->blockaddress[i]<<" : ";
			fseek(fp,512*index2->blockaddress[i],SEEK_SET);			
			fread(files,sizeof(File),1,fp);
			file =(File *)files;
			for(k=0; k<512;k++)
			{
				cout<<file->filechar[k];
			}
			cout<<endl;
		}
		///�и�ľ�����������
		cout<<"׼��"<<endl;

		


	}
	cout<<endl;

}
int passwd(string username)
{
	cout<<"�޸��ļ����뿩"<<endl;
	int is = isUser(username);	
	if(is >=0 )
	{
		fseek(fp,512*68+is*64,SEEK_SET);
		///���ﲻ�ܶ�filesystem���в��������߱��ٳ�ʼ��֮���filesystem���в���
		fread(files,sizeof(User),1,fp);
		User *user =(User *)files;
	
		string passwd;
		string passwd2;
		while(1)
		{
			
			cout<<"�������µ�passwd:";
			cin>>passwd;
			cout<<"���ٴ��µ�passwd:";
			cin>>passwd2;
			if(passwd2 == passwd)
			{	
				break;
			}
			else
			{
				cout<<"�������2�����벻һ��"<<endl;
			}
		}
		strcpy(user->password,passwd.c_str());
		fseek(fp,512*68+is*64,SEEK_SET);
		fwrite(user,sizeof(User),1,fp);
		return 0;
	}
	else
	{
		//cout<<"����������û�"<<endl;
		return -1;
	}
	
	
}
/*
	int Isdir(int inodeid)
	����inodeid
	����ֵ
		0 ����Ŀ¼
		-1 �Ǹ�Ŀ¼
*/
int Isdir(int inodeid)
{
	fseek(fp,512+64*inodeid,SEEK_SET);
	fread(inodeblock,sizeof(Inode),1,fp);
	Inode *inode=(Inode *)(inodeblock);
	if(inode->permissions[0] == 'd')
	{
		return -1;
	}
	return 0;
}
/*
	mkdir�����ļ�Ŀ¼
*/
int mkdir(string dirname)
{
	//0.����֤��ľ��Ȩ�޴���Ŀ¼��
	int pwdinode =findinode(PWD);
	if(get_auth(pwdinode,2) != 0)
	{
		cout<<"Permission Limited!! ��ľ��Ȩ�޵�ǰĿ¼"<<PWD<<"�ڴ����ļ�"<<endl;
		return -1;
	}
	//0.5
	//0.5
	int same =samefile(dirname);
	if(same == -1)
	{
		cout<<"��ֹ����ͬ��Ŀ¼\n";
		return -1;
	}

	//1.������ľ��inode���Է������
	int inodeid = get_inode();
	if(inodeid >= 0)
	{
		//cout<<"ż�������˵�"<<inodeid<<"��inode"<<endl;
	}
	else
	{
		cout<<"inode�ڵ��Ѿ�����\n";
		return -1;
	}
	//2.�ҿ����̿�
	int block_num = get_block();
	if(block_num >= 0)
	{
		//cout<<"ż�������˵�"<<block_num<<"���̿�"<<endl;
	}
	else
	{
		cout<<"�ļ�ϵͳ�Ѿ������ˣ�ľ��ʣ��Ŀ����̿���\n";
		return -1;
	}
	

	//3.inode�ڵ���д��
//	fseek(fp,512+64*inodeid,SEEK_SET);
//	fread(inodeblock,sizeof(Inode),1,fp);
	Inode *inode =(Inode *)inodeblock;
	inode->inode_id = inodeid;							//��д��ǰ�����inodeid
	inode->group_id = USERID;							//��д��ǰ�û���groupid,�ðɣ�����Ҫ��ȡ��ǰ
	inode->user_id = GROUPID;
	string permissions="d"+UMASK;
	//char *per = "dr--------";
	strcpy(inode->permissions,permissions.c_str());
//	cout<<inode->permissions<<endl;
	inode->ds=0;
	inode->filesize =512;
	inode->blockaddress[0]=block_num;
	inode->blocknum=1;
	inode->links=1;
	inode->mtime = time(0);
	fseek(fp,512+64*inodeid,SEEK_SET);
	fwrite(inode,sizeof(Inode),1,fp);

	fseek(fp,512+64*inodeid,SEEK_SET);
	fread(inodeblock,sizeof(Inode),1,fp);
	inode =(Inode *)inodeblock;
	//cout<<inode->permissions<<endl;
	//4.��ǰinode�ڵ��ָ���Ŀ¼�̿飬�������ӵ�Ŀ¼
//	cout<<PWD<<endl;
	int pwdinodeid = findinode(PWD);
	fseek(fp,512+64*pwdinodeid,SEEK_SET);
	fread(inodeblock,sizeof(Inode),1,fp);
	inode =(Inode *)inodeblock;
	
	int parentinodeid = inode->inode_id;
	int parentaddress = inode->blockaddress[0];
	int ds = inode->ds;

	inode->ds++;
	fseek(fp,512+64*pwdinodeid,SEEK_SET);
	fwrite(inodeblock,sizeof(Inode),1,fp);

//	cout<<parentaddress<<endl;
//	cout<<ds<<endl;
	fseek(fp,512*parentaddress+32*ds,SEEK_SET);
	fread(directory,sizeof(Directory),1,fp);
	Directory *dir = (Directory *)directory;

	strcpy(dir->filename,dirname.c_str());
	dir->inode_id = inodeid;
	dir->parent_id =parentinodeid;
	filename = "suzie";
	strcpy(dir->parentfilename,filename);
	fseek(fp,512*parentaddress+32*ds,SEEK_SET);
	fwrite(dir,sizeof(Directory),1,fp);

	return 0;
}
/*
	���տ����̿�ջ
*/
void return_block(int blockaddress)
{
	//2�����
	if(block_stack.size() == 10)
	{
		
		BlockStack *bs =(BlockStack *)(blockstack);
		for(int i=9; i>=0;i--)
		{
			bs->blockstack[i]=block_stack.top();
			block_stack.pop();
		}
		fseek(fp,512*blockaddress,SEEK_SET);
		fwrite(bs,sizeof(BlockStack),1,fp);
		block_stack.push(blockaddress);
	}
	else
	{
		block_stack.push(blockaddress);
	}
	
}
/*
	����inode�ڵ�
*/
void return_inode(int inodeid)
{
	fseek(fp,0,SEEK_SET); 
	fread(files,sizeof(char),512,fp);
	Fnode *f_system=(Fnode *)files;
	//cout<<sizeof(Fnode);
	f_system->bitmap[inodeid]=0;
	fseek(fp,0,SEEK_SET); 
	fwrite(f_system,sizeof(char),512,fp);
}
/*
	rmdir(string filename)
	ɾ���ļ�Ŀ¼
*/
int rmdir(string filename)
{
	/*
		0.���ж���ľ��w��Ȩ��
	*/
	int inodeid = findinode(filename);
	if(get_auth(inodeid,2) != 0)
	{
		cout<<"Permission Limited!! ��ľ��Ȩ��ɾ�����Ŀ¼"<<endl;
		return -1;
	}
	int tempid =inodeid;
//	cout<<"��ɾ��Ŀ¼"<<filename<<"��inodeΪ:"<<inodeid<<endl;
	
	fseek(fp,512+64*inodeid,SEEK_SET);
	fread(inodeblock,sizeof(Inode),1,fp);
	Inode *inode=(Inode *)(inodeblock);
	//cout<<inode->blockaddress[0]<<endl;
	return_inode(tempid);
	return_block(inode->blockaddress[0]);
	
	inodeid = findinode(PWD);
//	cout<<"��ǰĿ¼��inode"<<inodeid<<endl;
	fseek(fp,512+64*inodeid,SEEK_SET);
	fread(inodeblock,sizeof(Inode),1,fp);
	inode=(Inode *)(inodeblock);
//	cout<<"��ǰĿ¼�̿�ĵ�ַ"<<endl;
//	cout<<inode->blockaddress[0]<<endl;
//	cout<<inode->ds<<endl;
	int ds = inode->ds;
	int blockaddress = inode->blockaddress[0];
	int id;
	for(int i=0 ;i<ds;i++)
	{
		fseek(fp,512*blockaddress+32*i,SEEK_SET);
		fread(directory,sizeof(Directory),1,fp);
		Directory *dir =(Directory *)directory;
		
		//cout<<dir->parentfilename<<endl;
		id =dir->inode_id;
	//	cout<<dir->filename<<"��inode:"<<id<<endl;
		if(id == tempid)
		{
			char *parname ="";
			strcpy(dir->parentfilename,parname);
		//	cout<<dir->parentfilename<<endl;
		//	cout<<"��������"<<endl;
			fseek(fp,512*blockaddress+32*i,SEEK_SET);
			fwrite(dir,sizeof(Directory),1,fp);
		}
		
	}

}
/*
	cd�л��ļ�Ŀ¼
*/
void cd(string shell)
{
	int inodeid;
	int isdir;
	string temp;
	temp = shell.substr(shell.find_first_of(" ")+1,shell.length()-shell.find_first_of(" "));
	if(shell.find_first_of(" ") == string::npos)
	{
		 temp ="/home/" + USERNAME;
		 PWD =temp;
	}
	else if(temp.find_first_of("..") == string::npos && temp.find_first_of("~") == string::npos)
	{
		inodeid = findinode(temp);
		if(temp.find_first_of("/") == 0)
		{
					
					//cout<<inodeid<<endl;
			if(inodeid >= 0)
			{
				isdir = Isdir(inodeid);
				if(isdir == -1)
				{
					if(get_auth(inodeid,1) == 0)
						PWD = temp;
					else
						cout<<"Permission Limited!!!"<<endl;
				}
				else
				{
					cout<<temp<<"����Ŀ¼"<<endl;
				}
			}
			else
			{
				cout<<"ľ���ҵ�"<<temp<<"���Ŀ¼"<<endl;
			}
		}
		else
		{
			if(PWD !="/")
				temp = PWD+"/"+temp;
			else
			{
				temp = PWD+temp;
			}
			inodeid = findinode(temp);
			isdir = Isdir(inodeid);
			if(isdir == -1)
			{
				if(get_auth(inodeid,1) == 0)
					PWD = temp;
				else
					cout<<"Permission Limited!!!"<<endl;
			}
			else
			{
				cout<<temp<<"����Ŀ¼"<<endl;
			}
		}


	}
	else if(temp.find_first_of("~/") != 0)
	{
		temp = shell.substr(shell.find_first_of("..")+2,shell.length()-shell.find_first_of(".."));
		
		string parenttemp = PWD.substr(0,PWD.find_last_of("/"));
		temp = parenttemp + temp;

		inodeid = findinode(temp);
		isdir = Isdir(inodeid);
		if(isdir == -1)
		{
			PWD = temp;
		}
		else
		{
			cout<<temp<<"����Ŀ¼."<<endl;
		}
		//cout<<temp<<endl;
		//cout<< parenttemp<<endl;
		//inodeid=findinode(PWD);                                                                                                                                                                                                                                                                                                                          
	}
	else
	{
		//temp = shell.substr(shell.find_first_of(" ")+1,shell.length()-shell.find_first_of(" "));
		//cout<<PWD<<endl;
		temp = temp.substr(1,temp.length());
		temp = "/home/"+USERNAME+temp;
		//cout<<temp<<endl;
		inodeid = findinode(temp);
		isdir = Isdir(inodeid);
		if(isdir == -1)
		{
			PWD = temp;
		}
		else
		{
			cout<<temp<<"����Ŀ¼."<<endl;
		}
	}
}
/*
	chmod
	�ı��ļ�Ȩ��
	�������
	����ֵ	0	�޸ĳɹ�
			-1	�޸�ʧ��
 */
int chmod(string permissions,int inodeid)
{
	//cout<<permissions<<endl;
	string per = num2char(permissions);
	//cout<<inodeid<<endl;
	//cout<<per<<endl;
	if(per == "-1")
	{
		return -1;
	}
	fseek(fp,512+64*inodeid,SEEK_SET);
	fread(inodeblock,sizeof(Inode),1,fp);
	Inode *inode = (Inode *)inodeblock;

	per = inode->permissions[0]+per;
	strcpy(inode->permissions,per.c_str());

	fseek(fp,512+64*inodeid,SEEK_SET);
	fwrite(inode,sizeof(Inode),1,fp);

	return 0;
}
/*
	chown
	�ı��ļ�������
*/
int chown(int userid,int inodeid)
{
	fseek(fp,512+64*inodeid,SEEK_SET);
	fread(inodeblock,sizeof(Inode),1,fp);
	Inode *inode = (Inode *)inodeblock;
//	cout<<"ԭ����"<<inode->user_id<<endl;
//	cout<<"�޸ĵ�"<<userid<<endl;
	inode->user_id = userid;
	fseek(fp,512+64*inodeid,SEEK_SET);
	fwrite(inode,sizeof(Inode),1,fp);
	return 0;
}
/*
	chgrp
	�ı��ļ�������
*/
int chgrp(int groupid,int inodeid)
{
	fseek(fp,512+64*inodeid,SEEK_SET);
	fread(inodeblock,sizeof(Inode),1,fp);
	Inode *inode = (Inode *)inodeblock;
//	cout<<"ԭ����"<<inode->group_id<<endl;
//	cout<<"�޸ĵ�"<<groupid<<endl;
	inode->group_id = groupid;
	fseek(fp,512+64*inodeid,SEEK_SET);
	fwrite(inode,sizeof(Inode),1,fp);
	return 0;
}
/*
	mv �ı��ļ�����
*/
int mv(int inodeid,string newfilename)
{
	int tempid =inodeid;

	inodeid = findinode(PWD);
	fseek(fp,512+64*inodeid,SEEK_SET);
	fread(inodeblock,sizeof(Inode),1,fp);
	Inode *inode=(Inode *)(inodeblock);

	int ds = inode->ds;
	int blockaddress = inode->blockaddress[0];
	int id;
	for(int i=0 ;i<ds;i++)
	{
		fseek(fp,512*blockaddress+32*i,SEEK_SET);
		fread(directory,sizeof(Directory),1,fp);
		Directory *dir =(Directory *)directory;
		
		//cout<<dir->parentfilename<<endl;
		id =dir->inode_id;
		if(id == tempid)
		{
			//char *parname ="";
			strcpy(dir->filename,newfilename.c_str());
			fseek(fp,512*blockaddress+32*i,SEEK_SET);
			fwrite(dir,sizeof(Directory),1,fp);
		}
		
	}
	return 0;
}
/*
	mk
	�����ļ�
	������� filename�ļ����� filesize�ļ���С filechar�ļ�����ַ�
	����ֵ
*/
int mk(string filename,int filesize,char filechar)
{
	
	//0.����֤��ľ��Ȩ�޴���Ŀ¼��
	int pwdinode =findinode(PWD);
	if(get_auth(pwdinode,2) != 0)
	{
		cout<<"Permission Limited!! ��ľ��Ȩ�޵�ǰĿ¼"<<PWD<<"�ڴ����ļ�"<<endl;
		return -1;
	}
	//0.5
	int same =samefile(filename);
	if(same == -1)
	{
		cout<<"��ֹ����ͬ���ļ�\n";
		return -1;
	}

	//1.������ľ��inode���Է������
	int inodeid = get_inode();
	if(inodeid >= 0)
	{
		//cout<<"ż�������˵�"<<inodeid<<"��inode"<<endl;
	}
	else
	{
		cout<<"inode�ڵ��Ѿ�����\n";
		return -1;
	}

	/*2.�ҿ����̿�,����Ҫ�ȼ���filesizeת����������Ҫ���̿���
		1.blocknum = (filesize-1)/512+1;
		2.blocknum >4 ����Ҫһ�μ��Ѱַ
		3.blocknum >4+128 ����Ҫ2�μ��Ѱַ   ������Ҫ���·���һ���̿������
		4.blocknum >4+128+128*128 û�еĴ���.
	*/
	int needblocknum = (filesize-1)/512 +1;
	int i,bs;
	vector<int>block_num;
//	int block_num[10];
	for(i=0; i<needblocknum; i++)
	{
		
		bs = get_block();
		
		if(bs >= 0)
		{
			block_num.push_back(bs);
			//cout<<"ż�������˵�"<<bs<<"���̿�"<<endl;
		}
		else
		{
			cout<<"�ļ�ϵͳ�Ѿ������ˣ�ľ��ʣ��Ŀ����̿���\n";
		return -1;
		}
	}
	

	//3.inode�ڵ���д��
	Inode *inode =(Inode *)inodeblock;
	inode->inode_id = inodeid;							//��д��ǰ�����inodeid
	inode->group_id = USERID;							//��д��ǰ�û���groupid,�ðɣ�����Ҫ��ȡ��ǰ
	inode->user_id = GROUPID;
	string permissions="-"+UMASK;

	strcpy(inode->permissions,permissions.c_str());
	inode->ds=0;
	inode->filesize =filesize;
	//cout<<filesize<<endl;
	/*
		Ϊ�����׼��;
	*/
	File *tempfile =(File *)tempfiles;
	for(i=0;i<512;i++)
	{
		tempfile->filechar[i]=filechar;
	}
	//3.1 needblocknum <=4;
	if(needblocknum <=4)
	{
		for(int j=0;j<needblocknum;j++)
		{
			inode->blockaddress[j]=block_num[j];
			cout<<inode->blockaddress[j]<<" ";
			fseek(fp,512*block_num[j],SEEK_SET);
			fwrite(tempfile,sizeof(File),1,fp);
		}
	}
	//3.2 needblocknum<=128+4 &&needblocknum>4;
	int index_first,j;
	if(needblocknum <=128+4 && needblocknum >4)
	{
		for(j=0;j<4;j++)
		{
			inode->blockaddress[j]=block_num[j];
			cout<<inode->blockaddress[j]<<" ";
			fseek(fp,512*block_num[j],SEEK_SET);
			fwrite(tempfile,sizeof(File),1,fp);
		}
		index_first = get_block();
		inode->blockaddress[4] =index_first;
		
		j=4;
		Index *index1 =(Index *)files;
		for(i=0;i<needblocknum-4; i++)
		{
			cout<<block_num[j]<<" ";
			fseek(fp,512*block_num[j],SEEK_SET);
			fwrite(tempfile,sizeof(File),1,fp);

			index1->blockaddress[i]=block_num[j++];
				
		}
		fseek(fp,512*index_first,SEEK_SET);
		fwrite(index1,sizeof(Index),1,fp);

	}
	//3.3 2�μ��Ѱַ needblocknum <4+128+128*128 && needblocknum >4+128
	int index_second,k;
	if(needblocknum <4+128+128*128 && needblocknum >4+128)
	{
		//ֱ������
		for(j=0;j<4;j++)
		{
			inode->blockaddress[j]=block_num[j];
			cout<<inode->blockaddress[j]<<" ";
			fseek(fp,512*block_num[j],SEEK_SET);
			fwrite(tempfile,sizeof(File),1,fp);
		}
		index_first = get_block();
		cout<<"һ�μ�������̿�:"<<index_first<<endl;
		inode->blockaddress[4] =index_first;
		
		//������1������
		Index *index1 =(Index *)files;
		for(i=0,j=4;i<128; i++)
		{
			cout<<block_num[j]<<" ";
			fseek(fp,512*block_num[j],SEEK_SET);
			fwrite(tempfile,sizeof(File),1,fp);
			index1->blockaddress[i]=block_num[j++];
		}
		fseek(fp,512*index_first,SEEK_SET);
		fwrite(index1,sizeof(Index),1,fp);
		
		//��ʼ����2������
		index_first = get_block();
		cout<<"һ�μ�������̿�:"<<index_first<<endl;
		inode->blockaddress[5] =index_first;	
		int temp2 = needblocknum-128-4;
		int temp3 =	(temp2-1)/128+1;			//��Ҫ2���������̿���
		index1 = (Index *)files;
		Index *index2;
		k=4+128;
		for(j=0;j<temp3-1; j++)
		{
			index_second =get_block();
			cout<<"���μ�������̿�:"<<index_second<<endl;
			index1->blockaddress[j]=index_second;
			
			index2 =(Index *)index;
			for(i=0;i<128;i++)
			{
				cout<<block_num[k]<<" ";
				fseek(fp,512*block_num[k],SEEK_SET);
				fwrite(tempfile,sizeof(File),1,fp);
				index2->blockaddress[i]=block_num[k++];
			}
			fseek(fp,512*index_second,SEEK_SET);
			fwrite(index2,sizeof(Index),1,fp);	
		}
		///�и�ľ�����������
		index_second =get_block();
		cout<<"���μ�������̿�:"<<index_second<<endl;
		index1->blockaddress[temp3-1]=index_second;
		index2 =(Index *)index;
		for(i=0;i<temp2-128*(temp3-1);i++)
		{
			cout<<block_num[k]<<" ";
			fseek(fp,512*block_num[k],SEEK_SET);
			fwrite(tempfile,sizeof(File),1,fp);
			index2->blockaddress[i]=block_num[k++];
		}
		fseek(fp,512*index_second,SEEK_SET);
		fwrite(index2,sizeof(Index),1,fp);	


		fseek(fp,512*index_first,SEEK_SET);
		fwrite(index1,sizeof(Index),1,fp);	

	}


	inode->blocknum=needblocknum;
	inode->links=1;
	inode->mtime = time(0);
	fseek(fp,512+64*inodeid,SEEK_SET);
	fwrite(inode,sizeof(Inode),1,fp);
	

	fseek(fp,512+64*inodeid,SEEK_SET);
	fread(inodeblock,sizeof(Inode),1,fp);
	inode =(Inode *)inodeblock;
	//cout<<inode->permissions<<endl;
	//4.��ǰinode�ڵ��ָ���Ŀ¼�̿飬�������ӵ��ļ�����
//	cout<<PWD<<endl;
	int pwdinodeid = findinode(PWD);
	fseek(fp,512+64*pwdinodeid,SEEK_SET);
	fread(inodeblock,sizeof(Inode),1,fp);
	inode =(Inode *)inodeblock;
	
	int parentinodeid = inode->inode_id;
	int parentaddress = inode->blockaddress[0];
	int ds = inode->ds;

	inode->ds++;
	fseek(fp,512+64*pwdinodeid,SEEK_SET);
	fwrite(inodeblock,sizeof(Inode),1,fp);

//	cout<<parentaddress<<endl;
//	cout<<ds<<endl;
	fseek(fp,512*parentaddress+32*ds,SEEK_SET);
	fread(directory,sizeof(Directory),1,fp);
	Directory *dir = (Directory *)directory;

	strcpy(dir->filename,filename.c_str());
	dir->inode_id = inodeid;
	dir->parent_id =parentinodeid;
	strcpy(dir->parentfilename,filename.c_str());
	fseek(fp,512*parentaddress+32*ds,SEEK_SET);
	fwrite(dir,sizeof(Directory),1,fp);
	
	cout<<endl;
	return 0;
}
/*
	ɾ���ļ�����
 */
int rm(int inodeid,string filename)
{
	/*
		0.���ж���ľ��w��Ȩ��
	*/
	if(get_auth(inodeid,2) != 0)
	{
		cout<<"Permission Limited!! ��ľ��Ȩ��ɾ������ļ�"<<endl;
		return -1;
	}

	int tempid=inodeid;
	fseek(fp,512+64*inodeid,SEEK_SET);
	fread(inodeblock,sizeof(Inode),1,fp);
	Inode *inode=(Inode *)(inodeblock);
	
	//cout<<inode->blockaddress[2]<<endl;
	//cout<<inode->links<<endl;
	if(inode->links > 1)
	{
		inodeid = findinode(PWD);
		//cout<<"��ǰĿ¼��inode"<<inodeid<<endl;
		fseek(fp,512+64*inodeid,SEEK_SET);
		fread(tempinodeblock,sizeof(Inode),1,fp);
		Inode *tempinode=(Inode *)(tempinodeblock);
	//	cout<<"��ǰĿ¼�̿�ĵ�ַ"<<endl;
	//	cout<<tempinode->blockaddress[0]<<endl;
	//	cout<<tempinode->ds<<endl;
		int ds = tempinode->ds;
		int blockaddress = tempinode->blockaddress[0];
		int id;
		for(int i=0 ;i<ds;i++)
		{
			fseek(fp,512*blockaddress+32*i,SEEK_SET);
			fread(directory,sizeof(Directory),1,fp);
			Directory *dir =(Directory *)directory;
		
		//cout<<dir->parentfilename<<endl;
			id =dir->inode_id;
			//cout<<dir->filename<<"��inode:"<<id<<endl;
			if(id == tempid || dir->filename == filename.c_str())
			{
				char *parname ="";
				strcpy(dir->parentfilename,parname);
			//	cout<<dir->parentfilename<<endl;
			//	cout<<"��������"<<endl;
				fseek(fp,512*blockaddress+32*i,SEEK_SET);
				fwrite(dir,sizeof(Directory),1,fp);
			}
		
		}
		
		fseek(fp,512+64*tempid,SEEK_SET);
		fread(inodeblock,sizeof(Inode),1,fp);
		Inode *inode=(Inode *)(inodeblock);
		inode->links--;
		inode->permissions[0]='x';
		inode->inode_id=-2;
		fseek(fp,512+64*tempid,SEEK_SET);
		fwrite(inode,sizeof(Inode),1,fp);
		return -1;
	}
	File *file;
	int i,j;
	int needblocknum =inode->blocknum;
	//ֻ��ֱ�ӵ�ַ��
	if(inode->blocknum <=4)
	{
		for(i=0; i<inode->blocknum; i++)
		{
			cout<<inode->blockaddress[i]<<" ";
			return_block(inode->blockaddress[i]);
		}
		return_inode(inode->inode_id);
	}
	//�õ�һ�ε�
	int index1,temp2;
	if(inode->blocknum <=128+4 && inode->blocknum >4)
	{
		for(i=0; i<4; i++)
		{
			cout<<inode->blockaddress[i]<<" ";
			return_block(inode->blockaddress[i]);
		}
		index1=inode->blockaddress[4];
		temp2=inode->blocknum-4;
		fseek(fp,512*index1,SEEK_SET);
		fread(index,sizeof(Index),1,fp);
		Index *index1 =(Index *)index;
		for(i=0; i<temp2; i++)
		{
			cout<<index1->blockaddress[i]<<" ";
			return_block(index1->blockaddress[i]);
		}
		cout<<"����һ�μ��"<<inode->blockaddress[4]<<endl;
		return_block(inode->blockaddress[4]);
		cout<<"����inode"<<inode->inode_id<<endl;
		return_inode(inode->inode_id);
		

	}
	//��ȡ�õ�2�μ�ַ���ļ�
	int index_first;
	if(inode->blocknum <=128*128+128+4 && inode->blocknum >4+128)
	{
		for(i=0; i<4; i++)
		{
			cout<<inode->blockaddress[i]<<" : ";
			return_block(inode->blockaddress[i]);
		}
		index_first=inode->blockaddress[4];
	//	temp2=inode->blocknum-4;
		fseek(fp,512*index_first,SEEK_SET);
		fread(index,sizeof(Index),1,fp);
		Index *index1 =(Index *)index;
		for(i=0; i<128; i++)
		{
			cout<<index1->blockaddress[i]<<" ";
			return_block(index1->blockaddress[i]);
		}
		cout<<"����һ�μ��"<<inode->blockaddress[4]<<endl;
		return_block(inode->blockaddress[4]);
		
		//2��������ȡ
		//2.1�ȶ���һ��
		int index_first = inode->blockaddress[5];
		fseek(fp,512*index_first,SEEK_SET);
		fread(files,sizeof(Index),1,fp);
		index1 =(Index *)files;
		
		int temp2 = needblocknum-128-4;
		int temp3 =	(temp2-1)/128+1;			//��Ҫ2���������̿���

		Index *index2;
		int k,h=0;
		for(j=0;j<temp3-1; j++)
		{			
			fseek(fp,index1->blockaddress[j]*512,SEEK_SET);
			fread(index,sizeof(Index),1,fp);
			index2 =(Index *)index;
			for(i=0;i<128;i++)
			{
				cout<<index2->blockaddress[i]<<" ";
				return_block(index2->blockaddress[i]);
			}
			cout<<"����2�μ��"<<index1->blockaddress[j]<<endl;
			return_block(index1->blockaddress[j]);
			h++;
			
		}
		
		fseek(fp,index1->blockaddress[temp3-1]*512,SEEK_SET);
		fread(index,sizeof(Index),1,fp);
		index2 =(Index *)index;
		for(i=0;i<temp2-h*128;i++)
		{
			cout<<index2->blockaddress[i]<<" ";
			return_block(index2->blockaddress[i]);
		}
		cout<<"����2�μ���̿�"<<index1->blockaddress[temp3-1]<<endl;
		return_block(index1->blockaddress[temp3-1]);
		cout<<"����1�μ���̿�"<<inode->blockaddress[5]<<endl;
		return_block(inode->blockaddress[5]);
		cout<<"����inode"<<inode->inode_id<<endl;
		return_inode(inode->inode_id);

	}

	inodeid = findinode(PWD);
//	cout<<"��ǰĿ¼��inode"<<inodeid<<endl;
	fseek(fp,512+64*inodeid,SEEK_SET);
	fread(inodeblock,sizeof(Inode),1,fp);
	inode=(Inode *)(inodeblock);
//	cout<<"��ǰĿ¼�̿�ĵ�ַ"<<endl;
//	cout<<inode->blockaddress[0]<<endl;
//	cout<<inode->ds<<endl;
	int ds = inode->ds;
	int blockaddress = inode->blockaddress[0];
	int id;
	for(i=0 ;i<ds;i++)
	{
		fseek(fp,512*blockaddress+32*i,SEEK_SET);
		fread(directory,sizeof(Directory),1,fp);
		Directory *dir =(Directory *)directory;
		
		//cout<<dir->parentfilename<<endl;
		id =dir->inode_id;
	//	cout<<dir->filename<<"��inode:"<<id<<endl;
		if(id == tempid || dir->filename == filename.c_str())
		{
			char *parname ="";
			dir->parent_id=1;
			strcpy(dir->parentfilename,parname);
		//	cout<<dir->parentfilename<<endl;
		//	cout<<"��������"<<endl;
			fseek(fp,512*blockaddress+32*i,SEEK_SET);
			fwrite(dir,sizeof(Directory),1,fp);
		}
		
	}




	cout<<endl;
	return 0;
}
/*
	�����ļ�
*/
int cp(int inodeid,string newfilename)
{
	/*
		0.cp���ж���ľ��r��Ȩ��
	*/
	if(get_auth(inodeid,4) != 0)
	{
		cout<<"Permission Limited!! ��ľ��Ȩ�޸�������ļ�"<<endl;
		return -1;
	}

	fseek(fp,512+64*inodeid,SEEK_SET);
	fread(inodeblock,sizeof(Inode),1,fp);
	Inode *inode = (Inode *)inodeblock; 
	
	int filesize =inode->filesize;
	fseek(fp,512*inode->blockaddress[0],SEEK_SET);
	fread(files,sizeof(File),1,fp);
	File *file =(File *)files;
	char filechar =file->filechar[0];
	
	return mk(newfilename,filesize,filechar);

}
/*
	�����ļ�
	���������ļ�����Ȩ����֤
*/
int ln(int inodeid,string oldfilename,string linkfilename,int link)
{
	fseek(fp,512+64*inodeid,SEEK_SET);
	fread(inodeblock,sizeof(Inode),1,fp);
	Inode *inode =(Inode *)inodeblock;
	//1.link==1 ������
	int i;
	if(link == 1)
	{
		int newinodeid = get_inode();
		if(newinodeid >=0 )
		{
			Inode *lninode=(Inode *)tempinodeblock;
			for(i=0;i<6;i++)
			{
				lninode->blockaddress[i]=inode->blockaddress[i];
			}
			lninode->filesize	=	512;			//�����ӵĴ�С��С��
			lninode->blocknum	=	inode->blocknum;
			lninode->ds			=	inode->inode_id;
			char *per="lrwxrwxrwx";
			strcpy(lninode->permissions,per);
			lninode->inode_id	=	newinodeid;
			lninode->user_id	=	USERID;
			lninode->group_id	=	GROUPID;
			lninode->mtime		=	time(0);
			lninode->links		=	1;
			fseek(fp,512+64*newinodeid,SEEK_SET);
			fwrite(lninode,sizeof(Inode),1,fp);

			//4.��ǰinode�ڵ��ָ���Ŀ¼�̿飬�������ӵ�Ŀ¼
	
			int pwdinodeid = findinode(PWD);
			fseek(fp,512+64*pwdinodeid,SEEK_SET);
			fread(inodeblock,sizeof(Inode),1,fp);
			inode =(Inode *)inodeblock;
	
			int parentinodeid = inode->inode_id;
			int parentaddress = inode->blockaddress[0];
			int ds = inode->ds;

			inode->ds++;
			fseek(fp,512+64*pwdinodeid,SEEK_SET);
			fwrite(inodeblock,sizeof(Inode),1,fp);

			fseek(fp,512*parentaddress+32*ds,SEEK_SET);
			fread(directory,sizeof(Directory),1,fp);
			Directory *dir = (Directory *)directory;

			strcpy(dir->filename,linkfilename.c_str());
			dir->inode_id = newinodeid;
			dir->parent_id =parentinodeid;
			
			strcpy(dir->parentfilename,oldfilename.c_str());
			fseek(fp,512*parentaddress+32*ds,SEEK_SET);
			fwrite(dir,sizeof(Directory),1,fp);
			
		}
		else
		{
			cout<<"inode�Ѿ���������\n";
			
		}
	}
	//2.link==0 Ӳ����
	if(link == 0)
	{
		inode->links++;
		int tempid = inode->inode_id;
		fseek(fp,512+64*inodeid,SEEK_SET);
		fwrite(inode,sizeof(Inode),1,fp);
		//4.��ǰinode�ڵ��ָ���Ŀ¼�̿飬�������ӵ�Ŀ¼
	
			int pwdinodeid = findinode(PWD);
			fseek(fp,512+64*pwdinodeid,SEEK_SET);
			fread(inodeblock,sizeof(Inode),1,fp);
			inode =(Inode *)inodeblock;
	
			int parentinodeid = inode->inode_id;
			int parentaddress = inode->blockaddress[0];
			int ds = inode->ds;

			inode->ds++;
			fseek(fp,512+64*pwdinodeid,SEEK_SET);
			fwrite(inodeblock,sizeof(Inode),1,fp);

			fseek(fp,512*parentaddress+32*ds,SEEK_SET);
			fread(directory,sizeof(Directory),1,fp);
			Directory *dir = (Directory *)directory;

			strcpy(dir->filename,linkfilename.c_str());
			dir->inode_id = tempid;
			dir->parent_id = -1;
			
			strcpy(dir->parentfilename,oldfilename.c_str());
			fseek(fp,512*parentaddress+32*ds,SEEK_SET);
			fwrite(dir,sizeof(Directory),1,fp);			
	}
	return 0;
}
/*
	ͬ���ļ�����֤
*/
int samefile(string filename)
{
	//0.��ǰinode�ڵ��ָ���Ŀ¼�̿飬�������ӵ��ļ�����
	
	int pwdinodeid = findinode(PWD);
	fseek(fp,512+64*pwdinodeid,SEEK_SET);
	fread(inodeblock,sizeof(Inode),1,fp);
	Inode *inode =(Inode *)inodeblock;
	
	
	int ds = inode->ds;
	
	//cout<<inode->blockaddress[0];
	for(int i=0; i<ds; i++)
	{
		fseek(fp,512*inode->blockaddress[0]+32*i,SEEK_SET);
		fread(directory,sizeof(Directory),1,fp);
		Directory *dir = (Directory *)directory;
		if(strcmp(dir->filename,filename.c_str())==0)
		{
			return -1;
		}
	}	
	return 0;
}