/*
	1.��ʾmkdir��ʱ�� ��inodeid
	Ȼ��rmdirһ�£����ʱ��inode�Ѿ����գ�Ȼ����ȥ����mkdir��inodeid��һ�µ�!
*/
void help(string shell)
{

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
	int ix;
	for(ix =0; ix != str.size(); ++ix)
	{
		if(shell.substr(0,shell.find_first_of(" ")) == str[ix])
		{
			break;
		}
	}
	cout<<"       "<<shell<<"    �İ���"<<endl;
	cout<<"============================\n";
	switch(ix)
	{
		case 0:
		{
			cout<<"ls        ֱ����ʾ��ǰĿ¼����Ϣ\n";
			cout<<"ls /home  ��ʾ/homeĿ¼�µ�Ŀ¼��Ϣ(���·��������·������)\n";
			cout<<"ls test   ��ʾ��ǰ�ı��ļ�����Ϣ\n";
			break;
		}
		case 1:
		{
			break;
		}
		case 11:
		{
			cout<<"chmod 755 /home  �ı��ļ�����Ŀ¼��Ȩ��\n";
			break;
		}
		case 12:
		{
			cout<<"chown suzie /home  �ı��ļ�ӵ����\n";
			break;
		}
		case 13:
		{
			cout<<"chgrp zjut /home  �ı��ļ�������\n";
			break;
		}
		case 17:
		{
			cout<<"mk newfilename 512*7 a	�ڵ�ǰĿ¼�´���һ����СΪ512*7������Ϊnewfilename ������a��ĸ�����ļ�\n";
		}
	}
}