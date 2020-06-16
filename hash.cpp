#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define ID_MAX_SIZE 20	  //学生ID的最大长度
#define NAME_MAX_SIZE 20	  //学生名的最大长度
#define HASHSIZE 1024 //定义表长
#define ok 1
#define error -1
typedef int Status;
typedef char RE[NAME_MAX_SIZE];
typedef struct //记录结构体
{
	RE name;
	RE id;
	RE age;
} Record;
struct Node
{
	Record *elem;
	struct Node *next;
};

//定义散列表
typedef struct
{
	Node *L;   //数据链表的头指针
	int count; //当前数据元素的个数
	int size;  //当前的容量
} HashTable;
//关键字比较函数, 关键词相等的时候返回1；关键词不相等的时候返回-1；
Status eq(RE x, RE y)
{
	if (strcmp(x, y) == 0)
		return ok;
	else
		return error;
}
Status NUM_BER; //记录的个数

//初始化链表
Node *linklist_initialize(void)
{
	struct Node *L = (struct Node *)malloc(sizeof(struct Node));
	struct Node *l = L;

	for (int i = 0; i < HASHSIZE - 1; i++)
	{
		L->elem = NULL;
		L->next = (struct Node *)malloc(sizeof(struct Node));
		L = L->next;
		L->next = NULL;
	}
	return l;
}
//设置链表元素
void Set(Node *L, Record *a, int n)
{
	for (int i = 0; i < n; i++)
	{
		L = L->next;
	}
	L->elem = a;
}
//取链表元素
Record *Get(Node *L, int n)
{
	for (int i = 0; i < n; i++)
	{
		L = L->next;
	}
	return L->elem;
}
//输入函数 换成文件输入（用powershell 的方法输入 会方便很多）
void getinmes(Record *a)
{
	printf("需要输入的学生个数：\n");
	scanf("%d", &NUM_BER);
	int i;
	for (i = 0; i < NUM_BER; i++)
	{

		printf("请输入第%d个记录的学生姓名：\n", i + 1);
		scanf("%s", a[i].name);
		printf("请输入第%d个记录的学生ID：\n", i + 1);
		scanf("%s", a[i].id);
		printf("请输入第%d个记录的学生年龄：\n", i + 1);
		scanf("%s", a[i].age);
	}
}
void showInformation(Record *a)
{
	int i;
	for (i = 0; i < NUM_BER; i++)
	{
		printf("\n第%d个学生信息：\n学生姓名：%s\n 学生id：%s\n 年龄：%s\n", i + 1, a[i].name, a[i].id, a[i].age);
	}
}
void clear_screen(Record *a) //清屏
{
	system("cls");
}
//把学生名进行折叠处理
long fold(RE s)
{
	char *p;
	long sum = 0;
	RE ss; //创建一个新的字符串copy原字符串 然后改变原来字符串的大小写
	strcpy(ss, s);
	strupr(ss);
	p = ss;
	while (*p != '\0')
	{
		sum += *p++;
	}
	printf("\n 表的地址总数%d", sum);
	return sum;
}
//建立关于学生名的散列表
int Hash1(RE str)
{
	long n;
	int m;
	n = fold(str);	  //先进行学生名的折叠处理
	m = n % HASHSIZE; //折叠处理后以整数的形式，然后用除余留数法就可以构造散列函数
	return m;		  //返回模值
}
//建立学生id的散列函数
int Hash2(RE str)
{
	long n;
	int m;
	n = atoi(str);	  //把字符串转换成整型的函数
	m = n % HASHSIZE; //除余留数法构造散列函数；
	return m;		  //返回模值
}
//处理冲突，采用开放地址法的二次探测再散列法解决冲突
Status collision(int p, int &c)
{
	int i, q;
	i = c / 2 + 1;
	while (i < HASHSIZE)
	{
		if (c % 2 == 0)
		{
			c++;
			q = (p + i * i) % HASHSIZE;
			if (q >= 0)
			{
				return q;
			}
			else
			{
				i = c / 2 + 1;
			}
		}
		else
		{
			q = (p - i * i) % HASHSIZE;
			c++;
			if (q > 0)
			{
				return q;
			}
			else
			{
				i = c / 2 + 1;
			}
		}
	}
	return error;
}

