//this program will verify if the program is valid or not

#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<assert.h>
#include<unistd.h>
typedef struct identifiers{
	int size;
	char **loi;
	int cap;
} i;

#define VARIABLE 67
#define USER_DEF 68

char reserved[30][20] = {"commbox","var","const","integer","boolean","address",":",";","=","begin","send"
	,"to","recv","from","timeout","activate","with",",",">>>","(",")","+","-","*","/","|","end","and","or","not"};

char separators[9]={':','=',',',';','+','-','*','/','>'};
enum tokens{COMMBOX,VAR,CONST,INTEGER,BOOLEAN,ADDRESS,COLON,SEMICOLON,EQUAL,BEGIN,SEND,TO,RECEIVE,
	FROM,TIMEOUT,ACTIVATE,WITH,COMA,GT3,OB,CB,PLUS,MINUS,MUL,DIV,PIPE,END,AND,OR,NOT};

/* LIST OF METHODS */
char *next(char buf[]);
int validate(char buf[],int buf_size);
int check(char buf[],int buf_size,i *s1);
int ret_index(char *x);
int s_cmp(char *x,char y[]);
int isValidVariable(char *x);
int isValidValue(char *x);
int isNUM(char *x);
int c_commbox(char buf[]);
int c_const(char buf[],i *s1);
int c_var(char buf[], i *s1);
int c_begin(char buf[], i *s1, int buf_size);
void grow(i *s1);
int search(char *x, char **y,int size);
int c_con(char buf[],i *s1);
int c_stmt(char buf[],i *s1);
int c_term1(char buf[],i *s1);
int isSeparator(char a);
/* --------------------- */

int next_ptr=0;
char *curr;

int isSeparator(char a){
	int i=0,f=0;
	while(i<9 && f==0)
	{
		if(separators[i]==a)
			f=1;
		i=i+1;	
	}
return f;
}

void grow(i *s1){
	if(s1->size==s1->cap){
		s1->cap=s1->cap * 2;
		s1->loi=(char **)realloc(s1->loi,sizeof(char *)*s1->cap);
	}
	return;
}

int search(char *x,char **y,int size){
	int i=0,flag=0;
	while(i<size && flag==0){
//	printf("comparing curr: %s with y[%d]: %s\n",x,i,y[i]);
		if(s_cmp(x,y[i])){
			flag=1;
		}
		i=i+1;
	}
//	printf("\n%d\n",flag);
	return flag;
}

char *next(char buf[]){
	char *word = (char *)malloc(sizeof(char));
	int j=0,flag=0;
	while (flag==0){
	if(buf[next_ptr]=='\0')
	{
		flag=1;
		word[j]='\0';
	}
	else if(buf[next_ptr]==' '||buf[next_ptr]=='\n'||buf[next_ptr]=='\t')
	{
		if(j!=0)
		{
			flag = 1;
			next_ptr=next_ptr+1;
			word[j]='\0';
		}
		else{
			next_ptr=next_ptr+1;
		}
	}
	else
	{
		int k=0,l=0;
		while(k<9 && l==0){
			if(separators[k]==buf[next_ptr])
				l=1;
			k=k+1;
		}
		if(l==1)
		{
			if(j==0){
				printf("hello 1\n");
				word[j]=buf[next_ptr];
				j=j+1;
				next_ptr=next_ptr+1;
				while(isSeparator(buf[next_ptr])){
					printf("hello\n");
					word[j]=buf[next_ptr];
					j=j+1;
					next_ptr=next_ptr+1;
				}
				word[j]='\0';
				flag=1;
			}
			else
			{
				word[j]='\0';
				flag=1;
			}
		}
		else{
			word[j]=buf[next_ptr];
			j=j+1;
			next_ptr = next_ptr + 1;
		}
	}

	}
printf("next %s\n",word);
return word;
}

int validate(char buf[],int buf_size){

	i *s1 = (i *)malloc(sizeof(i));
	s1->size=0;
	s1->cap=10;
	s1->loi =(char **)malloc(sizeof(char *)*s1->cap);
	
	return check(buf,buf_size,s1);
	
}
int check(char buf[],int buf_size,i *s1)
{
	int x=1,flag=0;
	while(flag==0)
	{
		curr = next(buf);
		if(next_ptr == buf_size)
			flag=1;
		else if(ret_index(curr)==COMMBOX)
		{
			x=c_commbox(buf);
			if(x==0)
				flag=1;
			else
			{
				curr=next(buf);
				while((ret_index(curr) != BEGIN) && next_ptr<buf_size && flag==0)
				{
					if(ret_index(curr)==CONST)
					{
						x=c_const(buf,s1);
						if(x==0)
							flag=1;
					}
					else if(ret_index(curr)==VAR)
					{

						x=c_var(buf,s1);
						if(x==0)
							flag=1;
					}
					else
					{
						x=0;
						flag=1;
					}
					curr=next(buf);
				}
				if(next_ptr>=buf_size || flag==1)
				{
					x=0;
					flag=1;
				}
				else{
					x=c_begin(buf,s1,buf_size);
					if(x==0)
						flag=1;
				}
			}		
		}
		else
		{
			x=0;
			flag=1;

		}
	}
return x;
}

