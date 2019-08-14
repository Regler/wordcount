# wordcount

	词频统计程序 C语言 采用26链表数组，分别对应a到z开头的单词，最后链表拼接到第一条链表上，大大节约查找时间，效率高。

	-l, --sort by location no_arguement
	
	-d, --sort by dictionary no_arguement
	
	-n, --sort by number no_arguement
	
 	 -i, --ingore Case and case no_arguement
  
	-r, --reverse the list no_arguement
	
	-h, --help for more help no_arguement
	
	-w, --write to file require_arguement
	
	参数ldn互斥，先输出参数最后输出文件名， 可以多文件一起，最后在写文件 -w 接写文件名 
  
  举例:
  
 	 ./wordcount -idr Herry.txt test1.txt test2.txt -w Herrycount.txt test1count.txt test2count.txt
  
	  按照字典序反序输出 且把文件统计信息分别写道对应的文件中。
  
 	 不写i 默认不忽略大小写
  
 	 不写r 默认正序输出
  
  
