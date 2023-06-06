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

char separators[12]={':','=',',',';','+','-','*','/','>','|','(',')'};
enum tokens{COMMBOX,VAR,CONST,INTEGER,BOOLEAN,ADDRESS,COLON,SEMICOLON,EQUAL,BEGIN,SEND,TO,RECEIVE,
	FROM,TIMEOUT,ACTIVATE,WITH,COMA,GT3,OB,CB,PLUS,MINUS,MUL,DIV,PIPE,END,AND,OR,NOT};

/* LIST OF METHODS */
char *next();
int validate();
int check(i *s1);
int ret_index(char *x);
int s_cmp(char *x,char y[]);
int isValidVariable(char *x);
int isValidValue(char *x);
int isNUM(char *x);
int c_commbox();
int c_const(i *s1);
int c_var(i *s1);
int c_begin(i *s1);
void grow(i *s1);
int search(char *x, char **y,int size);
int c_con(i *s1);
int c_stmt(i *s1);
int c_term1(i *s1);
int isSeparator(char a);
int expression(i *s1);
/* --------------------- */

//int next_ptr=0;
char *curr;
FILE *buf;

int isSeparator(char a){
	int i=0,f=0;
	while(i<12 && f==0)
	{
		if(separators[i]==a)
			f=1;
		i=i+1;	
	}
return f;
}

