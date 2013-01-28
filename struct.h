#include<bitset>
#include<stack>
using namespace std;
/*
	inode
	�ڵ�Ľṹ
*/
typedef struct Inode{
	int inode_id;			//i�ڵ��
	int user_id;			//�û���ID
	int group_id;			//������ID
	char permissions[10];	//Ȩ��rwxrwxrwx
	time_t mtime;			//��һ���ļ����ݱ䶯��ʱ��
	int filesize;			//�ļ���С
	int blocknum;			//�ļ���ʹ�õĴ��̿��ʵ����Ŀ
	int blockaddress[6];	//�ļ�����block��λ��,ǰ4��Ϊֱ�ӣ���5��Ϊһ�μ��,��6��Ϊ2�μ��
	short links;			//�����������ж����ļ���ָ�����inode
	short ds;
}Inode;

/*
	fnode
	������Ľṹ
*/
typedef struct Fnode{
	char *fsname;			//�ļ�ϵͳ����
	int fssize;				//�ļ�ϵͳ��С
	int empty;				//�ļ�ϵͳ�����̿��С
	int usersnum;			//�û���Ŀ
	bitset<512> bitmap;		//λʾͼ
	int blockstack[10];		//���̿��п�ջ
}Fnode;
/*
	Directory
	�ļ�Ŀ¼��Ľṹ
	

*/
typedef struct Directory{
	char filename[14];			//Ŀ¼��
	short inode_id;				//Ŀ¼inodeid
	char parentfilename[14];	//��Ŀ¼��
	short parent_id;			//��Ŀ¼inodeid
}Directory;
/*
	User
	�û��ļ���Ŀ¼�ṹ
*/
typedef struct User{
	char username[16];		//�û���
	char password[16];		//�û�����
	int user_id;			//�û�ID
	int group_id;			//�û���ID
	int others;				//�����ֶ�
	char pwd[20];			//�û�����
}User;

/*
	��ʱ�����̿�ջ�ṹ
*/
typedef struct BlockStack{
	int blockstack[10];		//�����̿�ջ
}BlockStack;

/*
	Group
	�û����Ŀ¼�ṹ
*/
typedef struct Group{
	int group_id;			//�û���ID
	char groupname[12];		//�û�����
}Group;
/*
	�ļ���Ľṹ
*/
typedef struct File{
	char filechar[512];		//�ֽ�
}File;
/*
	�����ṹ	
*/
typedef struct Index{
	int blockaddress[128];	//�����̿�
}Index;
