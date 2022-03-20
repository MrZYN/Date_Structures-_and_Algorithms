#include<stdio.h>
#include<malloc.h>
#define maxsize 1024 

//����˳��� 
typedef struct{
	char data[maxsize];
	int last;
}SeqList;
//����������ջ
typedef struct{
	float data[maxsize];
	int top;
}NumStack;
//���������ջ
typedef struct{
	char data[maxsize];
	int top;
}SymStack;


//˳���������ջ�������ջ 
void CreateList(SeqList*&L);          //����˳���
void InitNumStack(NumStack*&S);       //��ʼ��������ջ 
int NumEmpty(NumStack *S);            //�жϲ�����ջ�� 
int NumPush(NumStack *S,float x);     //�������ջ 
int NumPop(NumStack *S,float &x);     //��������ջ 
void InitSymStack(SymStack*&S);       //��ʼ�������ջ 
int SymEmpty(SymStack *S);            //�ж������ջ�� 
int SymPush(SymStack *S,char x);      //�������ջ 
int SymPop(SymStack *S,char &x);      //�������ջ

//��麯��
int InputCheck(SeqList*L);            //�����ַ��Ϸ��Լ��
int BracketCheck(SeqList*L);          //����ƥ����
int SymbolCheck(SeqList*L);           //������Ϸ��Լ��

//���ߺ��� 
int Priority(char op);                //��������ȼ���ֵ
float CTF(char c);                    //�����ַ�ת������ 
float Calculate(float left,float right,char sym);
    //���뵥�������͵����Ҳ��������ַ����͵������������������  
void Pop_calculate_push(NumStack *S_num,SymStack *S_sym);
    //��������������ջ��Ԫ�غ�һ�������ջ��Ԫ�أ�ִ����Ӧ���㣬������ѹ�ز�����ջ��

//���ĺ��� 
float Change_and_calculate(SeqList*L);//��׺���ʽת��Ϊ��׺���ʽ������ 

int main()
{
	printf("***�Ӽ��˳�������***\n");
	printf("�������������ʽ����׺������ENTER������\n");
	printf("ע�⣺�����������λ���������������ӦΪӢ�ĸ�ʽ\n");
	
	SeqList *L;
	while(1)
	{
		CreateList(L);                                        //����˳���
		if(InputCheck(L) && BracketCheck(L) && SymbolCheck(L))//���кϷ��Լ�� 
		    break;                                            //���ɹ�����ѭ��
		else                                                  //������������ 
        	printf("���������������������!\n");
	} 
	
	float ans;
	ans = Change_and_calculate(L); 
	printf("��������\n%.3f\n",ans);	
	
	return 0;
} 

//����˳���
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
//��������ջ 
//��ʼ��ջ 
void InitNumStack(NumStack*&S)
{
	S = (NumStack*)malloc(sizeof(NumStack));
	S->top = -1;//��ʼ��ջ��ָ�� 
}
//�ж�ջ��
int NumEmpty(NumStack *S)
{
	if(S->top==-1)//ջ��
	    return 1;
	else//���� 
	    return 0; 
}
//��Ԫ����ջ
int NumPush(NumStack *S,float x)
{
	if(S->top==maxsize-1)//ջ�������� 
	    return 0;
	S->top = S->top + 1;//ָ���ȼ�1 
	S->data[S->top] = x;//��Ԫ����ջ
	return 1; 
}
//��ջ����
int NumPop(NumStack *S,float &x)
{
	if(S->top==-1)//ջ�գ����� 
	    return 0;
	x = S->data[S->top];//ջ��Ԫ���ȳ�ջ 
	S->top = S->top - 1;//ָ���ټ�1 
	return 1;
}
//�������ջ 
//��ʼ��ջ 
void InitSymStack(SymStack*&S)
{
	S = (SymStack*)malloc(sizeof(SymStack));
	S->top = -1;
}
//�ж�ջ��
int SymEmpty(SymStack *S)
{
	if(S->top==-1)
	    return 1;
	else
	    return 0; 
} 
//��Ԫ����ջ
int SymPush(SymStack *S,char x)
{
	if(S->top==maxsize-1)
	    return 0;
	S->top = S->top + 1;
	S->data[S->top] = x;
	return 1; 
}
//��ջ����
int SymPop(SymStack *S,char &x)
{
	if(S->top==-1) 
	    return 0;
	x = S->data[S->top];
	S->top = S->top - 1;
	return 1;
}

