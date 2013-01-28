
//����һЩ��Ҫ���ļ�,��Щ�ļ���/ /bin /etc /etc/passwd /etc/etc/shadow /home 
void init_dir()
{
		// ��1��inode id=0,�����̿�ռ�õ�66��
		Inode *inode=(Inode *)(filesystem+512);
		inode->inode_id = 0;
		inode->group_id = 0;
		inode->blockaddress[0] = 65;
		inode->filesize = 512;
		inode->blocknum = 1;
		inode->links =1;
		inode->user_id = 0; //root  �û�IDΪ1
		char * permissons = "drwxrwxr-x";
		strcpy(inode->permissions,permissons);
		inode->mtime = time(0);
		inode->ds = 2;
		Directory *directory = (Directory *)(filesystem+512*65);
		char * filename ;
		filename = "home";
		strcpy(directory->filename,filename);
		directory->inode_id = 1;
		directory->parent_id = 0;
		filename = "/";
		strcpy(directory->parentfilename,filename);

		directory++;
		filename = "etc";
		strcpy(directory->filename,filename);
		directory->inode_id = 2;
		directory->parent_id = 0;
		filename = "/";
		strcpy(directory->parentfilename,filename);

		//��2��inode id=1 /homeĿ¼ ռ�õ�67���̿�
		inode=(Inode *)(filesystem+512+64);
		inode->inode_id = 0;
		inode->group_id = 0;
		inode->blockaddress[0] = 66;
		inode->filesize = 512;
		inode->blocknum = 1;
		inode->links =1;
		inode->user_id = 0; //root  �û�IDΪ0
		permissons = "drwxr-xr-x";
		strcpy(inode->permissions,permissons);
		inode->mtime = time(0);
		inode->ds = 2;
		//��һ����Ŀ¼
		directory = (Directory *)(filesystem+512*66);
		filename = "suzie";
		strcpy(directory->filename,filename);
		directory->inode_id = 4;
		directory->parent_id =1;
		filename="home";
		strcpy(directory->parentfilename,filename);
		//�ڶ�����Ŀ¼
		directory++;
		filename = "root";
		strcpy(directory->filename,filename);
		directory->inode_id = 6;
		directory->parent_id =1;
		filename="home";
		strcpy(directory->parentfilename,filename);



		//��3��inode id=2 /etc Ŀ¼ ռ�õ�68��
		inode=(Inode *)(filesystem+512+64*2);
		inode->inode_id = 2;
		inode->group_id = 0;
		inode->blockaddress[0] = 67;
		inode->filesize = 512;
		inode->blocknum = 1;
		inode->links =1;
		inode->user_id = 0; //root  �û�IDΪ0
		permissons = "drwxrw-rw-";
		strcpy(inode->permissions,permissons);
		inode->mtime = time(0);
		inode->ds = 2;

		directory = (Directory *)(filesystem+512*67);
		filename = "passwd";
		strcpy(directory->filename,filename);
		directory->inode_id = 3;
		directory->parent_id =2;
		filename = "etc";
		strcpy(directory->parentfilename,filename);
		directory++;
		filename = "group";
		strcpy(directory->filename,filename);
		directory->inode_id = 7;
		directory->parent_id =2;
		filename = "etc";
		strcpy(directory->parentfilename,filename);

		//��4��inode id=3 /etc/passwd ռ�õ�69���̿�
		inode=(Inode *)(filesystem+512+64*3);
		inode->inode_id = 3;
		inode->group_id = 0;
		inode->blockaddress[0] = 68;
		inode->filesize = 512;
		inode->blocknum = 1;
		inode->links =1;
		inode->user_id = 0; //root  �û�IDΪ0
		permissons = "-rwxrw-rw-";
		strcpy(inode->permissions,permissons);
		inode->mtime = time(0);
		inode->ds = 2;
		//��һ��Ĭ���û�suzie
		User *user =(User *)(filesystem+512*68);
		char *temp="root";
		strcpy(user->username,temp);
		temp="root";
		strcpy(user->password,temp);
		user->user_id = 0;
		user->group_id = 0;
		temp="/home/root";
		strcpy(user->pwd,temp);

		user++;
		temp="suzie";
		strcpy(user->username,temp);
		temp="123";
		strcpy(user->password,temp);
		user->user_id = 1;
		user->group_id = 1;
		temp="/home/suzie";
		strcpy(user->pwd,temp);
		///��2��Ĭ���û�root
	




		//��5���ļ� id=4 /home/suzie ռ�õ�70���̿�
		inode=(Inode *)(filesystem+512+64*4);
		inode->inode_id = 4;
		inode->group_id = 1;
		inode->blockaddress[0] = 69;
		inode->filesize = 512;
		inode->blocknum = 1;
		inode->links =1;
		inode->user_id = 1; //suzie  �û�IDΪ1
		permissons = "drwxrw-rw-";
		strcpy(inode->permissions,permissons);
		inode->mtime = time(0);
		inode->ds = 1;
		
		directory = (Directory *)(filesystem+512*69);
		filename = "test";
		strcpy(directory->filename,filename);
		directory->inode_id = 5;
		directory->parent_id =4;
		filename = "suzie";
		strcpy(directory->parentfilename,filename);
		//��6���ļ� id=5 /home/suzie/test ռ�õ�71��
		inode=(Inode *)(filesystem+512+64*5);
		inode->inode_id = 5;
		inode->group_id = 1;
		inode->blockaddress[0] = 70;
		inode->filesize = 512;
		inode->blocknum = 1;
		inode->links =1;
		inode->user_id = 1; //suzie  �û�IDΪ1
		permissons = "-rwxrw-rw-";
		strcpy(inode->permissions,permissons);
		inode->mtime = time(0);
		inode->ds = 0;

		//��7���ļ� id=6 /home/root ռ�õ�72��
		inode=(Inode *)(filesystem+512+64*6);
		inode->inode_id = 6;
		inode->group_id = 0;
		inode->blockaddress[0] = 71;
		inode->filesize = 512;
		inode->blocknum = 1;
		inode->links =1;
		inode->user_id = 0; //root  �û�IDΪ0
		permissons = "drwxrw-rw-";
		strcpy(inode->permissions,permissons);
		inode->mtime = time(0);
		inode->ds = 0;
		//��8��Ĭ���ļ� id=7 /etc/group ռ�õ�73��
		inode=(Inode *)(filesystem+512+64*7);
		inode->inode_id = 7;
		inode->group_id = 0;
		inode->blockaddress[0] = 72;
		inode->filesize = 512;
		inode->blocknum = 1;
		inode->links =1;
		inode->user_id = 0; //root  �û�IDΪ0
		permissons = "-rwxrw-rw-";
		strcpy(inode->permissions,permissons);
		inode->mtime = time(0);
		inode->ds = 2;
		
		//��һ�� �û���root
		Group *group = (Group *)(filesystem+512*72);
		char *groupname = "root";
		strcpy(group->groupname,groupname);
		group->group_id=0;
		//�ڶ��� �û���suzie
		group++;
		groupname = "zjut";
		strcpy(group->groupname,groupname);
		group->group_id=1;


}