#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ASCII	99999

//"��",169,192
//"��",169,166
//"��",169,184
//"��",169,164

#define VERTICAL_SYMBOL "��"
#define HORIZONTAL_SYMBOL "��"
#define BRANCH_SYMBOL "��"
#define BROKEN_SYMBOL "��"
//ÿ��������һ���ļ���ʱ���ı��ǰ��ķ��ŵĸ�����Windows��4����linux��3��
#define WINDOWS_SYMBOL_CNT_PER_CHANGE_DIR (4)
#define LINUX_SYMBOL_CNT_PER_CHANGE_DIR (3)

#define printline()	printf("%s",line)
#define printpath()	printf("%s",path)
#define printname()	printf("%s\n",name)
#define printfpath()	write(destfile,path,path_len)
#define printffile()	write(destfile,file,file_len)

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
/*
ANSI�ļ����룬ʵ������Ŀ¼���ļ�·����ת��
*/

FILE *srcfile = NULL;
//FILE *destfile = NULL;
int line_cnt = 0;
int error_line_cnt = 1;
int error_flag = 0;
int dir_cnt = 0;
int dir_cnt_old = 1;
unsigned char line[MAX_ASCII];
unsigned char path[MAX_ASCII] = "/";
unsigned char name[MAX_ASCII];
unsigned char name_old[MAX_ASCII];

unsigned int symbol_cnt_per_change_dir = WINDOWS_SYMBOL_CNT_PER_CHANGE_DIR;

//struct line_info_t
//{
//  char data[MAX_ASCII];
//  char path[MAX_ASCII];
//  char name[MAX_ASCII];
//  int dir_cnt;
//  int error_flag;
//  int line_idx;
//}line_info,line_old_info;

//��·��ƴ������
void path_add(unsigned char* dir_name ,int len)
{
  if (dir_name[strlen(dir_name)-1] != '/')
  {
    dir_name[strlen(dir_name)] = '/';
  }
	strcat(path,dir_name);
	//printf("path+:%s+%d+%d+\n",path,path_len,strlen(path));
} 

void path_del()
{
  //printf("path:%s,",path);
  int len = strlen(path) - 1;
	while(path[--len] != '/')
	{
		if(len <= 0)
		{
			error_flag = 1;//���ִ���ֱ�ӽ�������
			return;
		}
		path[len] = '\0';
	}
	//printf("%s\n",path);
	return ;
}

void dir_cnt_deal()
{
    if(dir_cnt > dir_cnt_old)
    {
      path_add(name_old,0);
    }
    if(dir_cnt < dir_cnt_old)
    {
      int diff = dir_cnt_old - dir_cnt;
      while(diff--)
      {
        path_del();
      }
    }
    dir_cnt_old = dir_cnt;
}

int get_line_info(unsigned char* _line)
{
  int symbol_cnt = 0;
  char* _name = name;
  //���ɵ�ǰ����ַ�
  while(*_line != '\0')
  {
    if(((*_line == 169) && *(_line+1) == 166)	//"��",169,166
    || ((*_line == 169) && *(_line+1) == 164)	//"��",169,164
    || ((*_line == 169) && *(_line+1) == 192)	//"��",169,192
    || ((*_line == 169) && *(_line+1) == 184))	//"��",169,184
    {
      symbol_cnt += 2;
      _line += 2;
    }
    else if((*_line ==  32))
    {
      ++_line;
      ++symbol_cnt;
    }
    else
    {
      //ǰ��ֻ�ܳ��� "��""��"/"��""��"" "��Щ�ַ���������Ϊǰ���ַ�����
      break;
    }
  }
  //dir_cnt = (symbol_cnt-7)/5;
  //windows
  //dir_cnt = symbol_cnt / 4;
  dir_cnt = symbol_cnt / symbol_cnt_per_change_dir;
  printf("space_cnt:%d\t",symbol_cnt);
  //printf("%s",_line);
  //printf("cnt :%d\n",cnt);
  //printf("%d,%d,%d,%d,%d,%\n",*line,*(line+1),*(line+2),*(line+3),*(line+4),*(line+5));
  //ǰ���ַ��������������С��ļ��б�ʶ����������������Ϊ���ָ�����ɡ�
  while( *_line != '\n'
  // && *_line != '/'
   && *_line != '\0' )
  {
    *_name++ = *_line++;
  }
}


//ÿһ�н��д���
//δ��ɡ�
int line_deal(unsigned char* _line)
{
    line_cnt ++;
    //memcpy(&(line_info.data),_line,sizeof(_line));
    //get_name(_line,name);
    get_line_info(_line);
    printf("%d  ",dir_cnt);
    dir_cnt_deal();

    printf("%s",path);
    printf("%s",name);
    //printf("%s",name_old);
    //printf("line:%s",line);
    printf("\n");
    //calc_line(line);
}


int file_deal(char* file)
{
  char* ret;
  srcfile = fopen(file,"r");//ֻ����ʽ��tree�ļ�
  //srcfile = fopen("src-f.txt","r");
  //srcfile = fopen("src-bpcs.txt","r");
  //srcfile = fopen("./t2.txt","r");
  //destfile = fopen("2019-.txt","w+");//ֻд��ʽ���ļ�
  while(!feof(srcfile))
  {
    memcpy(name_old,name,sizeof(name));
    memset(line,0,strlen(line));
    memset(name,0,strlen(name));
    ret = fgets(line,MAX_ASCII,srcfile);
    if(ret == NULL)//�������ļ�ĩβ������ѭ��
    {
      break;
    }
    //printf("%s",line);
    line_deal(line);
  }
  fclose(srcfile);
  //fclose(destfile);
  printf("�ļ���%d�У�����·����%d�С�\n",line_cnt,error_line_cnt);
}

int main(int argc, char *argv[])
{
//	if(argc <= 1)
//	{
//		printf("no file to input,program will exit.\n");
//		return 0;
//	}
  if (argc >= 3)
  {
    if (strcmp("linux",argv[2]))
    {
      symbol_cnt_per_change_dir = LINUX_SYMBOL_CNT_PER_CHANGE_DIR;
    }
  }
  file_deal(argv[1]);
  //system("pause");
  return 0;

}
