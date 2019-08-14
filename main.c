#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>


typedef struct wordcount
{
	char word[30];
	int count;
	int location;
}Wordcount;


//创建节点
void* creatcw(char *word, int location)
{
	Wordcount *wc = NULL;
	wc =(Wordcount *) malloc (sizeof(Wordcount));
	if(wc == NULL)  return NULL;
	strcpy(wc->word, word);
	wc->count = 1;
	wc->location = location;
	return wc;
}

//输出词频统计信息
void printWordcount(const void* const  w)
{
	const Wordcount   *wc = (Wordcount *)w;
	printf(" %-20s %d\n", wc->word, wc->count);
}

//忽略大小写
int Ingnorecase(void* a, void * b)
{
	Wordcount *wc1 = (Wordcount *)a;
	char *bb = (char *)b;
	if(strcasecmp(wc1->word, bb) == 0)
		return 1;
	return 0;
}

//不忽略大小写
int notIngnorecase(void * a, void * b)
{
	Wordcount *wc1 = (Wordcount *)a;
	char *bb = (char *)b;
	if(strcmp(wc1->word, bb) == 0)
		return 1;
	return 0;

}

int notIngnorecase1(void * a, void * b)
{
	Wordcount *wc1 = (Wordcount *)a;
	char *bb = (char *)b;
	if (strcmp(wc1->word, bb) <0 )
		return -1;
	else if(strcmp(wc1->word, bb) == 0)
		return 0;
	return 1;

}




//数量增加1
void addcount(void * a)
{
	Wordcount *wc = (Wordcount *)a;
	wc->count++;
}

//按照数量排序
int  sortMethod_count(const void* const p1,const void* const p2)
{
	const Wordcount *wc1 = (Wordcount *)p1;
	const Wordcount *wc2 = (Wordcount *)p2;
	if (wc1->count <=  wc2->count) 
	{    
		return 1;
	}
	return 0;
}

//按照字典序列排序
int sortMethod_word(const void* const p1, const void* const p2)
{
	const Wordcount *wc1 = (Wordcount *) p1;
	const Wordcount *wc2 = (Wordcount *) p2;
	if (strcasecmp(wc1->word, wc2->word) < 0)  
	{
		return 1;
	}
	return 0;
}

//按照位置序排列
int  sortMethod_flag(const void* const p1, const void* const p2)
{
	const Wordcount *wc1 = (Wordcount *)p1;
	const Wordcount *wc2 = (Wordcount *)p2;
	if (wc1->location <=  wc2->location) 
	{    
		return 1;
	}
	return 0;
}


//文件的长度
int filelength(FILE *fp)
{
	int num;
	fseek(fp, 0, SEEK_END);
	num=ftell(fp);
	fseek(fp, 0, SEEK_SET);
	return num;
}

//整块读文件
char *readfile(char *path)
{
	FILE *fp = NULL;
	int length;
	char *ch = NULL;
	if((fp = fopen(path, "r"))==NULL)
	{
		printf("open file %s error.\n", path);
		exit(0);
	}
	length = filelength(fp);
	ch=(char *)malloc(length);
	if (ch == NULL ) 
	{
		fclose(fp);
		return NULL;
	}
	fread(ch, length, 1, fp);
	*(ch + length - 1)='\0';
	fclose(fp);
	fp = NULL;
	return ch;
}



//词频统计原版效率低
//按位置排序
/*
MyList *myListWordcount_location(char *file,int (*is_ingorecase)(void *, void *), void *(*creatwc)(char *), void (*addcount)(void *))
{
	char *seg = "`~!@#$%^&*()-+{}:=;[]\"\',./\\ \?<>1234567890"; 
	FILE *fp = NULL; 
	fp = fopen(file,"r");
	if(fp == NULL)  return NULL;
	MyList *list = createMyList(); 
	char temp[100]; 
	fscanf(fp,"%s",temp); 
	while(!feof(fp)) 
	{ 
		char *p=strtok(temp,seg); 
		while(p) 
		{ 
			MyNode *mynode = myListFindDataNode(list, p, (*is_ingorecase));
			if(mynode == NULL ) 
			{
				void *s = (*creatwc)(p);
				myListInsertDataAtLast(list, s);
			}
			else 
			{
				(*addcount)(mynode->data);

			}

			p=strtok(NULL,seg); 
		} 
		fscanf(fp,"%s",temp); 
	} 
	fclose(fp);
	return list; 
}
*/



