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
ANSI文件编码，实现树形目录与文件夹目录的转换
├── /
│   ├── KTV/
│   │   ├── DVD歌库【26.4T-19.2万】/
│   │   │   ├── 14-15年部分更新/
│   │   │   │   ├── 14.07/
│   │   │   │   │   ├── 3秒乐团-ALL-国语-970889.mpg
│   │   │   │   │   ├── A SHOW-谁说我不在乎你人-国语-970684.mpg
│   │   │   │   │   ├── A.SA 乐团-过客角色-国语-970891.mpg

/t/
/t/KTV/
/t/KTV/DVD歌库【26.4T-19.2万】/
/t/KTV/DVD歌库【26.4T-19.2万】/14-15年部分更新/
/t/KTV/DVD歌库【26.4T-19.2万】/14-15年部分更新/14.07/
/t/KTV/DVD歌库【26.4T-19.2万】/14-15年部分更新/14.07/3秒乐团-ALL-国语-970889.mpg
/t/KTV/DVD歌库【26.4T-19.2万】/14-15年部分更新/14.07/A SHOW-谁说我不在乎你人-国语-970684.mpg
/t/KTV/DVD歌库【26.4T-19.2万】/14-15年部分更新/14.07/A.SA 乐团-过客角色-国语-970891.mpg
/t/KTV/DVD歌库【26.4T-19.2万】/14-15年部分更新/14.07/A.SA 乐团-为爱活一遍-国语-970890.mpg
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
{//		/0214/20190116-音/	18
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

int calc_line(char* line)
{
	int len_dis = 2;
	int old = 0;
	int name_dis = 0;
	static int name_dis_old = 0;
	int dir_in = 0;
	dir_cnt = 0;
	line_len = strlen(line);
	//前面处理路径问题 
	while(1)
	{
		if(line[len_dis-2] == '\n' || (line[len_dis - 2] == -87 && line[len_dis - 1] == -92))//"─"
			break; 
		if(line[len_dis-2] == -87 && line[len_dis-1] == -90)//│符号
		{
			dir_cnt ++ ;
		}
		len_dis++;
	}

	if((line[0] == -87 && line[1] == -64)	//"├"，-87，-64 
		 || (line[0] == -87 && line[1] == -90))//│符号
	{
		//再次处理文件名/文件夹名问题 
		while(line[++len_dis] != '\n')
		{
			if((line[len_dis - 3] == -87 && line[len_dis - 2] == -92) && line[len_dis - 1] == ' ')
			{//"─" + 空格符号，表明遇到了文件夹名称或者文件名称 
				name_dis = len_dis;//名称首字母下标下标 
				//接下来对文件名或者文件夹名进行复制 
				if(dir_cnt < dir_cnt_old)//这层比上层浅 
				{
					int out_cnt = dir_cnt_old - dir_cnt;
					while(out_cnt--)
						path_del();
				}
				else if(dir_cnt > dir_cnt_old)// 这层比上层深 
				{
				 	path_add(line_old,name_dis_old);//加长路径 [字符串起始地址，下标]
				} 
				dir_cnt_old = dir_cnt;//赋值 
				
				file_len = 0;
				while(line[len_dis] != '\n')
					file[file_len++] = line[len_dis++];
				file[file_len] = '\0';
				printpath();
				printfile();
				
				break;//直接结束，不可能有别的符号了 
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
	srcfile = fopen(argv[1],"r");//只读方式打开tree文件
//	destfile = fopen("2019-.txt","w+");//只写方式打开文件
	//int offset = 0;
	//while(offset -- )
	{
	//	fgets(line,MAX_ASCII,srcfile); 
	}
	while(!feof(srcfile))
	{
		memset(line,strlen(line),0);
		ret = fgets(line,MAX_ASCII,srcfile); 
		if(ret == NULL)//遇到了文件末尾，跳出循环 
			break;
		line_cnt ++;
		//printf("%d,%d:",line[0],line[1]);
		calc_line(line);
	}
	fclose(srcfile);
	//fclose(destfile);
	printf("文件共%d行；错误路径有%d行。\n",line_cnt,error_cnt);
	return 0;

}
