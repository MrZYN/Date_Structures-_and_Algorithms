#include<stdio.h>
#include<malloc.h>
#define maxsize 1024 

//建立顺序表 
typedef struct{
	char data[maxsize];
	int last;
}SeqList;
//建立操作数栈
typedef struct{
	float data[maxsize];
	int top;
}NumStack;
//建立运算符栈
typedef struct{
	char data[maxsize];
	int top;
}SymStack;


//顺序表、操作数栈和运算符栈 
void CreateList(SeqList*&L);          //建立顺序表
void InitNumStack(NumStack*&S);       //初始化操作数栈 
int NumEmpty(NumStack *S);            //判断操作数栈空 
int NumPush(NumStack *S,float x);     //入操作数栈 
int NumPop(NumStack *S,float &x);     //出操作数栈 
void InitSymStack(SymStack*&S);       //初始化运算符栈 
int SymEmpty(SymStack *S);            //判断运算符栈空 
int SymPush(SymStack *S,char x);      //入运算符栈 
int SymPop(SymStack *S,char &x);      //出运算符栈

//检查函数
int InputCheck(SeqList*L);            //输入字符合法性检查
int BracketCheck(SeqList*L);          //括号匹配检查
int SymbolCheck(SeqList*L);           //运算符合法性检查

//工具函数 
int Priority(char op);                //运算符优先级赋值
float CTF(char c);                    //数字字符转单精度 
float Calculate(float left,float right,char sym);
    //输入单精度类型的左右操作数和字符类型的运算符，给出计算结果  
void Pop_calculate_push(NumStack *S_num,SymStack *S_sym);
    //弹出两个操作数栈顶元素和一个运算符栈顶元素，执行相应运算，运算结果压回操作数栈顶

//核心函数 
float Change_and_calculate(SeqList*L);//中缀表达式转换为后缀表达式并计算 

int main()
{
	printf("***加减乘除计算器***\n");
	printf("请输入算术表达式（中缀），按ENTER键结束\n");
	printf("注意：仅可以输入个位正数，输入的括号应为英文格式\n");
	
	SeqList *L;
	while(1)
	{
		CreateList(L);                                        //建立顺序表
		if(InputCheck(L) && BracketCheck(L) && SymbolCheck(L))//进行合法性检查 
		    break;                                            //检查成功跳出循环
		else                                                  //否则，重新输入 
        	printf("输入有误，请检查后重新输入!\n");
	} 
	
	float ans;
	ans = Change_and_calculate(L); 
	printf("计算结果：\n%.3f\n",ans);	
	
	return 0;
} 

//建立顺序表
void CreateList(SeqList*&L)
{
	L = (SeqList*)malloc(sizeof(SeqList));
	L->last = 0;
	char ch;
	while((ch=getchar())!='\n')
	{   
		L->data[L->last]=ch;
		L->last++;
	}
}
//操作数链栈 
//初始化栈 
void InitNumStack(NumStack*&S)
{
	S = (NumStack*)malloc(sizeof(NumStack));
	S->top = -1;//初始化栈顶指针 
}
//判断栈空
int NumEmpty(NumStack *S)
{
	if(S->top==-1)//栈空
	    return 1;
	else//不空 
	    return 0; 
}
//新元素入栈
int NumPush(NumStack *S,float x)
{
	if(S->top==maxsize-1)//栈满，报错 
	    return 0;
	S->top = S->top + 1;//指针先加1 
	S->data[S->top] = x;//新元素入栈
	return 1; 
}
//出栈操作
int NumPop(NumStack *S,float &x)
{
	if(S->top==-1)//栈空，报错 
	    return 0;
	x = S->data[S->top];//栈顶元素先出栈 
	S->top = S->top - 1;//指针再减1 
	return 1;
}
//运算符链栈 
//初始化栈 
void InitSymStack(SymStack*&S)
{
	S = (SymStack*)malloc(sizeof(SymStack));
	S->top = -1;
}
//判断栈空
int SymEmpty(SymStack *S)
{
	if(S->top==-1)
	    return 1;
	else
	    return 0; 
} 
//新元素入栈
int SymPush(SymStack *S,char x)
{
	if(S->top==maxsize-1)
	    return 0;
	S->top = S->top + 1;
	S->data[S->top] = x;
	return 1; 
}
//出栈操作
int SymPop(SymStack *S,char &x)
{
	if(S->top==-1) 
	    return 0;
	x = S->data[S->top];
	S->top = S->top - 1;
	return 1;
}

