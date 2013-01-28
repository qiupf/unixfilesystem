/*
	�ж��Ƿ����û������ҷ����û��ı��
*/
int isUser(string username)
{
	fseek(fp,0,SEEK_SET);
	fread(files,sizeof(Fnode),1,fp);
	Fnode *f_system = (Fnode *)files;
	
	int user_num = f_system->usersnum;

	//User *user =(User *)files;
	
	//cout<<user->username<<endl;
	for(int i =0; i<user_num;i++)
	{
		fseek(fp,512*68+64*i,SEEK_SET);
		fread(files,sizeof(User),1,fp);
		User *user =(User *)files;
		if(user->username == username)
		{
			return user->user_id;
		}
	}
	return -1;
}
/*
	�ж��Ƿ����飿���ҷ�����ı��
*/
int isGroup(string groupname)
{
	fseek(fp,512+64*7,SEEK_SET);
	fread(inodeblock,sizeof(Inode),1,fp);
	Inode *inode = (Inode *)inodeblock;
	
	int group_num = inode->ds;

	for(int i =0; i<group_num;i++)
	{
		fseek(fp,512*72+16*i,SEEK_SET);
		fread(files,sizeof(Group),1,fp);
		Group *group =(Group *)files;
		if(group->groupname == groupname)
		{
			return group->group_id;
		}
	}
	return -1;
}



/*
	��time_tת��Ϊ��������ʱ��
*/
void get_time(time_t t) 
{
	char day[3],mon[3];
	int date,hour,min,second,year;
	//time_t t;
   time(&t);
//	b = time(0);
//	cout<<ctime(&b);
//		printf("sizeof(time_t) = %d\n", sizeof(time_t));
 // printf("Today's date and time: %s\n", ctime(&t));
//	printf("Today's date and time: %s\n", ctime(&b));
//	printf("sizeof(time_t) = %d\n", sizeof(time_t));
    sscanf(ctime(&t),"%s %s %i %i:%i:%i %i",&day,&mon,&date,&hour,&min,&second,&year );
    printf("             %s %i %i %i:%i",mon,date,year,hour,min);
 //  printf("The day of today is:%s\n",day);
 //   printf("Now the time is: %i:%i\n",hour,min);
 //   getchar();
}
/*
	�����ļ�������
*/
void umask(string umaskcode="022")
{
	
	map<string,string> umaskmap;

	umaskmap["002"]="rwxrwxr-x";
	umaskmap["022"]="rwxr-xr-x";
	umaskmap["037"]="rwx-wx---";
	umaskmap["077"]="rwx------";
	umaskmap["000"]="rwxrwxrwx";
	string umask="rwxr-xr-x";
	if(umaskmap.count(umaskcode))
	{
		umask=umaskmap[umaskcode];
	}
	UMASK = umask;
	cout<<umask<<endl;
}
/*
	�����û�userid�ҵ��û�������
*/
string get_username(int userid)
{
	fseek(fp,0,SEEK_SET);
	fread(files,sizeof(Fnode),1,fp);
	Fnode *f_system = (Fnode *)files;
	
	int user_num = f_system->usersnum;

	//User *user =(User *)files;
	
	//cout<<user->username<<endl;
	for(int i =0; i<user_num;i++)
	{
		fseek(fp,512*68+64*i,SEEK_SET);
		fread(files,sizeof(User),1,fp);
		User *user =(User *)files;
		if(user->user_id == userid)
		{
			return user->username;
		}
	}
	return "nobody";
}
/*
	�����û���groupid�ҵ��û��������
*/
string get_groupname(int groupid)
{
	fseek(fp,512+64*7,SEEK_SET);
	fread(inodeblock,sizeof(Inode),1,fp);
	Inode *inode = (Inode *)inodeblock;
	
	int group_num = inode->ds;

	for(int i =0; i<group_num;i++)
	{
		fseek(fp,512*72+16*i,SEEK_SET);
		fread(files,sizeof(Group),1,fp);
		Group *group =(Group *)files;
		if(group->group_id == groupid)
		{
			return group->groupname;
		}
	}
	return "nobody";
}
/*
	��֤Ȩ��
	�������	��ǰ��¼�û���USERID,GROUPID[ȫ�ֱ���],
				�������inodeid(�ļ�inodeid,Ŀ¼inodeid)
				��Ҫ���еĲ���r=5,w=2,x=1
	����ֵ	0	���Բ���
			-1	ľ��Ȩ�޲���
*/
int get_auth(int inodeid,int authid)
{
	//0.�ж��ǲ���root,rootӵ��ȫ��Ȩ��
	if(USERID == 0)
	{
		return 0;
	}
//	cout<<"ż��userID:  "<<USERID<<endl;
//	cout<<"ż��groupID: "<<GROUPID<<endl;
	fseek(fp,512+64*inodeid,SEEK_SET);
	fread(inodeblock,sizeof(Inode),1,fp);
	Inode *inode =(Inode *)inodeblock;
//	cout<<inode->permissions<<endl;
	//1.��֤��¼�û��ǲ����ļ�ӵ���ߣ�����Ȩ�ޣ�
	if(inode->user_id == USERID)
	{
		switch(authid)
		{
			case 4:
			{
				if(inode->permissions[1] == 'r')
				return 0;
				break;
			}
			case 2:
			{
				if(inode->permissions[2] == 'w')
				return 0;
				break;
			}
			case 1:
			{
				if(inode->permissions[3] == 'x')
				return 0;
				break;
			}
		}
		return -1;
	}
	//2.��֤��¼�û��ڲ����ļ�ӵ���飬����Ȩ��?
	if(inode->group_id == GROUPID)
	{
		switch(authid)
		{
			case 4:
			{
				if(inode->permissions[4] == 'r')
				return 0;
				break;
			}
			case 2:
			{
				if(inode->permissions[5] == 'w')
				return 0;
				break;
			}
			case 1:
			{
				if(inode->permissions[6] == 'x')
				return 0;
				break;
			}
		}
		return -1;
	}
	//3.��֤��¼�û��������˵�Ȩ�ޣ�
	switch(authid)
		{
			case 4:
			{
				if(inode->permissions[7] == 'r')
				return 0;
				break;
			}
			case 2:
			{
				if(inode->permissions[8] == 'w')
				return 0;
				break;
			}
			case 1:
			{
				if(inode->permissions[9] == 'x')
				return 0;
				break;
			}
		}
	//����3��ȫ��ʧ��
	return -1;
}
/*
	��������ת��Ϊ�ַ�Ȩ��
*/
string num2char(string permissions)
{
	map<string,string> pmap;
	pmap["755"]="rwxr-xr-x";
	pmap["000"]="---------";
	pmap["766"]="rwxrw-rw-";
	pmap["666"]="rw-rw-rw-";
	pmap["777"]="rwxrwxrwx";
	pmap["700"]="rwx------";
	pmap["770"]="rwxrwx---";
	pmap["333"]="-wx-wx-wx";

	if(pmap.count(permissions))
	{
		return pmap[permissions];
	}
	return "-1";
}
/*
	��ѯĳ��inode�Ƿ����
	0 ��δ����
	-1 �Ѿ�����
*/
int isInode(int inodeid)
{
	 fseek(fp,0,SEEK_SET); 
	 fread(files,sizeof(char),512,fp);
	 Fnode *f_system=(Fnode *)files;
	 if(f_system->bitmap[inodeid] == 0)
	 {
			return 0;

	 }
	 else
	 {
		return -1;
	 }
}


int su(string username="root")
{
	//string username;
	string password;
	int i;
	cout<<"Welcome to use zjut's filesystem!\n";
	while(1)
	{	
		
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