char *next(){
        char c;
        char *word = (char *)malloc(sizeof(char));
        int flag=0,j=0;
        while((c=getc(buf))!=EOF && flag==0)
        {
                if(c==' '||c=='\n'||c=='\t'){
                        if(j!=0)
                                {
                                        flag=1;
                                        c=ungetc(c,buf);
                                }
                }
                else if(isSeparator(c))
                {
                        if(j==0)
                        {
                        //      printf("HELLO\n");
                                while(isSeparator(c) && c!=EOF)
                                {
                                        word[j]=c;
                                        j=j+1;
                                        c=getc(buf);
                                }
                                flag=1;
                                if(c!=EOF)
                                        c=ungetc(c,buf);
                                        c=ungetc(c,buf);
                                //printf("%c ",c);
                        }
                        else
                        {
                                flag=1;
                                c=ungetc(c,buf);
                                c=ungetc(c,buf);
                                //printf(" %c ",c);
                        }
                }
                else
                {
                        word[j]=c;
                        j=j+1;
                }
        }
        if(c==EOF && j==0)
                word=NULL;
return word;
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

int validate(){

	i *s1 = (i *)malloc(sizeof(i));
	s1->size=0;
	s1->cap=10;
	s1->loi =(char **)malloc(sizeof(char *)*s1->cap);
	
	return check(s1);
	
}
int check(i *s1)
{
	int x=1,flag=0;
	while(flag==0)
	{
		curr = next();
		if(curr==NULL)
			flag=1;
		else if(ret_index(curr)==COMMBOX)
		{
			x=c_commbox();
			if(x==0)
				flag=1;
			else
			{
				curr=next();
				while((ret_index(curr) != BEGIN) && curr!=NULL && flag==0)
				{
					if(ret_index(curr)==CONST)
					{
						x=c_const(s1);
						printf("%d\n",x);
						if(x==0)
							flag=1;
					}
					else if(ret_index(curr)==VAR)
					{

						x=c_var(s1);
						if(x==0)
							flag=1;
					}
					else
					{
						x=0;
						flag=1;
					}
					curr=next();
				}
				if(curr == NULL || flag==1)
				{
					x=0;
					flag=1;
				}
				else{
				printf("begin");
					x=c_begin(s1);
					printf("after begin %d\n",x);
				//	if(x==0)
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
printf("Final x : %d\n",x);
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

int c_commbox()
{	int x;
	curr=next();
	if(ret_index(curr)==VARIABLE)
	{
		curr=next();
		if(ret_index(curr)==SEMICOLON)
			x=1;
		else
			x=0;
	}
	else
		x=0;
return x;
}

int c_const(i *s1){
	int x,rep;
	curr=next();
	if(ret_index(curr)==VARIABLE)
	{
		//printf("index of %s is %d\n",curr,ret_index(curr));
		rep=search(curr,s1->loi,s1->size);
	//	printf("REPITION %d\n",rep);
		if(rep==0){
			grow(s1);
			s1->loi[s1->size]=curr;
			s1->size = s1->size + 1;
			curr=next();
			if(ret_index(curr)==COLON){
				curr=next();
				if(ret_index(curr)==INTEGER || ret_index(curr)==BOOLEAN || ret_index(curr)==ADDRESS)
				{
					curr=next();
					if(ret_index(curr)==EQUAL)
					{
						curr=next();
						printf("%s\n",curr);
						if(ret_index(curr)==USER_DEF){
							curr=next();
							printf("%s\n",curr);
							if(ret_index(curr)==COMA){

								x=c_const(s1);
							}
							else if(ret_index(curr)==SEMICOLON)
							{	
								printf("semicolon");
								x=1;
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

int c_var(i *s1){
	int x,rep;
	curr=next();
	if(ret_index(curr)==VARIABLE)
	{
	rep=search(curr,s1->loi,s1->size);
	if(rep==0){
		grow(s1);
		s1->loi[s1->size]=curr;
		s1->size = s1->size + 1;
		curr=next();
		if(ret_index(curr)==COLON)
		{
			curr=next();
			if(ret_index(curr)==INTEGER || ret_index(curr)==BOOLEAN)
			{
				curr=next();
				if(ret_index(curr)==EQUAL)
				{
					curr=next();
					if(ret_index(curr)==USER_DEF)
					{
						curr=next();
						if(ret_index(curr)==COMA)
						{
							x=c_var(s1);
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
							x=c_var(s1);
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

int c_begin(i *s1)
{	
	int x=1;
	curr=next();
	while (ret_index(curr)!=END && x==1){
		if(ret_index(curr)==PIPE){
		while(ret_index(curr)!=GT3 && curr!=NULL)
		{
			curr=next();
			x=c_con(s1);
			printf("CONDITION FUNCTION %d\n",x);
		}
			if(x!=0)
			{
				if(ret_index(curr)==GT3)
				{
					printf("\nGT3\n");
					curr=next();
					printf("before loop: %s\n",curr);
					while((ret_index(curr)!=PIPE && ret_index(curr)!=END) && curr!=NULL && x==1)
					{
						x=c_stmt(s1);
						printf("STATEMENT FUNCTION WORKED CORRECTLY %d %s\n",x,curr);
					}
					
						printf("very important good morning message \n");
				}
				else
					x=0;
			}	
	}
	else
		x=0;
	}
	printf("curr %s\n",curr);
return x;
}

int c_stmt(i *s1)
{
	int x=1;
	//curr=next();
	if(expression(s1)){
			if(ret_index(curr)==SEMICOLON)
			{	
				x=1;
				curr=next();
			}
			else
				x=0;
	}
	else if(ret_index(curr)==SEND)
	{
		curr=next();
		if(ret_index(curr)==VARIABLE)
		{
			curr=next();
			if(ret_index(curr)==TO)
			{
				curr=next();
				if(ret_index(curr)==VARIABLE)
				{
				//printf("search %d\n",search(curr,s1->loi,s1->size));
					if(search(curr,s1->loi,s1->size))
					{
						curr=next();
						//printf("CURRENT %s\n",curr);
						if(ret_index(curr)==SEMICOLON)
						{
							printf("Semicolon in stmt\n");
							x=1;
							curr=next();
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
	}
	else if(ret_index(curr)==ACTIVATE){
		curr=next();
		if(ret_index(curr)==VARIABLE){
			curr=next();
			if(ret_index(curr)==WITH)
			{
				curr=next();
				if(ret_index(curr)==VARIABLE)
				{
					x=search(curr,s1->loi,s1->size);
					if(x!=0)
					{
						curr=next();
						if(ret_index(curr)==SEMICOLON)
						{x=1;
						}
						else
							x=0;
					}
				}
				else if(ret_index(curr)==USER_DEF)
				{	
					curr=next();
					if(ret_index(curr)==SEMICOLON)
					{	x=1;
						curr=next();
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

int c_con(i *s1){
	int x=1;
	//curr=next(buf);
	if(c_term1(s1))
		{
			curr=next();
			if(ret_index(curr)==AND)
				{
					curr=next();
					printf("AND NEXT %s\n",curr);
					x=c_con(s1);
				}
			else if(ret_index(curr)==OR)
				{
					curr=next();
					x=c_con(s1);
				}
			else
				{
					x=1;
				}
		}
	else if (ret_index(curr)==NOT)
	{
		curr=next();
		x=c_con(s1);
	}
	else{
		x=0;
	}
return x;
}
int c_term1(i *s1){
	int x;
	printf("TERMINAL %d  %d\n",ret_index(curr),RECEIVE);
	if (ret_index(curr)==TIMEOUT)
	{
		curr=next();
		if(ret_index(curr)==VARIABLE)
		{
			x=1;
		}
		else
			x=0;
	}
	else if(ret_index(curr)==RECEIVE)
	{
		curr=next();
		if(ret_index(curr)==VARIABLE)
		{
			curr=next();
			if(ret_index(curr)==FROM)
			{
				curr=next();
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
	else
		x=0;
return x;
}

int expression(i *s1){
	int x;
	if(ret_index(curr)==OB)
	{	
		curr=next();
		x=expression(s1);
		if(x==1)
		{
			if(ret_index(curr)==CB)
			{
				x=1;
				curr=next();
			}
			else
				x=0;
		}
	}
	else if(ret_index(curr)==VARIABLE)
	{
			if(search(curr,s1->loi,s1->size))
			{
				curr=next();
				if(ret_index(curr)==AND || ret_index(curr) == OR)
				{
					curr=next();
					if(ret_index(curr)==VARIABLE)
					{
						curr=next();
						x=1;
					}
					else
						x=0;
						//check type of variable boolean
					}
				else if(ret_index(curr)==PLUS || ret_index(curr)==MINUS || ret_index(curr)==MUL || ret_index(curr)==DIV)
					{
						curr=next();
						if(ret_index(curr)==VARIABLE)
						{
							curr=next();
							x=1;
						}
						else
							x=0;
				//check type of variable integer
					}
				else if(ret_index(curr)==EQUAL)
				{
						curr=next();
						if(ret_index(curr)==EQUAL)
						{	
							curr=next();
							x=expression(s1);
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

int main(){
FILE *ptr;
ptr=fopen("test","r");
if(ptr==NULL)
        printf("ERROR IN FILE OPENING\n");
else{
        buf=ptr;
	int x = validate();
	if(x==0)
		printf("\tINVALID SYNTAX\t\n");
	else
		printf("\tVALID SYNTAX\t\n");
        fclose(ptr);
}
}