//词频统计  改进1
//按位置排序 读文件快一点
MyList *myListWordcount_location1(char *file,int (*is_ingorecase)(void *, void *), void *(*creatwc)(char *), void (*addcount)(void *))
{
	char *seg = " `~!@#$%^&*()-+{}:\n=;[]\"\',./\\ \?<>1234567890\a\b\f\r\t\v"; 
	clock_t start, stop;
	double duration;
	MyList *list = createMyList(); 
	char *string ;
	string = readfile(file);
	start = clock();
	char *p=strtok(string,seg); 
	while(p) 
	{ 
		MyNode *mynode = myListFindDataNode(list, p, (*is_ingorecase));
		if(mynode == NULL ) 
		{
			void *s = (*creatwc)(p);
			myListInsertDataAtLast(list, s);
		}
		else 
		{
			(*addcount)(mynode->data);
		}
		p=strtok(NULL,seg); 
	} 
	free(string);
	stop = clock();
	duration =(double)((stop - start)/1000);
	printf("创建链表需要时间 : %lf\n",duration);
	return list; 
}






//词频统计
//本文用这个
//利用26链表数组存放a到z,大大节约查找时间，最后拼接到第一个list[0]上
MyList *myListWordcount(char *temp, int (*is_ingorecase)(void *, void *), void *(*creatwc)(char *, int ), void (*addcount)(void *))
{

	MyList  *list = NULL;
	list = (MyList *)malloc(sizeof(MyList)*26);
	if(list == NULL)  return NULL;
	for(int i = 0; i < 26 ; i++)
	{

		(&list[i])->first  =  NULL;
		(&list[i])->last = NULL;
		(&list[i])->length = 0;
	}
	int length = strlen(temp);
	char b[100];
	int j = 0;
	int location = 0;
	int index = 0;
	for(int i = 0; i < length; i++)
	{

		if ((temp[i] >= 'a' && temp[i] <= 'z')||(temp[i] >= 'A' && temp[i] <= 'Z'))
			b[j++] = temp[i];
		else 
		{

			b[j] = '\0';
			if(j != 0)
			{

				if (b[0] >= 'A' && b[0] <= 'Z') 
					index = b[0]-'A';	
				else
					index = b[0]-'a';

				MyNode *mynode = list[index].first;		
				while(mynode)
				{

					if((*is_ingorecase)(mynode->data, b)==1)
					{

						(*addcount)(mynode->data);
						break;
					}
					mynode = mynode->next;
				}
				if(mynode == NULL)
				{

					void *s = (*creatwc)(b, location);
					myListInsertDataAtLast(&list[index], s);
					location++;
				}
			}
			j=0;
		}
	}
	b[j] = '\0';
	if(j != 0)
	{

		if (b[0] >= 'A' && b[0] <= 'Z')  index = b[0]-'A';
		else  index = b[0]-'a';

		MyNode *mynode = list[index].first;		
		while(mynode)
		{

			if((*is_ingorecase)(mynode->data, b)==1)
			{

				(*addcount)(mynode->data);
				break;
			}
			mynode = mynode->next;
		}
		if(mynode == NULL)
		{

			void *s = (*creatwc)(b, location);
			myListInsertDataAtLast(&list[index], s);
		}

	}
	free(temp);
//	temp = NULL;
	return list; 
}