//��麯�� 
//�����ַ��Ϸ��Լ��
int InputCheck(SeqList*L)
{
	for(int i=0;i<L->last;i++)
	{
		if((L->data[i]<'0'||L->data[i]>'9') && (L->data[i]!='+' && L->data[i]!='-' && L->data[i]!='*' && L->data[i]!='/' && L->data[i]!='(' && L->data[i]!=')'))
		    return 0;
	}
	return 1;
} 
//����ƥ����
int BracketCheck(SeqList*L)
{
	SymStack*S;                //����һ��ջ 
	InitSymStack(S);           //��ʼ��ջ 
	for(int i=0;i<L->last;i++)
	{
		if(L->data[i]=='(')    //��������������ջ 
		    SymPush(S,L->data[i]);
	    
	    else if(L->data[i]==')')  //�����������ų�ջ
	    {
	    	char x;
	    	int y;
	    	y = SymPop(S,x);
	    	if(y==0)            //��ջ�գ���ƥ��ʧ�� 
	    	    return 0;
	    }
	}
	//ɨ��������˳��� 
	if(S->top == -1)         //����ʱջ�գ���ƥ��ɹ� 
	    return 1;
	else                     //������ʣ������ţ���ƥ��ʧ�� 
	    return 0;
}
//������Ϸ��Լ��
int SymbolCheck(SeqList*L)
{
	//���ʽ��ͷ�����������+-*/ 
	if(L->data[0]=='+' || L->data[0]=='-' || L->data[0]=='*' || L->data[0]=='/')
	    return 0;
	
	int i;
	for(i=1;i<L->last-1;i++)
	{
		//����Ĳ���������Ϊ��λ�������������������ּ��ж����Ϸ� 
		if(L->data[i]>='0'&&L->data[i]<='9')
		    if(L->data[i-1]>='0'&&L->data[i-1]<='9')
		        return 0;
		//��������������������ж�Ϊ���Ϸ� 
		if(L->data[i]=='+'||L->data[i]=='-'||L->data[i]=='*'||L->data[i]=='/')
		    if(L->data[i-1]=='+'||L->data[i-1]=='-'||L->data[i-1]=='*'||L->data[i-1]=='/')
	            return 0;
	    //������ǰ���������֣������ź󲻵��������
		if(L->data[i]=='(')
		    if(L->data[i+1]=='+'||L->data[i+1]=='-'||L->data[i+1]=='*'||L->data[i+1]=='/' || (L->data[i-1]>='0'&&L->data[i-1]<='9'))
	            return 0;
	    //������ǰ�����������,�����ź󲻵�������
		if(L->data[i]==')')
		    if(L->data[i-1]=='+'||L->data[i-1]=='-'||L->data[i-1]=='*'||L->data[i-1]=='/' || (L->data[i+1]>='0'&&L->data[i+1]<='9'))
	            return 0;
	}
	
	//���ʽ��β�����������+-*/ 
	if(L->data[i]=='+' || L->data[i]=='-' || L->data[i]=='*' || L->data[i]=='/')
	    return 0;
	
	return 1;           //������޴��� 
} 

//���ߺ��� 
//��������ȼ���ֵ 
int Priority(char op)
{
	if(op=='+' || op=='-')
	    return 1;
	if(op=='*' || op=='/')
	    return 2;
} 
//�����ַ�ת������ 
float CTF(char c)
{
	float num;
	num = c - '0';
	return num;
}
//���뵥�������͵����Ҳ��������ַ����͵������������������ 
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
//��������������ջ��Ԫ�غ�һ�������ջ��Ԫ�أ�ִ����Ӧ���㣬������ѹ�ز�����ջ�� 
void Pop_calculate_push(NumStack*S_num,SymStack*S_sym)
{
	float left,right,result;               //�������������͵����Ҳ����������������� 
	char sym;                              //�����ַ����͵���������� 
	int x,y,z;
	x = NumPop(S_num,right);               //�ȳ�������ջ��Ϊ�Ҳ�����
	y = NumPop(S_num,left);                //���������ջ��Ϊ�������
	z = SymPop(S_sym,sym);                 //�����ջ��ջ��Ԫ�س�ջ
	result = Calculate(left,right,sym);    //ִ���������
	NumPush(S_num,result);                 //�������������ջ 
} 

//���ĺ��� 
//��׺���ʽת��Ϊ��׺���ʽ������ 
float Change_and_calculate(SeqList*L)
{
	NumStack *S_num;                       //�����Ų������Ĳ�����ջ
	SymStack *S_sym;                       //������������������ջ 
	InitNumStack(S_num);                   //������ջ��ʼ�� 
	InitSymStack(S_sym);                   //�����ջ��ʼ��
	
	for(int i=0;i<L->last;i++)
	{
		if(L->data[i]>='0' && L->data[i]<='9')//����������
		    NumPush(S_num,CTF(L->data[i]));   //��������ת���ɵ��������ͺ��ѹ�������ջ
		
		if(L->data[i]=='(')                   //�������޷���������
			SymPush(S_sym,L->data[i]);        //������ֱ���������ջ

		if(L->data[i]==')')                   //�������޷���������
		{
			while(S_sym->data[S_sym->top] != '(')    //ֱ������������Ϊֹ
			{
				Pop_calculate_push(S_num,S_sym);
				//��������������ջ��Ԫ�غ�һ�������ջ��Ԫ�أ�ִ����Ӧ���㣬������ѹ�ز�����ջ��
			}
			int w;
			char bracket;
			w = SymPop(S_sym,bracket);    //���������ţ�����
		} 
		
		if(L->data[i]=='+'||L->data[i]=='-'||L->data[i]=='*'||L->data[i]=='/')//���������
		{
			while(Priority(S_sym->data[S_sym->top])>=Priority(L->data[i]) && S_sym->top!=-1 && S_sym->data[S_sym->top]!='(')
			{
			//��ջ��Ԫ�ص����ȼ�>=��ǰ����������ȼ� �� ջ��Ϊ�� �� ջ��Ԫ�ز�Ϊ������ʱ
			//���ε���ջ������������м��� 
				Pop_calculate_push(S_num,S_sym);
				//��������������ջ��Ԫ�غ�һ�������ջ��Ԫ�أ�ִ����Ӧ���㣬������ѹ�ز�����ջ�� 
			}
			//��ջ��Ԫ�ص����ȼ�<��ǰ����������ȼ� �� ջ�� �� ջ��Ԫ��Ϊ������ʱ
			SymPush(S_sym,L->data[i]);                                    //��ǰ������������ջ 
		}
	}
	
	while(S_sym->top != -1)
		Pop_calculate_push(S_num,S_sym);
		//��������������ջ��Ԫ�غ�һ�������ջ��Ԫ�أ�ִ����Ӧ���㣬������ѹ�ز�����ջ�� 

	float ans;                      //�������������͵ļ���������
	ans = S_num->data[S_num->top];  //��ǰ������ջ��ջ��Ԫ�ؼ�Ϊ������ 
	return ans;                     //���������� 
}