//以学生名字为关键字来建表，建立相应的散列表，并有处理冲突的机制
void create_hashtable_by_name(HashTable *H, Record *a)
{
	int i, p = -1, c, pp;
	for (i = 0; i < NUM_BER; i++)
	{
		c = 0;
		p = Hash1(a[i].name);
		pp = p;
		while (Get(H->L, pp) != NULL)
		{
			pp = collision(p, c);
			if (pp < 0)
			{
				printf("第%d记录无法解决冲突", i + 1); //需要显示冲突次数时输出
				continue;
			} //无法解决冲突 跳入下一个循环（i++）
		}
		Set(H->L, &(a[i]), pp);
		H->count++;
		printf("第%d个记录冲突为%d。\n", i + 1, c); //需要显示冲突次数时输出
	}
	printf("\n建表完成!\n此散列表容量为为%d，当前表内存储的记录个数为%d.\n", HASHSIZE, H->count);
}
//以学生id为关键字来建表，并解决相应的冲突
void create_hashtable_by_id(HashTable *H, Record *a)
{
	int i, p = -1, c, pp;
	for (i = 0; i < NUM_BER; i++)
	{
		c = 0;
		p = Hash2(a[i].id);
		pp = p;
		while (Get(H->L, pp) != NULL)
		{
			pp = collision(p, c);
			if (pp < 0)
			{
				printf("第%d记录无法解决冲突", i + 1); //需要显示冲突次数时输出
				continue;
			} //无法解决冲突 跳入下一个循环（i++）
		}
		Set(H->L, &(a[i]), pp);
		H->count++;
		printf("第%d个记录冲突为%d。\n", i + 1, c); //需要显示冲突次数时输出
	}
	printf("\n建表完成!\n此散列表容量为为%d，当前表内存储的记录个数为%d.\n", HASHSIZE, H->count);
}
//通过学生名查找，若查找成功，显示信息
//c用来显示冲突的次数
void search_hashtable_name(HashTable *H, int &c)
{
	RE str;
	printf("\n请输入要查找的记录的学生姓名：\n");
	scanf("%s", str);
	int p, pp;
	p = Hash1(str);
	pp = p;
	while (Get(H->L, pp) != NULL && (eq(str, Get(H->L, pp)->name) == -1))
	{
		pp = collision(p, c);
	}
	if (Get(H->L, pp) != NULL && eq(str, Get(H->L, pp)->name) == 1)
	{
		printf("\n查找成功！\n查找过程冲突次数为%d.以下是您需要查找的信息：\n\n", c);
		printf("学生姓名：%s\n学生ID：%s\n学生年龄：%s\n", Get(H->L, pp)->name, Get(H->L, pp)->id, Get(H->L, pp)->age);
	}
	else
	{
		printf("\n此人不存在，查找不成功！\n");
	}
}
// 在管理系统中按照学生id为关键字进行查找，若查找成功，显示信息
//c用来记录冲突次数，查找成功的时候显示冲突次数
void search_hashtable_id(HashTable *H, int &c)
{
	RE idn;
	printf("\n请输入要查找的记录的学生ID：\n");
	scanf("%s", idn);
	int p, pp;
	p = Hash2(idn);
	pp = p;
	while ((Get(H->L, pp) != NULL) && (eq(idn, Get(H->L, pp)->id) == -1))
	{
		pp = collision(p, c);
	}
	if (Get(H->L, pp) != NULL && eq(idn, Get(H->L, pp)->id) == 1)
	{
		printf("\n查找成功！\n查找过程冲突次数为%d.以下是您需要查找的信息：\n\n", c);
		printf("学生姓名：%s\n学生ID：%s\n学生年龄：%s\n", Get(H->L, pp)->name, Get(H->L, pp)->id, Get(H->L, pp)->age);
	}
	else
	{
		printf("\n此人不存在，查找不成功！\n");
	}
}
void delete_hashtable_name(HashTable *H, int &c)
{
	RE str;
	printf("\n请输入要删除的记录的学生姓名：\n");
	scanf("%s", str);
	int p, pp;
	int index = Hash1(str);
	p = Hash1(str);
	pp = p;
	while ((Get(H->L, pp) != NULL) && (eq(str, Get(H->L, pp)->name) == -1))
	{
		pp = collision(p, c);
	}
	if (Get(H->L, pp) != NULL && eq(str, Get(H->L, pp)->name) == 1)
	{
		{
			for (pp = p; pp <= H->size - 2; pp++)
			{
				Set(H->L, Get(H->L, pp + 1), pp); //结点前移
			}
			H->size--; //表长度减1
			printf("\n删除完成\n");
		}
	}
	else
	{
		printf("\n此人不存在，删除不成功\n");
	}
}
void delete_hashtable_id(HashTable *H, int &c)
{
	RE idn;
	printf("\n请输入要删除的记录的学生ID：\n");
	scanf("%s", idn);
	int p, pp;
	int index = Hash1(idn);
	p = Hash2(idn);
	pp = p;
	while ((Get(H->L, pp) != NULL) && (eq(idn, Get(H->L, pp)->id) == -1))
	{
		pp = collision(p, c);
	}
	if (Get(H->L, pp) != NULL && eq(idn, Get(H->L, pp)->id) == 1)
	{
		{
			for (pp = p; pp <= H->size - 2; pp++)
			{
				Set(H->L, Get(H->L, pp + 1), pp); //结点前移
			}
			H->size--; //表长度减1
			printf("\n删除完成\n");
		}
	}
	else
	{
		printf("\n此人不存在，删除不成功\n");
	}
}
void hashtable_insert(HashTable *H, int &c, Record *a)
{
	NUM_BER++;
	printf("请输入需要插入的学生姓名：\n", NUM_BER);
	scanf("%s", a[NUM_BER].name);
	printf("请输入需要插入的学生ID：\n", NUM_BER);
	scanf("%s", a[NUM_BER].id);
	printf("请输入需要插入的学生年龄：\n", NUM_BER);
	scanf("%s", a[NUM_BER].age);
	int i, p = -1, pp;
	c = 0;
	p = Hash1(a[NUM_BER].name);
	pp = p;
	while (Get(H->L, pp) != NULL)
	{
		pp = collision(p, c);
		if (pp < 0)
		{
			printf("无法解决冲突");
			continue;
		} //无法解决冲突 跳入下一个循环
	}
	Set(H->L, &(a[NUM_BER]), pp);
	H->count++;
	printf("插入成功");
}
//保存数据的函数
void Save()
{
	FILE *fp;
	if ((fp = fopen("c:\test.txt", "w")) == NULL)
	{
		printf("\n ERROR opening customet file ");
	}
	fclose(fp);
}
int main(int argc, char *argv[])
{
	system("chcp 65001 > nul");
	int c, flag = 1;
	HashTable *H;
	H = (HashTable *)malloc(sizeof(HashTable));
	H->L = linklist_initialize();
	for (int i = 0; i < HASHSIZE; i++)
	{
		Set(H->L, NULL, i);
	}
	H->size = HASHSIZE;
	H->count = 0;
	Record a[ID_MAX_SIZE];
	printf("欢迎使用基于 HashTable 的学生信息管理系统");
	printf("\n1.\t添加学生信息");
	printf("\n2.\t读取学生信息");
	printf("\n3.\t以学生姓名建表");
	printf("\n4.\t以学生ID建表");
	printf("\n5.\t输入学生的姓名进行查找");
	printf("\n6.\t输入学生的ID进行查找");
	printf("\n7.\t输入学生的姓名进行删除");
	printf("\n8.\t输入学生的ID进行删除");
	printf("\n9.\t以学生的姓名进行插入");
	printf("\n10.\t清屏");
	printf("\n11.\t保存");
	printf("\n12.\t退出");
	printf("\nAttention：");
	printf("\nⅠ.在进行3操作之后才能进行5，7，9操作");
	printf("\nⅡ.进行6，7操作的时候，请先4进行操作");
	printf("\n请输入你想要的操作：\n");
	while (1)
	{
		int num;
		scanf("%d", &num);
		switch (num)
		{
		case 1:
			getinmes(a);
			break;
		case 2:
			showInformation(a);
			break;
		case 3:
			create_hashtable_by_name(H, a); //以姓名建立散列表；
			break;
		case 4:
			create_hashtable_by_id(H, a); //以学生ID建表
			break;
		case 5:
			c = 0;
			search_hashtable_name(H, c); //以姓名来查找；
			break;
		case 6:
			c = 0;
			search_hashtable_id(H, c); //以学生ID来查找；
			break;
		case 7:
			c = 0;
			delete_hashtable_name(H, c); //以姓名来删除；
			break;
		case 8:
			c = 0;
			delete_hashtable_id(H, c); //以学生ID来删除；
			break;
		case 9:
			c = 0;
			hashtable_insert(H, c, a);
			break;
		case 10:
			clear_screen(a);
			break;
		case 11:
			Save();
			break;
		case 12:
			return 0;
			break;
		default:
			printf("输入错误，请重新输入！");
			printf("\n");
		}
	}
	system("pause");
	return 0;
}
