#include<iostream>
#include<string>
using namespace std;
#include "help.h"
void shell()
{
/*4.������Ϣ����ѭ��
		4.1 ���û�����exit�˳�
	*/
	string shell;
	vector<string>str(20);
	str[0]="ls";
	str[1]="exit";
	str[2]="clear";
	str[3]="df";
	str[4]="mkdir";
	str[5]="cat";
	str[6]="cd";
	str[7]="pwd";
	str[8]="passwd";
	str[9]="umask";
	str[10]="rmdir";
	str[11]="chmod";
	str[12]="chown";
	str[13]="chgrp";
	str[14]="mv";				//�ƶ��ļ� ���߸ı��ļ���
	str[15]="cp";				//�����ļ�
	str[16]="rm";				//ɾ���ļ�
	str[17]="mk";				//���������ļ�
	str[18]="ln";				//
	str[19]="su";
	int ix;
	while(1)
	{
		////��ȡһ��SHELL���	
		getline(cin,shell);
		while(shell == "" )
		{
			cout<<"["<<USERNAME<<"@"<<HOSTNAME<<" "<<PWD<<"] "<<PS1;
			getline(cin,shell);
		}
		
		for(ix =0; ix != str.size(); ++ix)
		{
			if(shell.substr(0,shell.find_first_of(" ")) == str[ix])
			{
				break;
			}
		}
		//cout<<shell.substr(shell.find_last_of(" ")+1,2)<<endl;
		
		if("-h" == shell.substr(shell.find_last_of(" ")+1,2))
		{	
			shell=shell.substr(0,shell.find_first_of(" "));
			help(shell);
		}
		else
		switch(ix)
		{
			
			case 0:
			{
				//ls�г�Ŀ¼���ļ�
				///����/  �ҵ�/etc  Ȼ��
				int inodeid;
				string temp;
				if(shell.find_first_of(" ") == string::npos)
				{
					inodeid = findinode(PWD);
					//cout<<inodeid;
					ls(inodeid);
				}
				else
				{
					//cout<<PWD;
					temp = shell.substr(shell.find_first_of(" ")+1,shell.length()-shell.find_first_of(" "));
					inodeid = findinode(temp);
					
					if(inodeid >= 0)
					{
					//cout<<inodeid;
						ls(inodeid,temp);
					}
					else
					{
						cout<<"ľ���ҵ�"<<temp<<"�������"<<endl;
					}
				}
				break;
			}
			case 1:
			{
				exit_block_stack();
				sync();
				
				cout<<"�û� "<<USERNAME<<" �Ѿ��˳�"<<endl;
				exit(0);
				break;
			}
			case 2:
			{
				////����
				system("cls");
				break;
			}
			case 3:
			{
				//��ʾ�ļ�ϵͳ����ϸ��Ϣ
				df();
				break;
			}
			case 4:
			{
				//�����ļ�Ŀ¼
				string dirname;
				if(shell.find_first_of(" ") == string::npos)
				{
					cout<<"������Ҫ������Ŀ¼������"<<endl;
				}
				else
				{
					dirname = shell.substr(shell.find_first_of(" ")+1,shell.length()-shell.find_first_of(" "));
					mkdir(dirname);
				}
				break;
			}
			case 5:
			{
				//��ʾ�ļ�ϵͳ����ϸ��Ϣ
				int inodeid;
				int is;
				string temp;
				if(shell.find_first_of(" ") == string::npos)
				{
					cout<<"��Ҫcat ��?"<<endl;
				}
				else
				{
					temp = shell.substr(shell.find_first_of(" ")+1,shell.length()-shell.find_first_of(" "));
					inodeid = findinode(temp);
					
					is = Isdir(inodeid);
				//	cout<<inodeid<<endl;
					if(is == 0)
					{
						cat(inodeid);
					}
					else
					{
						cout<<temp<<"�Ǹ�Ŀ¼"<<endl;
					}

				}
				
				break;
			}
			case 6:
			{
				////cd �ı��ļ�Ŀ¼��Ҫ����
				cd(shell);
				/**/
				break;
			}
			case 7:
			{
				//��ʾ�ļ�ϵͳ����ϸ��Ϣ
				pwd();
				break;
			}
			case 8:
			{
				//�޸��û�����
				int suss;
				string temp;
				if(shell.find_first_of(" ") == string::npos)
				{
					
				}
				else
				{
					temp = shell.substr(shell.find_first_of(" ")+1,shell.length()-shell.find_first_of(" "));

					//cout<<temp<<endl;
					suss = passwd(temp);
					if(suss == 0)
					{
						cout<<"�޸�����ɹ�\n";
						//cin.clear();
						/*
							����ʹ��cin.clear();�ǲ��Եģ�cin.clear()���ò�����ռ��̻����������������ǰʹ��cin���趨��һЩcin״̬(�������������ȣ���ʽ�ȵ�)�����������Ӧ��ʹ��cin.ignore()��
							cin.ignore()��һ�����ù��ܾ�����������Իس����������뻺���������ݣ�������һ���������һ�������Ӱ�졣���������ô�ã�cin.ignore( 1024,   '\n'   );
							ͨ���ѵ�һ���������õ��㹻������ʵ��������ֻ�еڶ�������   '\n'   �����ã�������һ����ǰѻس��������س���֮ǰ�������ַ������뻺�壨�����������ȥ
						
						*/
						cin.ignore();
					}
					else
					{
						cout<<"ľ��"<<temp<<"����û�,�޷��޸�����"<<endl;
					}

				}
				break;
			}
			case 9:
			{
				//umask �����ļ�Ĭ��Ȩ��
				string temp;
				if(shell.find_first_of(" ") == string::npos)
				{
					umask();
				}
				else
				{
					temp = shell.substr(shell.find_first_of(" ")+1,shell.length()-shell.find_first_of(" "));
					umask(temp);
				}
			
				break;
			}
			case 10:
			{
				//ɾ���ļ�Ŀ¼
				string dirname;
				if(shell.find_first_of(" ") == string::npos)
				{
					cout<<"������Ҫɾ����Ŀ¼������"<<endl;
				}
				else
				{
					dirname = shell.substr(shell.find_first_of(" ")+1,shell.length()-shell.find_first_of(" "));
					rmdir(dirname);
				}
				break;
			}
			case 11:
			{
				//chmod �ı��дִ�е�Ȩ��rwx
				string permissions;
				string temp;
				int inodeid;
				int xx;
				if(shell.find_first_of(" ") == string::npos)
				{
					cout<<"ȱ�ٲ���"<<endl;
				}
				else
				{
					permissions = shell.substr(shell.find_first_of(" ")+1,3);
					temp = shell.substr(shell.find_last_of(" ")+1,shell.length()-shell.find_last_of(" "));
				//	cout<<permissions<<endl;
				//	cout<<temp<<endl;
					inodeid = findinode(temp);

					if(inodeid != -1)
					{
						xx = chmod(permissions,inodeid);
						if(xx == 0)
						{
						
						}
						else
						{
							cout<<"�޸�Ȩ��ʧ��,û�д���Ȩ��"<<endl;
						}
					}
					else
					{
						cout<<"�޸�Ȩ��ʧ��"<<endl;
					}
				}
				break;
			}
			case 12:
			{
				//chown �ı�ӵ����
				string owner;
				string temp;
				int inodeid;
				int userid;
				int xx;
				if(shell.find_first_of(" ") == string::npos)
				{
					cout<<"ȱ�ٲ���"<<endl;
				}
				else
				{
					xx = shell.find_first_of(" ");
					owner = shell.substr(xx+1,shell.find_first_of(" ",xx+1)-xx-1);
					temp = shell.substr(shell.find_last_of(" ")+1,shell.length()-shell.find_last_of(" "));
				//	cout<<permissions<<endl;
				//	cout<<owner<<endl;
					inodeid = findinode(temp);
					if(inodeid != -1)
					{
						userid = isUser(owner);
						if(userid >= 0)
						{
							chown(userid,inodeid);
						}
						else
						{
							cout<<"�Բ���ľ������û�"<<owner<<endl;
						}
					}
					else
					{
						cout<<"��ǰĿ¼������"<<temp<<endl;
					}
				}
				break;
			}
			case 13:
			{
				//chgrp �ı��ļ�������
				string group;
				string temp;
				int inodeid;
				int groupid;
				int xx;
				if(shell.find_first_of(" ") == string::npos)
				{
					cout<<"ȱ�ٲ���"<<endl;
				}
				else
				{
					xx = shell.find_first_of(" ");
					group = shell.substr(xx+1,shell.find_first_of(" ",xx+1)-xx-1);
					temp = shell.substr(shell.find_last_of(" ")+1,shell.length()-shell.find_last_of(" "));
				//	cout<<permissions<<endl;
				//	cout<<owner<<endl;
					inodeid = findinode(temp);
					if(inodeid != -1)
					{
						groupid = isGroup(group);
						if(groupid >= 0)
						{
							chgrp(groupid,inodeid);
						}
						else
						{
							cout<<"�Բ���ľ������û��� "<<group<<endl;
						}
					}
					else
					{
						cout<<"��ǰĿ¼������"<<temp<<endl;
					}
				}
				break;
			}
			case 14:
			{
				//mv �ı��ļ���
				string filename;
				string newfilename;
				int xx,inodeid;
				if(shell.find_first_of(" ") == string::npos)
				{
					cout<<"ȱ�ٲ���"<<endl;
				}
				else
				{
					xx = shell.find_first_of(" ");
					filename = shell.substr(xx+1,shell.find_first_of(" ",xx+1)-xx-1);
					newfilename = shell.substr(shell.find_last_of(" ")+1,shell.length()-shell.find_last_of(" "));
					//cout<<"�ĺ������Ϊ"<<newfilename<<endl;

					inodeid = findinode(filename);
					if(inodeid != -1)
					{
						mv(inodeid,newfilename);
					}
					else
					{
						cout<<"��ǰĿ¼������"<<filename<<endl;
					}
				}
				break;
			}
			case 15:
			{
				//cp �����ļ�
				string filename;
				string newfilename;
				int xx,inodeid;
				if(shell.find_first_of(" ") == string::npos)
				{
					cout<<"ȱ�ٲ���"<<endl;
				}
				else
				{
					xx = shell.find_first_of(" ");
					filename = shell.substr(xx+1,shell.find_first_of(" ",xx+1)-xx-1);
					newfilename = shell.substr(shell.find_last_of(" ")+1,shell.length()-shell.find_last_of(" "));
					//cout<<"�ĺ������Ϊ"<<newfilename<<endl;

					inodeid = findinode(filename);
					if(inodeid != -1)
					{
						cp(inodeid,newfilename);
					}
					else
					{
						cout<<"��ǰĿ¼������"<<filename<<endl;
					}
				}
				break;
			}
			case 16:
			{
				//rm ɾ���ļ�
				string filename;
				int inodeid;
				if(shell.find_first_of(" ") == string::npos)
				{
					cout<<"������Ҫɾ����Ŀ¼������"<<endl;
				}
				else
				{
					filename = shell.substr(shell.find_first_of(" ")+1,shell.length()-shell.find_first_of(" "));
					inodeid = findinode(filename);
					rm(inodeid,filename);
				}
				break;
			}
			case 17:
			{
				//mk �����ļ�
				string filename;
				int filesize;
				char filechar;
				cout<<"�������ļ�����: ";
				cin>>filename;
				cout<<"\n�������ļ���С: ";
				cin>>filesize;
				cout<<"\n����������ַ�: ";
				cin>>filechar;
				mk(filename,filesize,filechar);
				cin.ignore();
				break;
			}
			case 18:
			{
				//ln �����ļ� 2�����,�����˿���ɾ����ʱ���ǵ�links ==0 ��ʱ��Ż�ɾ����
				string filename;
				string linkfilename;
				string link;
				int index,index2,index3,inodeid;
				if(shell.find_first_of(" ") == string::npos)
				{
					cout<<"ȱ�ٲ���"<<endl;
				}
				else
				{
					index  = shell.find_first_of(" ");
					index2 = shell.find_first_of(" ",index+1);
					index3 = shell.find_last_of(" ");
					filename = shell.substr(index+1,index2-index-1);
					linkfilename = shell.substr(index2+1,index3-index2-1);
					link = shell.substr(index3+1,shell.length()-index3);
					inodeid = findinode(filename);
					if(inodeid != -1)
					{
						if(link=="-s")
						{
							cout<<"ż��������\n";
							ln(inodeid,filename,linkfilename,1);
						}
						else if(index2==index3)
						{
							cout<<"ż��Ӳ����\n";
							ln(inodeid,filename,linkfilename,0);
						}
						else
						{
							cout<<"�Բ���,û�����ѡ��(Ӳ���Ӳ��Ӳ���,�����Ӽ�-s)\n";
						}
					}
					else
					{
						cout<<"��ǰĿ¼������"<<filename<<endl;
					}
				}
				break;
			}
			case 19:
			{
				string username;
				username = shell.substr(shell.find_first_of(" ")+1,shell.length()-shell.find_first_of(" "));
				if(isUser(username)>=0)
				{
					su(username);
				}
				else if(shell=="su")
				{
					su();
				}
				else
				{
					cout<<"���������"<<username<<"�û�"<<endl;
				}
				//su("root");
				cin.ignore();
				break;
			}
			default:
			{
				cout<<"ľ�����ָ��\n";
			}
		}
		cout<<"["<<USERNAME<<"@"<<HOSTNAME<<" "<<PWD<<"] "<<PS1;
	//	cin.clear();
	//	cout.clear();
		
	}

}