int ret_index(char *x){
	int i=0,f=0;
	while(i<27 && f==0){
		if(s_cmp(x,reserved[i]))
		{
			f=1;

		}
		else
			i=i+1;
	}
	if(i==27){
		if(isValidVariable(x))
			i=VARIABLE;
		else if(isValidValue(x))
			i=USER_DEF;
		else
			i=-1;
	}
	return i;
}
int s_cmp(char *x,char y[]){
	int i=0,j=0,f=1;
	while(x[i]!='\0' && y[j]!='\0' && f==1){
		if(x[i]!=y[j])
			f=0;
		i=i+1;
		j=j+1;
	}
	if(x[i]!='\0' && y[j]!='\0')
	       f=0;
return f;	
}

int isValidVariable(char *x){
	int i=0,f=1;
	while(i<27 && f==1){
		if(s_cmp(x,reserved[i]))
		{	
			f=0;
		}
		i=i+1;
	}
	if(f!=0){
		if((x[0]>='a' && x[0]<='z') || (x[0]>='A' && x[0]<='Z') || x[0]=='_')
		{
			f=1;
		}
		else
			f=0;
	}
	return f;
}

int isValidValue(char *x){
	int i=0,f=1;
	while(i<27 && f==1){
		if(s_cmp(x,reserved[i]))
			f=0;
		i=i+1;
	}
	if(f!=0)
	{
		if(isNUM(x))
			f=1;
		else if(isValidVariable(x))
			f=1;
		else
			f=0;
	}
	return f;
}
int isNUM(char *x){
	int i=0,f=1;
	while(x[i]!='\0' && f==1){
		if(x[i]<'0' && x[i]>'9')
			f=0;
		i=i+1;
	}
return f;
}

int c_commbox(char buf[])
{	int x;
	curr=next(buf);
	if(ret_index(curr)==VARIABLE)
	{
		curr=next(buf);
		if(ret_index(curr)==SEMICOLON)
			x=1;
		else
			x=0;
	}
	else
		x=0;
return x;
}

int c_const(char buf[],i *s1){
	int x,rep;
	curr=next(buf);
	if(ret_index(curr)==VARIABLE)
	{
		//printf("index of %s is %d\n",curr,ret_index(curr));
		rep=search(curr,s1->loi,s1->size);
		//printf("REPITION %d\n",rep);
		if(rep==0){
			grow(s1);
			s1->loi[s1->size]=curr;
			s1->size = s1->size + 1;
			curr=next(buf);
			if(ret_index(curr)==COLON){
				curr=next(buf);
				if(ret_index(curr)==INTEGER || ret_index(curr)==BOOLEAN || ret_index(curr)==ADDRESS)
				{
					curr=next(buf);
					if(ret_index(curr)==EQUAL)
					{
						curr=next(buf);
						if(ret_index(curr)==USER_DEF){
							curr=next(buf);
							if(ret_index(curr)==COMA){

								x=c_const(buf,s1);
							}
							else if(ret_index(curr)==SEMICOLON)
							{	x=1;
							}
							else
							{	x=0;
							}
						}
						else
							x=0;
					}
					else
						x=0;
				}	
				else
					x=0;
			}
			else{
					x=0;
				}
			}
		else
			x=0;
		}
		else
			x=0;
//printf("x %d\n",x);

return x;
}

int c_var(char buf[],i *s1){
	int x,rep;
	curr=next(buf);
	if(ret_index(curr)==VARIABLE)
	{
	rep=search(curr,s1->loi,s1->size);
	if(rep==0){
		grow(s1);
		s1->loi[s1->size]=curr;
		s1->size = s1->size + 1;
		curr=next(buf);
		if(ret_index(curr)==COLON)
		{
			curr=next(buf);
			if(ret_index(curr)==INTEGER || ret_index(curr)==BOOLEAN)
			{
				curr=next(buf);
				if(ret_index(curr)==EQUAL)
				{
					curr=next(buf);
					if(ret_index(curr)==USER_DEF)
					{
						curr=next(buf);
						if(ret_index(curr)==COMA)
						{
							x=c_var(buf,s1);
						}
						else if(ret_index(curr)==SEMICOLON)
						{
							x=1;
						}
						else
						{
							x=0;
						}
					}
					else
						x=0;
				}
				else{
						if(ret_index(curr)==COMA)
						{
							x=c_var(buf,s1);
						}
						else if(ret_index(curr)==SEMICOLON)
						{
							x=1;
						}
						else
						{
							x=0;
						}

				}
			}
			else
				x=0;
		}
		else
			x=0;
	}
	else
		x=0;
	}
	else
		x=0;
return x;
}

