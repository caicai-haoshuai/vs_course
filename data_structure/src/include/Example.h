#pragma once
#include "basic.h"
#include "SqStack.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                                                 迷宫求解                                                          ////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////Solving with  SqStack////////////////////////////////////////////////

typedef struct
{
	int i;	//当前方块的行号
	int j;	//当前方块的列号
	int di;	//di是下一可走相邻方位的方位号
} Box;		//定义方块类型

int mg[8 + 2][8 + 2] =
{
	{ 1, 1,1,1,1,1,1,1,1,  1 },
	{ 1, 0,0,1,0,0,0,1,0,  1 },
	{ 1, 0,0,1,0,0,0,1,0,  1 },
	{ 1, 0,0,0,0,1,1,0,0,  1 },
	{ 1, 0,1,1,1,0,0,0,0,  1 },
	{ 1, 0,0,0,1,0,0,0,0,  1 },
	{ 1, 0,1,0,0,0,1,0,0,  1 },
	{ 1, 0,1,1,1,0,1,1,0,  1 },
	{ 1, 1,0,0,0,0,0,0,0,  1 },
	{ 1, 1,1,1,1,1,1,1,1,  1 }
};

bool mgpath1(int xi, int yi, int xe, int ye)	//求解路径为:(xi,yi)->(xe,ye)
{
	Box path[MAX_SIZE], e;  int i, j, di, i1, j1, k;   bool find;
	SqStack<Box> *st = 0;			//定义栈st
	InitStack(st);				//初始化栈顶指针
	e.i = xi; e.j = yi; e.di = -1;			//设置e为入口
	Push(st, e);				//方块e进栈
	mg[xi][yi] = -1;	//入口的迷宫值置为-1避免重复走到该方块

	while (!StackEmpty(st))		//栈不空时循环
	{
		GetTop(st, e);		//取栈顶方块e
		i = e.i; j = e.j; di = e.di;
		if (i == xe && j == ye)		//找到了出口,输出该路径
		{
			printf("一条迷宫路径如下:\n");
			k = 0;
			while (!StackEmpty(st))
			{
				Pop(st, e);		//出栈方块e
				path[k++] = e;	//将e添加到path数组中
			}
			while (k >= 1)
			{
				k--;
				printf("\t(%d,%d)", path[k].i, path[k].j);
				if ((k + 2) % 5 == 0)  //每输出每5个方块后换一行
					printf("\n");
			}
			printf("\n");
			DestroyStack(st);//销毁栈
			return true;	//输出一条迷宫路径后返回true
		}
		find = false;
		while (di<4 && !find)   //找相邻可走方块(i1,j1)
		{
			di++;
			switch (di)
			{
			case 0:i1 = i - 1; j1 = j;   break;
			case 1:i1 = i;   j1 = j + 1; break;
			case 2:i1 = i + 1; j1 = j;   break;
			case 3:i1 = i;   j1 = j - 1; break;
			}
			if (mg[i1][j1] == 0)  find = true;
			//找到一个相邻可走方块，设置find为真
		}
		if (find)  		//找到了一个相邻可走方块(i1,j1)
		{
			st->data[st->top].di = di;	  //修改原栈顶元素的di值
			e.i = i1; e.j = j1; e.di = -1;
			Push(st, e);		  //相邻可走方块e进栈
			mg[i1][j1] = -1;
			//(i1,j1)迷宫值置为-1避免重复走到该方块
		}
		else			//没有路径可走,则退栈
		{
			Pop(st, e);	//将栈顶方块退栈
			mg[e.i][e.j] = 0;
			//让退栈方块的位置变为其他路径可走方块
		}
	}
	DestroyStack(st);	//销毁栈
	return false;		//表示没有可走路径
}

void mgpath_SqStack()
{
	if (!mgpath1(1, 1, 8, 8)) {
		printf("该迷宫问题没有解!");
	}
}

//////////////////////////////////////////Solving with  SqQueue///////////////////////////////////////

typedef struct {
	int i;
	int j;
	int pre;
}Box1;

int mg1[8 + 2][8 + 2] =
{
	{ 1, 1,1,1,1,1,1,1,1,  1 },
	{ 1, 0,0,1,0,0,0,1,0,  1 },
	{ 1, 0,0,1,0,0,0,1,0,  1 },
	{ 1, 0,0,0,0,1,1,0,0,  1 },
	{ 1, 0,1,1,1,0,0,0,0,  1 },
	{ 1, 0,0,0,1,0,0,0,0,  1 },
	{ 1, 0,1,0,0,0,1,0,0,  1 },
	{ 1, 0,1,1,1,0,1,1,0,  1 },
	{ 1, 1,0,0,0,0,0,0,0,  1 },
	{ 1, 1,1,1,1,1,1,1,1,  1 }
};

template <typename T> void Disp(SqQueue<T> *qu, int front)
{
	int k = front, j, ns = 0;
	cout << endl;
	do {
		j = k;
		k = qu->data[k].pre;
		qu->data[j].pre = -1;
	} while (k != 0);
	cout << "一条迷宫通路如下：" << endl;
	k = 0;
	while (k < MAX_SIZE) {
		if (qu->data[k].pre == -1) {
			ns++;
			cout << '\t' << '(' << qu->data[k].i << ',' << qu->data[k].j << ')';
			if (ns % 5 == 0) {
				cout << endl;
			}
		}
		k++;
	}
	cout << endl;
}

bool mgpath2(int xi, int yi, int xe, int ye)
{
	Box1 e;
	int  i, j, di, i1, j1;
	SqQueue<Box1> *qu;
	InitQueue(qu);
	e.i = xi; e.j = yi; e.pre = -1;
	enQueue(qu, e);
	mg1[xi][yi] = -1;
	while (!QueueEmpty(qu)) {
		deQueue(qu, e);
		i = e.i;
		j = e.j;
		if (i == xe && j == ye) {
			Disp(qu, qu->front);
			DestroyQueue(qu);
			return true;
		}
		for (di = 0; di < 4; di++) {
			switch (di)
			{
			case 0:i1 = i - 1; j1 = j;   break;
			case 1:i1 = i;   j1 = j + 1; break;
			case 2:i1 = i + 1; j1 = j;   break;
			case 3:i1 = i;   j1 = j - 1; break;
			}
			if (mg1[i1][j1] == 0) {
				e.i = i1; e.j = j1;
				e.pre = qu->front;
				enQueue(qu, e);
				mg1[i1][j1] = -1;
			}
		}
	}
	DestroyQueue(qu);
	return false;
}

void mgpath_SqQueue()
{
	if (!mgpath2(1, 1, 8, 8)) {
		cout << "该迷宫没有解！" << endl;
	}
}