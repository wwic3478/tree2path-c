#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ASCII	99999

//"├",169,192
//"│",169,166
//"└",169,184
//"─",169,164

#define VERTICAL_SYMBOL "│"
#define HORIZONTAL_SYMBOL "─"
#define BRANCH_SYMBOL "├"
#define BROKEN_SYMBOL "└"
#define DIR_SYMBOL '/'
//但是数据不一定是正常的。
//每进入或出来一个文件夹时，改变的前面的符号的个数，Windows有4个，linux有5个
//（使用termux的tree命令）安卓：0，7，12，17，22，27，
//bpcs（bpcs tree /f >）：0，7，12，17，22，27，
//ubuntu（/f）：0，7，12，17，22，27，
//centos（/f）：0，7，12，17，22，27，
//windows（cmd的tree /f）：0，4，8，12，16，28

#define WINDOWS_SYMBOL_CNT_PER_CHANGE_DIR (4)
#define LINUX_SYMBOL_CNT_PER_CHANGE_DIR (5)

#define printline()	printf("%s",line)
#define printpath()	printf("%s",path)
#define printname()	printf("%s\n",name)
#define printfpath()	write(destfile,path,path_len)
#define printffile()	write(destfile,file,file_len)

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
/*
ANSI文件编码，实现树形目录与文件路径的转换
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

//将路径拼接起来
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
			error_flag = 1;//出现错误，直接结束程序
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
  //过渡掉前面的字符
  symbol_cnt = 0;
  while(*_line != '\0')
  {
    if(((*_line == 169) && *(_line+1) == 166)	//"│",169,166
    || ((*_line == 169) && *(_line+1) == 164)	//"─",169,164
    || ((*_line == 169) && *(_line+1) == 192)	//"├",169,192
    || ((*_line == 169) && *(_line+1) == 184))	//"└",169,184
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
      //前面只能出现 "│""─"/"├""└"" "这些字符，否则认为前置字符结束
      break;
    }
  }
  //dir_cnt = (symbol_cnt-7)/5;
  //windows
  //dir_cnt = symbol_cnt / 4;
  //symbol_cnt -= 2;
  // 如果出现空行，则不出现故障
  if (symbol_cnt / symbol_cnt_per_change_dir != 0)
	  dir_cnt = symbol_cnt / symbol_cnt_per_change_dir;
  //printf("space_cnt:%d\t",symbol_cnt);
  //printf("%s",_line);
  //printf("cnt :%d\n",cnt);
  //printf("%d,%d,%d,%d,%d,%\n",*line,*(line+1),*(line+2),*(line+3),*(line+4),*(line+5));
  //前置字符结束后，遇到换行、文件夹标识符、结束符，就认为名字复制完成。
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


//每一行进行处理
//未完成。
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
  srcfile = fopen((char *)_srcfile,"r");//只读方式打开tree文件，如果没有就打开失败。
  destfile = fopen((char *)_destfile,"w+");//只写方式打开文件，如果没有文件就创建文件。
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
    if(ret == NULL)//遇到了文件末尾，跳出循环
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
  // 只有在文件出现错误的时候才进行输出错误信息
  if (0 != error_line_cnt)
  {
	printf("文件共%d行；错误路径有%d行。\n",line_cnt,error_line_cnt);
  }
}

void demo()
{
	printf("使用方法为“tree2path-c.exe src_tree.txt  dst_path.txt”");
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
  printf("程序正常启动。\r\n");
  strcpy((char*)srcfile_str,argv[1]);
  strcpy((char*)destfile_str,argv[2]);
  file_deal(srcfile_str,destfile_str);
  //file_deal((unsigned char*)"3478_tree.txt",(unsigned char*)"3478_path.txt");
  file_deal((unsigned char *)argv[1],(unsigned char *)argv[2]);
  printf("程序正常结束。\r\n");
  system("pause");
  return 0;

}