int c_begin(char buf[],i *s1,int buf_size)
{	
	int x=1;
	curr=next(buf);
//	while (ret_index(curr)!=END && x==1){
		if(ret_index(curr)==PIPE){
		while(ret_index(curr)!=GT3 && next_ptr<buf_size)
		{
			curr=next(buf);
			x=c_con(buf,s1);
			printf("CONDITION FUNCTION %d\n",x);
		}
			if(x!=0)
			{
			//	curr=next(buf);
				if(ret_index(curr)==GT3)
				{
					printf("\nGT3\n");
					x=c_stmt(buf,s1);
					printf("STATEMENT FUNCTION WORKED CORRECTLY %d\n",x);
					if(x!=0)
					{
					//	curr=next(buf);
						if(ret_index(curr)==END)
							{
								x=1;
							}
						//printf("current %s\n",curr);
					}
				}
				else
					x=0;
			}	
	}
	else
		x=0;
	//}
return x;
}

int c_stmt(char buf[],i *s1)
{
	int x=1;
	curr=next(buf);
	if(ret_index(curr)==SEND)
	{
		curr=next(buf);
		if(ret_index(curr)==VARIABLE)
		{
			curr=next(buf);
			if(ret_index(curr)==TO)
			{
				curr=next(buf);
				if(ret_index(curr)==VARIABLE)
				{
				//printf("search %d\n",search(curr,s1->loi,s1->size));
					if(search(curr,s1->loi,s1->size))
					{
						curr=next(buf);
						//printf("CURRENT %s\n",curr);
						if(ret_index(curr)==SEMICOLON)
						{	
							x=1;
						}
					}
					else
						x=0;
				}
				else
					x=0;
			}
			else
				x=0;
		}
		else
			x=0;
	}
	else if(ret_index(curr)==ACTIVATE){
		curr=next(buf);
		if(ret_index(curr)==VARIABLE){
			curr=next(buf);
			if(ret_index(curr)==WITH)
			{
				curr=next(buf);
				if(ret_index(curr)==VARIABLE)
				{
					x=search(curr,s1->loi,s1->size);
					if(x!=0)
					{
						curr=next(buf);
						if(ret_index(curr)==SEMICOLON)
						{x=1;
						}
						else
							x=0;
					}
				}
				else if(ret_index(curr)==USER_DEF)
				{	
					curr=next(buf);
					if(ret_index(curr)==SEMICOLON)
					{	x=1;
					}
					else
						x=0;
				}
				else
					x=0;
			}
			else
				x=0;
		}
		else
			x=0;
	}
	else
		x=0;

return x;
}

int c_con(char buf[],i *s1){
	int x=1;
	//curr=next(buf);
	if(c_term1(buf,s1))
		{
			curr=next(buf);
			if(ret_index(curr)==AND)
				{
					curr=next(buf);
					printf("AND NEXT %s\n",curr);
					x=c_con(buf,s1);
				}
			else if(ret_index(curr)==OR)
				{
					curr=next(buf);
					x=c_con(buf,s1);
				}
			else
				{
					x=1;
				}
		}
	else if (ret_index(curr)==NOT)
	{
		curr=next(buf);
		x=c_con(buf,s1);
	}
	else{
		x=0;
	}
return x;
}
int c_term1(char buf[],i *s1){
	int x;
	printf("TERMINAL %d  %d\n",ret_index(curr),RECEIVE);
	if (ret_index(curr)==TIMEOUT)
	{
		curr=next(buf);
		if(ret_index(curr)==VARIABLE)
		{
			/*if(search(curr,s1->loi,s1->size)==0)
			{
				x=0;
			}
			else
				x=1;*/
		}
		else
			x=0;
	}
	else if(ret_index(curr)==RECEIVE)
	{
		curr=next(buf);
		if(ret_index(curr)==VARIABLE)
		{
			curr=next(buf);
			if(ret_index(curr)==FROM)
			{
				curr=next(buf);
				if(ret_index(curr)==VARIABLE)
				{
					printf("SEARCH %d\n",search(curr,s1->loi,s1->size));
					if(search(curr,s1->loi,s1->size)==0)
					{	
						x=0;
					}
					else
						x=1;
				}
				else
					x=0;
			}
			else
				x=0;
		}
		else
			x=0;
	}
	else if(ret_index(curr)==VARIABLE)
	{
		if(search(curr,s1->loi,s1->size)==0)
			x=0;
		else
		{
			x=1;
		}
	}
/*	else if(expression(buf,s1))
	{
		x=1;
	}*/
	else
		x=0;
return x;
}

int expression(char buf[], i *s1){
	int x;
	if(ret_index(curr)==OB)
	{	
		curr=next(buf);
		x=expression(buf,s1);
		if(x==1)
		{
			if(ret_index(curr)==CB)
			{
				x=1;
			}
		}
	}
return x;
}
int main(){
	char buf[1024];
	int fd=open("commbox",O_RDONLY);
	int nb=read(fd,buf,1024);
	close(fd);
	int x=validate(buf,nb);
	if(x==0)
		printf("\tINVALID SYNTAX\n");
	else
		printf("\tVALID SYNTAX\n");
}