//检查函数 
//输入字符合法性检查
int InputCheck(SeqList*L)
{
	for(int i=0;i<L->last;i++)
	{
		if((L->data[i]<'0'||L->data[i]>'9') && (L->data[i]!='+' && L->data[i]!='-' && L->data[i]!='*' && L->data[i]!='/' && L->data[i]!='(' && L->data[i]!=')'))
		    return 0;
	}
	return 1;
} 
//括号匹配检查
int BracketCheck(SeqList*L)
{
	SymStack*S;                //定义一个栈 
	InitSymStack(S);           //初始化栈 
	for(int i=0;i<L->last;i++)
	{
		if(L->data[i]=='(')    //搜索到左括号入栈 
		    SymPush(S,L->data[i]);
	    
	    else if(L->data[i]==')')  //搜索到右括号出栈
	    {
	    	char x;
	    	int y;
	    	y = SymPop(S,x);
	    	if(y==0)            //若栈空，则匹配失败 
	    	    return 0;
	    }
	}
	//扫描完整个顺序表 
	if(S->top == -1)         //若此时栈空，则匹配成功 
	    return 1;
	else                     //若还有剩余的括号，则匹配失败 
	    return 0;
}
//运算符合法性检查
int SymbolCheck(SeqList*L)
{
	//表达式开头不得是运算符+-*/ 
	if(L->data[0]=='+' || L->data[0]=='-' || L->data[0]=='*' || L->data[0]=='/')
	    return 0;
	
	int i;
	for(i=1;i<L->last-1;i++)
	{
		//输入的操作数必须为个位数，连续输入两个数字即判定不合法 
		if(L->data[i]>='0'&&L->data[i]<='9')
		    if(L->data[i-1]>='0'&&L->data[i-1]<='9')
		        return 0;
		//连续输入两个运算符即判定为不合法 
		if(L->data[i]=='+'||L->data[i]=='-'||L->data[i]=='*'||L->data[i]=='/')
		    if(L->data[i-1]=='+'||L->data[i-1]=='-'||L->data[i-1]=='*'||L->data[i-1]=='/')
	            return 0;
	    //左括号前不得有数字，左括号后不得有运算符
		if(L->data[i]=='(')
		    if(L->data[i+1]=='+'||L->data[i+1]=='-'||L->data[i+1]=='*'||L->data[i+1]=='/' || (L->data[i-1]>='0'&&L->data[i-1]<='9'))
	            return 0;
	    //右括号前不得有运算符,右括号后不得有数字
		if(L->data[i]==')')
		    if(L->data[i-1]=='+'||L->data[i-1]=='-'||L->data[i-1]=='*'||L->data[i-1]=='/' || (L->data[i+1]>='0'&&L->data[i+1]<='9'))
	            return 0;
	}
	
	//表达式结尾不得是运算符+-*/ 
	if(L->data[i]=='+' || L->data[i]=='-' || L->data[i]=='*' || L->data[i]=='/')
	    return 0;
	
	return 1;           //经检查无错误 
} 

//工具函数 
//运算符优先级赋值 
int Priority(char op)
{
	if(op=='+' || op=='-')
	    return 1;
	if(op=='*' || op=='/')
	    return 2;
} 
//数字字符转单精度 
float CTF(char c)
{
	float num;
	num = c - '0';
	return num;
}
//输入单精度类型的左右操作数和字符类型的运算符，给出计算结果 
float Calculate(float left,float right,char sym)
{
	float ans;
	if(sym=='+')
	    ans = left + right;
	else if(sym=='-')
	    ans = left - right;
	else if(sym=='*')
	    ans = left * right;
	else if(sym=='/')
	    ans = left / right;
	return ans;
} 
//弹出两个操作数栈顶元素和一个运算符栈顶元素，执行相应运算，运算结果压回操作数栈顶 
void Pop_calculate_push(NumStack*S_num,SymStack*S_sym)
{
	float left,right,result;               //声明单精度类型的左右操作数变量和运算结果 
	char sym;                              //声明字符类型的运算符变量 
	int x,y,z;
	x = NumPop(S_num,right);               //先出操作数栈的为右操作数
	y = NumPop(S_num,left);                //后出操作数栈的为左操作数
	z = SymPop(S_sym,sym);                 //运算符栈的栈顶元素出栈
	result = Calculate(left,right,sym);    //执行运算程序
	NumPush(S_num,result);                 //运算结果入操作数栈 
} 

//核心函数 
//中缀表达式转换为后缀表达式并计算 
float Change_and_calculate(SeqList*L)
{
	NumStack *S_num;                       //定义存放操作数的操作数栈
	SymStack *S_sym;                       //定义存放运算符的运算符栈 
	InitNumStack(S_num);                   //操作数栈初始化 
	InitSymStack(S_sym);                   //运算符栈初始化
	
	for(int i=0;i<L->last;i++)
	{
		if(L->data[i]>='0' && L->data[i]<='9')//遇到操作数
		    NumPush(S_num,CTF(L->data[i]));   //将操作数转换成单精度类型后从压入操作数栈
		
		if(L->data[i]=='(')                   //遇到界限符：左括号
			SymPush(S_sym,L->data[i]);        //左括号直接入运算符栈

		if(L->data[i]==')')                   //遇到界限符：右括号
		{
			while(S_sym->data[S_sym->top] != '(')    //直到弹出左括号为止
			{
				Pop_calculate_push(S_num,S_sym);
				//弹出两个操作数栈顶元素和一个运算符栈顶元素，执行相应运算，运算结果压回操作数栈顶
			}
			int w;
			char bracket;
			w = SymPop(S_sym,bracket);    //弹出左括号，舍弃
		} 
		
		if(L->data[i]=='+'||L->data[i]=='-'||L->data[i]=='*'||L->data[i]=='/')//遇到运算符
		{
			while(Priority(S_sym->data[S_sym->top])>=Priority(L->data[i]) && S_sym->top!=-1 && S_sym->data[S_sym->top]!='(')
			{
			//当栈顶元素的优先级>=当前运算符的优先级 且 栈不为空 且 栈顶元素不为左括号时
			//依次弹出栈内运算符并进行计算 
				Pop_calculate_push(S_num,S_sym);
				//弹出两个操作数栈顶元素和一个运算符栈顶元素，执行相应运算，运算结果压回操作数栈顶 
			}
			//当栈顶元素的优先级<当前运算符的优先级 或 栈空 或 栈顶元素为左括号时
			SymPush(S_sym,L->data[i]);                                    //当前运算符入运算符栈 
		}
	}
	
	while(S_sym->top != -1)
		Pop_calculate_push(S_num,S_sym);
		//弹出两个操作数栈顶元素和一个运算符栈顶元素，执行相应运算，运算结果压回操作数栈顶 

	float ans;                      //声明单精度类型的计算结果变量
	ans = S_num->data[S_num->top];  //当前操作数栈的栈顶元素即为运算结果 
	return ans;                     //返回运算结果 
}

