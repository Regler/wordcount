#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
#include <stdio.h>


typedef struct myNode
{
	void * data;
    struct myNode *prior;
	struct myNode *next;
} MyNode;

typedef struct myList
{
	MyNode * first;
	MyNode * last;
	int length;
} MyList;


//创建链表
MyList * createMyList();


//释放链表
void freeMyList(MyList * list);//, void* data, int length,  void(*freeData)(void *, int));

//插入在尾部
MyNode* myListInsertDataAtLast(MyList* const list, void* const data);

//插入在首部
void myListInsertDataAtFirst(MyList * const list, void* const data);

//在某个节点前插入
void myListInsertDataAtNodeprior(MyList* const list, void* const data, MyNode *p);

//插入
int myListInsertDataAt(MyList * const list, void* const data, int index);

//删除在尾部
void* myListRemoveDataAtLast(MyList* const list);

//删除在首部
void* myListRemoveDataAtFirst(MyList * const list);

//删除
void* myListRemoveDataAt(MyList* const list, int index);

//长度
int myListGetSize(const MyList * const list);

//打印
void myListOutput(const MyList * const list, void(*pt)(const void * const));

//查询节点
MyNode* myListFindDataNode( MyList * list, void *  ,int(*pt)( void * ,  void * ));

//查询所有节点
MyList* myListFindDataAllNode(const MyList * const list,const void * const ,int(*pt)(const void * const,const void * const));

//反向打印
void myListOutput_reverse(const MyList * const list, void (*f)(const void * const));

//取得数据
void* myListGetDataAt(const MyList * const list, int index);

//取得第一个数据
void* myListGetDataAtFirst(const MyList * const list);

//取得最后一个数据
void* myListGetDataAtLast(const MyList * const list);

//快速排序
void  myListQuickSort(MyList * const list,int(*pt)(const void * const ,const void * const));

//插入排序
void myListInsertSort(MyList *const list, int (*cmp)(const void * const, const void * const));

//链表反向
void myListReverse(MyList *list);

//链表的拼接
MyList *MyListSplicing(MyList *list, int length);

#endif // LIST_H_INCLUDED