//词频统计  
//第二种优化方式 直接字典序排列，不需要便利完所有的链表。但是比较时间多。
MyList *myListWordcount1(char *temp, int (*is_ingorecase)(void *, void *), void *(*creatwc)(char *, int ), void (*addcount)(void *))
{
	MyList  *list = NULL;
	list = (MyList *)malloc(sizeof(MyList)*26);
	if(list == NULL)  return NULL;
	for(int i = 0; i < 26 ; i++)
	{
		(&list[i])->first  =  NULL;
		(&list[i])->last = NULL;
		(&list[i])->length = 0;
	}
	int length = strlen(temp);
	char b[100];
	int j = 0;
	int location = 0;
	int index = 0;
	for(int i = 0; i < length; i++)
	{
		if ((temp[i] >= 'a' && temp[i] <= 'z')||(temp[i] >= 'A' && temp[i] <= 'Z'))
			b[j++] = temp[i];
		else 
		{
			b[j] = '\0';
			if(j != 0)
			{
				if (b[0] >= 'A' && b[0] <= 'Z') 
					index = b[0]-'A';	
				else
					index = b[0]-'a';
				MyNode *mynode = list[index].first;		
				while(mynode && (*is_ingorecase)(mynode->data, b) <= 0 )
				{
					if((*is_ingorecase)(mynode->data, b)==0)
					{
						(*addcount)(mynode->data);
						break;
					}
					mynode = mynode->next;
				}
				if(mynode == NULL)
				{
					void *s = (*creatwc)(b, location);
					myListInsertDataAtLast(&list[index], s);
					location++;
				}
				else if((*is_ingorecase)(mynode->data, b) > 0)
				{

					void *s = (*creatwc)(b,location);
					myListInsertDataAtNodeprior(&list[index], s, mynode);
					location++;
				}
			}
			j=0;
		}
	}
	b[j] = '\0';
	if(j != 0)
	{
		if (b[0] >= 'A' && b[0] <= 'Z')  index = b[0]-'A';
		else  index = b[0]-'a';
		MyNode *mynode = list[index].first;		
		while(mynode && (*is_ingorecase)(mynode->data, b) <= 0)
		{
			if((*is_ingorecase)(mynode->data, b)==0)
			{
				(*addcount)(mynode->data);
				break;
			}
			mynode = mynode->next;
		}
		if(mynode == NULL)
		{
			void *s = (*creatwc)(b, location);
			myListInsertDataAtLast(&list[index], s);
			location++;
		}
		else if((*is_ingorecase)(mynode->data, b) > 0)
		{
			void *s = (*creatwc)(b,location);
			myListInsertDataAtNodeprior(&list[index], s, mynode);
			location++;
		}


	}
	free(temp);
//	temp = NULL;
	return list; 
}




static struct option longOpts[] = {
	{ "location", 0, NULL, 'l' },
	{ "dictionary",0 , NULL, 'd' },
	{ "number",0 , NULL, 'n' },
	{ "reverse",0 , NULL, 'r' },
	{ "help", 0, NULL, 'h' },
	{"file",1 ,NULL, 'f'},
	{"writefile",1 , NULL, 'w'},
	{"ingoreCasecase", 0 , NULL, 'i'},
	{ 0, 0, 0, 0 }
};

//显示帮助信息
void help()
{
	printf("\n\n选项:\n");
	printf("\t-l, --sort by location no_arguement\n");
	printf("\t-d, --sort by dictionary no_arguement\n");
	printf("\t-n, --sort by number no_arguement\n");
	printf("\t-i, --ingore Case and case no_arguement\n");
	printf("\t-r, --reverse the list no_arguement\n");
	printf("\t-h, --help for more help no_arguement\n");
	printf("\t-w, --write to file require_arguement\n");
	printf("\t参数ldn互斥，先输出参数最后输出文件名， 可以多文件一起，最后在写文件 -w 接写文件名 \n\n");
	printf("举例:\n\t./wordcount -di A.txt B.txt C.txt -w D.txt E.txt F.txt\n\n\n");
}


