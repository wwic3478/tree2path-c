#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_ASCII	99999
#define printline()	printf("%s",line)
#define printpath()	printf("%s",path)
#define printfile()	printf("%s\n",file)
#define printfpath()	write(destfile,path,path_len)
#define printffile()	write(destfile,file,file_len)
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
/*
ANSI�ļ����룬ʵ������Ŀ¼���ļ���Ŀ¼��ת��
������ /
��   ������ KTV/
��   ��   ������ DVD��⡾26.4T-19.2��/
��   ��   ��   ������ 14-15�겿�ָ���/
��   ��   ��   ��   ������ 14.07/
��   ��   ��   ��   ��   ������ 3������-ALL-����-970889.mpg
��   ��   ��   ��   ��   ������ A SHOW-˭˵�Ҳ��ں�����-����-970684.mpg
��   ��   ��   ��   ��   ������ A.SA ����-���ͽ�ɫ-����-970891.mpg

/t/
/t/KTV/
/t/KTV/DVD��⡾26.4T-19.2��/
/t/KTV/DVD��⡾26.4T-19.2��/14-15�겿�ָ���/
/t/KTV/DVD��⡾26.4T-19.2��/14-15�겿�ָ���/14.07/
/t/KTV/DVD��⡾26.4T-19.2��/14-15�겿�ָ���/14.07/3������-ALL-����-970889.mpg
/t/KTV/DVD��⡾26.4T-19.2��/14-15�겿�ָ���/14.07/A SHOW-˭˵�Ҳ��ں�����-����-970684.mpg
/t/KTV/DVD��⡾26.4T-19.2��/14-15�겿�ָ���/14.07/A.SA ����-���ͽ�ɫ-����-970891.mpg
/t/KTV/DVD��⡾26.4T-19.2��/14-15�겿�ָ���/14.07/A.SA ����-Ϊ����һ��-����-970890.mpg
*/


FILE *srcfile = NULL;
//FILE *destfile = NULL;
int line_cnt = 0;
int line_len = 0;
int path_len = 1;
int file_len = 0;
int dir_cnt = 0;
int error_cnt = 0;
int dir_cnt_old = 0;
char line_old[MAX_ASCII]; 
char line[MAX_ASCII];
char path[MAX_ASCII] = "/";
char file[MAX_ASCII];

void path_add(char* line ,int len)
{
	while(line[len] != '\n')
		path[path_len++] = line[len++];
	//printf("path+:%s+%d+%d+\n",path,path_len,strlen(path));
} 

void path_del()//	/0214/	6
{//		/0214/20190116-��/	18
	--path_len;
	path[path_len] = '\0';
	while(path[--path_len] != '/')
	{
		//if(path_len <= 0)
		{
		//	error_flag = 1;//���ִ���ֱ�ӽ������� 
		//	return;
		}
		path[path_len] = '\0';
	}
	path[++path_len] = '\0';
	//printf("path-:%s-%d-%d-\n",path,path_len,strlen(path));
	return ;
}

int calc_line(char* line)
{
	int len_dis = 2;
	int old = 0;
	int name_dis = 0;
	static int name_dis_old = 0;
	int dir_in = 0;
	dir_cnt = 0;
	line_len = strlen(line);
	//ǰ�洦��·������ 
	while(1)
	{
		if(line[len_dis-2] == '\n' || (line[len_dis - 2] == -87 && line[len_dis - 1] == -92))//"��"
			break; 
		if(line[len_dis-2] == -87 && line[len_dis-1] == -90)//������
		{
			dir_cnt ++ ;
		}
		len_dis++;
	}

	if((line[0] == -87 && line[1] == -64)	//"��"��-87��-64 
		 || (line[0] == -87 && line[1] == -90))//������
	{
		//�ٴδ����ļ���/�ļ��������� 
		while(line[++len_dis] != '\n')
		{
			if((line[len_dis - 3] == -87 && line[len_dis - 2] == -92) && line[len_dis - 1] == ' ')
			{//"��" + �ո���ţ������������ļ������ƻ����ļ����� 
				name_dis = len_dis;//��������ĸ�±��±� 
				//���������ļ��������ļ��������и��� 
				if(dir_cnt < dir_cnt_old)//�����ϲ�ǳ 
				{
					int out_cnt = dir_cnt_old - dir_cnt;
					while(out_cnt--)
						path_del();
				}
				else if(dir_cnt > dir_cnt_old)// �����ϲ��� 
				{
				 	path_add(line_old,name_dis_old);//�ӳ�·�� [�ַ�����ʼ��ַ���±�]
				} 
				dir_cnt_old = dir_cnt;//��ֵ 
				
				file_len = 0;
				while(line[len_dis] != '\n')
					file[file_len++] = line[len_dis++];
				file[file_len] = '\0';
				printpath();
				printfile();
				
				break;//ֱ�ӽ������������б�ķ����� 
			}
		}
		//printf("%d",dir_cnt);
		//printpath();
		name_dis_old = name_dis;
		strcpy(line_old,line);
	}
	else
	{
		printf("line_error:%d.\n",line_cnt);
		++error_cnt;
		return -1; 
	}
	return len_dis;
}

int main(int argc, char *argv[])
{
	char* ret;
	srcfile = fopen(argv[1],"r");//ֻ����ʽ��tree�ļ�
//	destfile = fopen("2019-.txt","w+");//ֻд��ʽ���ļ�
	//int offset = 0;
	//while(offset -- )
	{
	//	fgets(line,MAX_ASCII,srcfile); 
	}
	while(!feof(srcfile))
	{
		memset(line,strlen(line),0);
		ret = fgets(line,MAX_ASCII,srcfile); 
		if(ret == NULL)//�������ļ�ĩβ������ѭ�� 
			break;
		line_cnt ++;
		//printf("%d,%d:",line[0],line[1]);
		calc_line(line);
	}
	fclose(srcfile);
	//fclose(destfile);
	printf("�ļ���%d�У�����·����%d�С�\n",line_cnt,error_cnt);
	return 0;

}
