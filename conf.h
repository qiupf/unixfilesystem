
#define INODE_SIZE 512 //����Inode��С

string HOSTNAME = "zjut";			//�û�ȱʡ��ʾ��

string USERNAME = "guest";

string PWD = "/home/suzie";

string PS1 = "$";
int GROUPID=2;

int USERID=2;

string UMASK="rwxr-xr-x";

//char *PASSWORD	= "123";

char *filesystem=new char[512*2*1024];


char *files=new char[512];
char *tempfiles = new char[512];
char *index=new char[512];
char *inodeblock=new char[64];
char *tempinodeblock=new char[64];
char *directory = new char[32];
int *blockstack = new int[10];
char *filename ="1.txt";
/*SHELL���*/

FILE *fp;//=fopen("1.txt","r+b");

stack<int> block_stack;


//char *HOMEDIR = "/home/suzie";


///����
int get_block();
int get_inode();
void df();
void get_time();
void init_block_stack();
void exit_block_stack();
int samefile(string filename);
//���ձ�


