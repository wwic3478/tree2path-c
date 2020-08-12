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
ANSI文件编码，实现树形目录与文件路径的转换
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
char name[MAX_ASCII]; 

//将路径拼接起来
//未完成
void path_add(char* dir_name ,int len)
{
	strcat(path,dir_name);
	//printf("path+:%s+%d+%d+\n",path,path_len,strlen(path));
} 

void path_del()
{
	--path_len;
	path[path_len] = '\0';
	while(path[--path_len] != '/')
	{
		//if(path_len <= 0)
		{
		//	error_flag = 1;//出现错误，直接结束程序 
		//	return;
		}
		path[path_len] = '\0';
	}
	path[++path_len] = '\0';
	//printf("path-:%s-%d-%d-\n",path,path_len,strlen(path));
	return ;
}

//"├",169,192
//"│",169,166
//"└",169,184
//"─",169,164
//获取文件、文件夹名字，已完成。
void get_name(unsigned char* _line,unsigned char* _name)
{
	int dir_flag = 0;
	int space_cnt = 0; 
	//过度掉前面的字符 
	while(*_line != '\0')
	{
		//不具有文件夹属性 
		if( ((*_line == 169) && *(_line+1) == 166)	//"│",169,166 
		 || ((*_line == 169) && *(_line+1) == 164))	//"─",169,164
		 {
		 	_line += 2;
		 }
		 //具有文件夹属性 
	 	else if(	((*_line == 169) && *(_line+1) == 192)	//"├",169,192 
		 ||	((*_line == 169) && *(_line+1) == 184))	//"└",169,184 
		 {
		 	dir_flag = 1;
			_line += 2;
		 }
		 else if((*_line ==  32))
		 {
		 	++_line;
		 	++space_cnt;
		 }
		 else
		 {
		 	//前面只能出现 "│""─"/"├""└"" "这些字符，否则认为前置字符结束 
		 	break;
		 }
	}
	//printf("space_cnt:%d\t",space_cnt);
	//printf("cnt :%d\n",cnt);
	//printf("%d,%d,%d,%d,%d,%\n",*line,*(line+1),*(line+2),*(line+3),*(line+4),*(line+5));
	//前置字符结束后，遇到换行、文件夹标识符、结束符，就认为名字复制完成。
	while( *_line != '\n' 
		&& *_line != '/'	
		&& *_line != '\0' )
	{
		*_name++ = *_line++;
	}
	if(dir_flag == 1)
	{
		*_name++ = '/';
	}
} 

//获取当前文件夹层数
//未完成
unsigned char get_dir_cnt(unsigned char* _line)
{
	int _dir_cnt = 0;
	while(*_line != '\0')
	{
		if(	((*_line == 169) && *(_line+1) == 192)	//"├",169,192 
		 || ((*_line == 169) && *(_line+1) == 166))	//"│",-87,-90 
		 {
			++_dir_cnt;
		 }
		 ++_line;
	}
	return _dir_cnt;
}

//每一行进行处理
//未完成。
int calc_line(char* line)
{
	int len_dis = 2;
	return len_dis;
}

int main(int argc, char *argv[])
{
	char* ret;
	//srcfile = fopen(argv[1],"r");//只读方式打开tree文件
	srcfile = fopen("./src.txt","r");
	//destfile = fopen("2019-.txt","w+");//只写方式打开文件
	while(!feof(srcfile))
	{
		memset(line,0,strlen(line));
		memset(name,0,strlen(name));
		ret = fgets(line,MAX_ASCII,srcfile); 
		if(ret == NULL)//遇到了文件末尾，跳出循环 
		break;
		line_cnt ++;
		//printf("%d,%d:",line[0],line[1]);
		get_name(line,name);
		//dir_cnt = get_dir_cnt(line);
		//printf("%d\n",dir_cnt);
		//printf("line:%s",line);
		printf("%s\n",name);
		//calc_line(line);
	}
	fclose(srcfile);
	//fclose(destfile);
	printf("文件共%d行；错误路径有%d行。\n",line_cnt,error_cnt);
	return 0;

}
