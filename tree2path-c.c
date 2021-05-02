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
#define DIR_SYMBOL '/'
//�������ݲ�һ���������ġ�
//ÿ��������һ���ļ���ʱ���ı��ǰ��ķ��ŵĸ�����Windows��4����linux��5��
//��ʹ��termux��tree�����׿��0��7��12��17��22��27��
//bpcs��bpcs tree /f >����0��7��12��17��22��27��
//ubuntu��/f����0��7��12��17��22��27��
//centos��/f����0��7��12��17��22��27��
//windows��cmd��tree /f����0��4��8��12��16��28

#define WINDOWS_SYMBOL_CNT_PER_CHANGE_DIR (4)
#define LINUX_SYMBOL_CNT_PER_CHANGE_DIR (5)

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
FILE *destfile = NULL;
unsigned char srcfile_str[50] = {0};
unsigned char destfile_str[50] = {0};
int line_cnt = 0;
int error_line_cnt = 0;
int error_flag = 0;
int symbol_cnt = 0;
int dir_cnt = 0;
int dir_cnt_old = 1;
unsigned char line[MAX_ASCII];
unsigned char path[MAX_ASCII] = "\0";
unsigned char name[MAX_ASCII];
unsigned char buffer[MAX_ASCII];
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
void path_add(unsigned char *dir_name ,int len)
{
  if (dir_name[strlen((char *)dir_name)-1] != DIR_SYMBOL)
  {
    dir_name[strlen((char *)dir_name)] = DIR_SYMBOL;
  }
	strcat((char *)path,(char *)dir_name);
	//printf("path+:%s+%d+%d+\n",path,path_len,strlen(path));
} 

void path_del()
{
  //printf("path:%s,",path);
  int len = strlen((char *)path) - 1;
	while(path[--len] != DIR_SYMBOL)
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
      while((diff--) > 0)
      {
        path_del();
      }
    }
    dir_cnt_old = dir_cnt;
}

int get_line_info(unsigned char *_line)
{
  unsigned char *_name = name;
  //���ɵ�ǰ����ַ�
  symbol_cnt = 0;
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
  //symbol_cnt -= 2;
  // ������ֿ��У��򲻳��ֹ���
  if (symbol_cnt / symbol_cnt_per_change_dir != 0)
	  dir_cnt = symbol_cnt / symbol_cnt_per_change_dir;
  //printf("space_cnt:%d\t",symbol_cnt);
  //printf("%s",_line);
  //printf("cnt :%d\n",cnt);
  //printf("%d,%d,%d,%d,%d,%\n",*line,*(line+1),*(line+2),*(line+3),*(line+4),*(line+5));
  //ǰ���ַ��������������С��ļ��б�ʶ����������������Ϊ���ָ�����ɡ�
  if (dir_cnt != 0)
  {
	  while( *_line != '\n'
	  // && *_line != '/'
	   && *_line != '\0' )
	  {
		*_name++ = *_line++;
	  }
  }
}


//ÿһ�н��д���
//δ��ɡ�
int line_deal(unsigned char *_line)
{
    line_cnt ++;
    //memcpy(&(line_info.data),_line,sizeof(_line));
    //get_name(_line,name);
    get_line_info(_line);
    //printf("%d  ",dir_cnt);
    dir_cnt_deal();
}


int file_deal(unsigned char *_srcfile,unsigned char *_destfile)
{
  char *ret;
  FILE* logfile = NULL;
  if (NULL == _srcfile)
  {
	return -1;
  }
  srcfile = fopen((char *)_srcfile,"r");//ֻ����ʽ��tree�ļ������û�оʹ�ʧ�ܡ�
  destfile = fopen((char *)_destfile,"w+");//ֻд��ʽ���ļ������û���ļ��ʹ����ļ���
  logfile = fopen("tree2path_log.txt","w+");
  if (NULL == srcfile || NULL == destfile || NULL == logfile)
  {
	return -1;
  }
  while(!feof(srcfile))
  {
    memcpy(name_old,name,sizeof(name));
    memset(line,0,strlen((char *)line) + 2);
    memset(name,0,strlen((char *)name) + 2);
    ret = fgets((char *)line,MAX_ASCII,srcfile);
    if(ret == NULL)//�������ļ�ĩβ������ѭ��
    {
      break;
    }
    //printf("%s",line);
    line_deal(line);

	
    //printf("%s",path);
    //printf("%s",name);
    //printf("%s",name_old);
    //printf("line:%s",line);
    //printf("\n");
	sprintf((char *)buffer,"%d,%d,%d\n",line_cnt,symbol_cnt,dir_cnt);
	fputs((char *)buffer,logfile);
	
	fputs((char *)path,destfile);
	fputs((char *)name,destfile);
	fputs((char *)"\n",destfile);
  }
  fclose(srcfile);
  fclose(destfile);
  // ֻ�����ļ����ִ����ʱ��Ž������������Ϣ
  if (0 != error_line_cnt)
  {
	printf("�ļ���%d�У�����·����%d�С�\n",line_cnt,error_line_cnt);
  }
}

void demo()
{
	printf("ʹ�÷���Ϊ��tree2path-c.exe src_tree.txt  dst_path.txt��");
}


int main(int argc, char *argv[])
{
	if(argc <= 1)
	{
		demo();
	//	return 0;
	}
  /*if (argc >= 3)
  {
    if (strcmp("linux",argv[3]))
    {
      symbol_cnt_per_change_dir = LINUX_SYMBOL_CNT_PER_CHANGE_DIR;
    }
  }*/
  printf("��������������\r\n");
  strcpy((char*)srcfile_str,argv[1]);
  strcpy((char*)destfile_str,argv[2]);
  file_deal(srcfile_str,destfile_str);
  //file_deal((unsigned char*)"3478_tree.txt",(unsigned char*)"3478_path.txt");
  file_deal((unsigned char *)argv[1],(unsigned char *)argv[2]);
  printf("��������������\r\n");
  system("pause");
  return 0;

}