int main(int argc, char *argv[])
{
	clock_t start, stop;
	double duration;
	start = clock();
	char *allword = NULL; //文件内容
	MyList *list[10] = { NULL } ;
	for(int i = 0; i < 10; i++)
		list[i] = NULL;
	char  *suffix;		//后缀，判断是否为txt文件
	int opt;			
	int is_ingorecase = 0 ;  //是否忽略大小写，默认不忽略
	int is_reverse = 0;		//是否反向输出，默认正向
	int sortmethod = 0;		//排序方式，默认不排
	const char *optstring = "ldnrw:ih";
	extern int optind;
	char filename[100][50];
	int filenum = 0;
	int finish = 0;
	char writefilename[100][50];
	while ((opt = getopt_long(argc, argv, optstring, longOpts, NULL)) != -1)
	{		
		switch (opt) 
		{	
			case 'i':	//忽略大小写
				is_ingorecase =1 ;
				break;
			case 'l':					//按照位置排序
				if(sortmethod == 2 || sortmethod == 3)
					printf("已经按照某种方式排序不能在进行这次排序\n");
				else
				{	
					int i = optind - 1 ;
					while(i > 0)
					{
						suffix = strrchr(argv[i], '.');
						if(suffix && strcmp(suffix,".txt")==0)
						{ 
							printf("文件要在-l参数之后输入\n");
							return 0;
						}
						i--;
					}			
					printf("按照位置\n");
					sortmethod = 1;
				}
				break;
			case 'd':					//按照字典排序
				if(sortmethod == 1 || sortmethod ==3)
					printf("已经按照某种方式排序，不能再进行这次排序\n");
				else 
				{
					int i = optind - 1 ;
					while(i > 0)
					{
						suffix = strrchr(argv[i], '.');
						if(suffix && strcmp(suffix,".txt")==0)
						{ 
							printf("文件要在-d参数之后输入\n");
							return 0;
						}
						i--;
					}	
					printf("按照字典序\n");
					sortmethod = 2;
				}
				break;
			case 'n':					//按照次数排序
				if(sortmethod == 1 || sortmethod == 2)
					printf("已经按照某种方式排序，不能进行这次排序\n");
				else 
				{	
					int i = optind - 1 ;
					while(i > 0)
					{
						suffix = strrchr(argv[i], '.');
						if(suffix && strcmp(suffix,".txt")==0)
						{ 
							printf("文件要在-n参数之后输入\n");
							return 0;
						}
						i--;
					}	 
					printf("按照次数\n");
					sortmethod = 3;
				}
				break;
			case 'r':					//反序输出
				printf("反序输出\n");
				is_reverse = 1;
				break;
			case 'w':					//写入文件
				for(int i= 0 ; i<filenum; i++)
				{	if(i+optind-1 >= argc) 
					{
						printf("写入的文件太少\n");
						break;
					}
					strcpy(writefilename[i],argv[i+optind-1]);
					freopen(writefilename[i], "w", stdout);
					if(list[i] == NULL)  return 0;		
					printf("共统计 %d  个单词\n",list[i][0].length);
					if(is_reverse == 0)
					{
						myListOutput(&list[i][0], printWordcount);
					}
					else 
					{
						myListOutput_reverse(&list[i][0], printWordcount);
					}
					freopen("/dev/tty", "w", stdout);
					printf("写入文件: %s ok\n", writefilename[i]);
				}
				finish = 1;
				break;
			case 'h':
				help();
				break;
			default :
				printf("optopt = %c\n", (char)optopt);
				printf("opterr = %d\n", opterr);
				fprintf(stderr, " %s 具体操作见help\n", argv[0]);
				exit(EXIT_FAILURE);
		}
		if(finish == 1 )  break;
		if(optind < argc)
			suffix = strrchr(argv[optind], '.');
		if(suffix == NULL)   continue;	
		while(  strcmp(suffix,".txt")==0)
		{
			strcpy(filename[filenum++],argv[optind]);
			optind++;
			if(optind >= argc)	 break;	
			suffix = strrchr(argv[optind], '.');	
			if(suffix == NULL ) 
			{
				optind--;
				break;
			}
		}
		for(int i=0; i<filenum; i++)
		{
			allword = readfile(filename[i]);
			if(is_ingorecase == 0)
				list[i] = myListWordcount(allword, notIngnorecase, creatcw, addcount );
			else 
				list[i] = myListWordcount(allword, Ingnorecase, creatcw, addcount );
			list[i] = MyListSplicing(list[i], 26);
			if(&list[i][0])
			{
				if(sortmethod == 1)
					myListQuickSort(&list[i][0], sortMethod_flag);
				else if(sortmethod == 2)
					myListQuickSort(&list[i][0], sortMethod_word);
				else if(sortmethod == 3)
					myListQuickSort(&list[i][0], sortMethod_count);
			}

		}

	}

	for(int i=0; i<filenum; i++)
		if (list[i] != NULL)
		{
			printf("文件 %s  :共统计 %d  个单词\n", filename[i], list[i][0].length);
			freeMyList(list[i]);
			list[i] = NULL;
		}
	stop = clock();
	duration = (double)(stop - start)/1000000;
	printf("程序运行时间为：%lf\n",duration);
	fclose(stdout);
	return 0;

}




