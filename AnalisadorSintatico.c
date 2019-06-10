#include <stdio.h>
#include <string.h>
#include <ctype.h>


//-------------------------TOKENS RESERVADOS-------------------------

#define ID 0x800
#define CT 0x801

#define OF 0x805
#define VAR 0x841
#define PROCEDURE 0x806
#define FUNCTION 0x807
#define BEGIN 0x808
#define END 0x809
#define IF 0x811
#define THEN 0x812
#define ELSE 0x813
#define WHILE 0x814
#define DO 0x815
#define FOR 0x816
#define TO 0x817
#define REPEAT 0x819
#define UNTIL 0x820
#define OR 0x821
#define AND 0x822
#define NOT 0x823
#define DIV 0x824
#define MOD 0x825
#define PROGRAM 0x826
#define INTEGER 0x827
#define BOOLEAN 0x829
#define ATRIB 0x830
#define MENORIGUAL 0x831
#define MAIORIGUAL 0x832
#define DIFERENTE 0x833
#define DOTDOT 0x834
#define READ 0x835
#define WRITE 0x836
#define CASE 0x837
#define ARRAY 0x838
#define REAL 0X839
#define CHAR 0x840
#define CONST 0x842
#define DOWNTO 0x843
#define GOTO 0x844
#define IN 0x845
#define LABEL 0x846
#define PACKED 0x847
#define NIL 0x848
#define RECORD 0x849
#define SET 0x850
#define TYPE 0x851
#define WITH 0x852


#define MAXENTRIES 500


/*---------------------- VARIAVEIS GLOBAIS-----------------------*/

// LOOKAHEAD - Onde armazena o token do ultimo lexema.
// LEXEMA    - Onde armazena o lexema.  

FILE *arq_entrada, *arq_objeto;

char lexema[33];

typedef long int TOKENTYPE;
TOKENTYPE lookahead;

TOKENTYPE match(TOKENTYPE token);
TOKENTYPE gettoken(void);

void programa (void);
void declara_variavel (void);
void subprogramas (void);
void bloco (void);
void comando (void);
void ifthen (void);
void thenelse (void);
void whiledo (void);
void repeatuntil (void);
void declara_for (void);
void declara_case(void);
void declara_read (void);
void declara_write (void);
void lista_expressao (void);
void expressao (void);
void expressao_simples (void);
void termo (void);
void fator (void);

int lookup (const char *symbol);
int insert (const char *symbol, int token);
void declara (void);
void verifica(const char *symbol);


//--------------------------------ANALISADOR LEXICO----------------------------------

// Verificando se é caracter especial

TOKENTYPE gettoken()
{
  while((*lexema=getc(arq_entrada)),isspace(*lexema));
  
  if (isalpha(*lexema))
	 {
		ungetc(*lexema,arq_entrada);
		fscanf(arq_entrada, "%[A-Za-z0-9_]",lexema);
      
	return (insert (lexema, ID));
	 } else

	 if (isdigit(*lexema))
	 {
		ungetc(*lexema,arq_entrada);
		fscanf(arq_entrada, "%[0-9]",lexema);
		return(CT);
	 } else
	 switch(*lexema)
	 {
		case ';':
		case ',':
		case '=':
		case '+':
		case '-':
		case '*':
		case '/':
		case '"':
		case '[':
		case ']':
		case '{':
		case '}':	
		case '(':
		case ')': return *lexema;
		case '.':
				  {
					 *lexema= fgetc(arq_entrada);
					 if (*lexema == '.')
						 return DOTDOT;
					 else
					 {
						ungetc(*lexema,arq_entrada);
						return '.';
					 }
				  }
		case ':':
				  {
					 *lexema= fgetc(arq_entrada);
					 if (*lexema == '=')
						 return ATRIB;
					 else
					 {
						ungetc(*lexema,arq_entrada);
						return ':';
					 }
				  }
		case '<':
				  {
					 *lexema= fgetc(arq_entrada);
					 if (*lexema == '=')
						return MENORIGUAL;
					 else if (*lexema == '>')
						return DIFERENTE;
					 else
					 {
						ungetc(*lexema,arq_entrada);
						return '<';
					 }
				  }
		case '>':
				  {
					 *lexema= fgetc(arq_entrada);
					 if (*lexema == '=')
						 return MAIORIGUAL;
					 else
					 {
						ungetc(*lexema,arq_entrada);
						return '>';
					 }
				  }
	 }

  return(0);
}

TOKENTYPE match(TOKENTYPE token)
{
  if (token == lookahead)
  {
	lookahead = gettoken();
	return lookahead;
  }
  else
  {
	printf("\n\t\tERRO!\n");
	return(0);

  }
}

//typedef char palavras[12];

char lexstream[10000];

//	LASTENTRY	- Posição da ultima entrada na tabela de simbolos
//	NEXTENTRY	- Posição da próxima entrada na tabela de simbolos
//	LEXENTRY	- Posição na fita onde os lexemas são armazenados


int nextentry, lexentry, lastentry;

//Estrutura da tabela de símbolos

typedef struct
{
  int lex;	   			// Ponteiro para vetor de lexemas
  TOKENTYPE tolken;		// Armazena o token
  int dec;				// Armazena 1 para variável declarada
} tipo_tab_simbolo;

tipo_tab_simbolo symboltable[MAXENTRIES];


//Insere simbolos na tabela

int insert (const char *symbol, int token){
  int pos;
  pos = lookup (symbol);
  if (pos == -1){
	symboltable[nextentry].lex = lexentry;
      symboltable[nextentry].tolken = token;
      strcpy (lexstream + lexentry, symbol);
      lexentry += strlen (symbol) + 1;
	lastentry = nextentry;
      nextentry++;
      return token;
  }
  return symboltable[pos].tolken;
} 

//Retorna a posição do simbolo

int lookup (const char *symbol){
  int i;
  for (i = 0; i < nextentry; i++){
      if (strcmp (symbol, lexstream + symboltable[i].lex) == 0)
      return (i);
  }
  return (-1);
};

void declara ( ){
	
	symboltable[lastentry].dec = 1;

}

//Verifica se o ID foi declarado

void verifica (const char *symbol){
	int pos;
	pos=lookup(symbol);
	
	if (symboltable[pos].dec==1)
	{
	}else
	{ 
		printf ("A VARIAVEL: %s, NAO FOI DECLARADA!\n", symbol);
	}
}


//--------------------------------ANALISADOR SINTATICO--------------------

// Variável que faz o controle dos rótulos

int ROTULO= 0;

void programa (void){
	match (PROGRAM);
  	match (ID);
  	match (';');
  	declara_variavel();
  	subprogramas ();
  	fprintf(arq_objeto,"\n.section .text \n\n");
  	fprintf(arq_objeto,"\t .global _start \n");
  	fprintf(arq_objeto, "_start: \n");
  	bloco ();
  	match ('.');

	fprintf(arq_objeto, "_end: \n");
  	fprintf(arq_objeto, "\t movl $1, %eax\n");  
  	fprintf(arq_objeto, "\t xorl %ebx, %ebx \n");
  	fprintf(arq_objeto, "\t int 0x80\n");

}

// Declaração de variáveis globais, integer e boolean
 
void declara_variavel (void){
	if (lookahead == VAR)
	{

      	fprintf(arq_objeto,".section .bss \n\n");
		match (VAR);
		do
   		{
		
			fprintf(arq_objeto, ".common %s, 4\n", lexema);

			declara();
			match(ID);
			do
			{
	   			if (lookahead == ',')
	   			{
					match(',');
				
					fprintf(arq_objeto, ".common %s, 4\n", lexema);
					declara();
					match(ID);
	   			}
			}while (lookahead == ',');
			match(':');
			if (lookahead == INTEGER)
	      	match(INTEGER);
			else
	      		if (lookahead == BOOLEAN)
		 			match(BOOLEAN);
					match(';');
   		}while(lookahead == ID);
	}
}

void subprogramas (){
  	while ((lookahead == PROCEDURE) || (lookahead == FUNCTION))
	{   
    	fprintf(arq_objeto,"\nTesteCompleto.text \n\n");
    	if (lookahead == PROCEDURE)
	{
		match (PROCEDURE);
		fprintf (arq_objeto, "%s:\n", lexema);
		fprintf (arq_objeto, "\t pushl %ebp \n");
		fprintf (arq_objeto, "\t movl %esp, %ebp \n");
		declara();
		match (ID);
		if (lookahead == '(')
		{
      		match ('(');
      		match (')');
		}
	match (';');
	declara_variavel ();
	fprintf(arq_objeto, "\t addl $0, %esp \n");
	bloco ();
	fprintf(arq_objeto, "\t movl %ebp, %esp\n");
	fprintf(arq_objeto, "\t popl %ebp \n");
	fprintf(arq_objeto, "\t ret \n");
	match (';');
    	}
    	else{
		match (FUNCTION);
		fprintf (arq_objeto, "%s:\n", lexema);
		declara();
		match (ID);
		if (lookahead == '(')
		{
      		match ('(');
     	 		match (')');
		}
		match (':');
		if (lookahead == INTEGER)
		match(INTEGER);
		else
			if (lookahead == BOOLEAN)
			match(BOOLEAN);
		match (';');
		bloco ();
		match (';');
    		}
  	}
}

void bloco (void){
	
	match (BEGIN);
  	while (lookahead != END)
	{
      comando ();
      if(lookahead==';') 
	match (';');
      else break;
    	} 
	match (END);

}

void comando (void){
  char auxiliar[33];	
  	if (lookahead == BEGIN)
    	bloco ();

 	 else if (lookahead == ID)
		{     

      	strcpy (auxiliar, lexema);

		verifica (lexema);
      	match (ID);
		if (lookahead != ATRIB)
		fprintf (arq_objeto, "\t call %s\n", auxiliar);
    		if (lookahead=='(')
		{
			match('(');
			match(')');
    		}else
    			{
      		match (ATRIB);
      		expressao_simples ();
			fprintf (arq_objeto, "\t movl %eax, %s\n", auxiliar);
    			}
    		}

  	ifthen ();
  	whiledo ();
  	repeatuntil ();
	declara_for();
  	declara_read();
 	declara_write();
	declara_case();

}

void ifthen (void){
  	int L, L1;

  	if (lookahead == IF)
	{
      match (IF);
      expressao ();
      match (THEN);
      L1 = L = ROTULO++;
      fprintf (arq_objeto, "\t and %eax,%eax\n\t jz .L%d\n", L);
      comando ();
      if (lookahead == ELSE)
	{
		match (ELSE);
	  	L1 = ROTULO++;
	  	fprintf (arq_objeto, "\t jmp .L%d\n.L%d:\n", L1, L);
	  	comando ();
      }	
      fprintf (arq_objeto, ".L%d:\n", L1);
  	}
}

void whiledo (void){
  	int L, L1;

  	if (lookahead == WHILE)
	{
      	match (WHILE);
      	L = ROTULO++;
      	fprintf (arq_objeto, ".L%d:\n", L);
      	expressao ();
     	 	L1 = ROTULO++;
      	fprintf (arq_objeto, "\t jz .L%d\n", L1);
      	match (DO);
      	comando ();
      	fprintf (arq_objeto, "\t jmp .L%d\n.L%d:\n", L, L1);
   	}
}

void repeatuntil (void){
  	int L;

  	if (lookahead == REPEAT)
	{
      	match (REPEAT);
      	L = ROTULO++;
      	fprintf (arq_objeto, ".L%d:\n", L);
		do
		{
      		comando ();
      		if(lookahead==';') 
			match (';');
		}while(lookahead==ID);
      	
		match (UNTIL);
      	expressao ();
      	fprintf (arq_objeto, "\t jz .L%d\n", L);
  	}
}

void declara_for (void){
	int L,L1;
	char auxiliar[33];
	if (lookahead == FOR)
	{
		match(FOR);
	
  		if (lookahead == ID)
		{	
      		strcpy (auxiliar, lexema);
			verifica(lexema);
      		match (ID);
			match (ATRIB);
      		fprintf (arq_objeto, "\t movl %s, %eax\n", lexema);
		      fprintf (arq_objeto, "\t movl %eax, %s\n", auxiliar);
			match(CT);
			match(TO);
      		fprintf (arq_objeto, "\t movl %s, %edx\n", lexema);
			match(CT);
     	 		L= ROTULO++;
	      	fprintf (arq_objeto, ".L%d:\n", L);
      		fprintf (arq_objeto, "\t movl %s, %eax\n", auxiliar);
			fprintf (arq_objeto, "\t cmpl %edx, %eax \n");
			fprintf (arq_objeto, "\t setgeb %al \n");
			L1 = ROTULO++;      		
			fprintf (arq_objeto, "\t jz .L%d\n", L1);
			fprintf (arq_objeto, "\t inc %eax\n");
		      fprintf (arq_objeto, "\t movl %eax, %s\n", auxiliar);
      		match (DO);
      		comando ();
      		fprintf (arq_objeto, "\t jmp .L%d\n.L%d:\n", L, L1);
		}
	}
}

void declara_read()
{	
	if (lookahead == READ)
    	{		
    	match(READ);
    	match('(');

    	fprintf(arq_objeto, "\n\t movl $3, %eax\n");
    	fprintf(arq_objeto, "\t movl $0, %ebx\n");
    	fprintf(arq_objeto, "\t movl %s,%ecx\n",lexema);
    	fprintf(arq_objeto, "\t movl 4, %edx\n");
    	fprintf(arq_objeto, "\t int 0x80\n\n");
    	match(ID);
    	match(')');
    	}	
}

void declara_write()
{     
	char constante[60] = "";
	if(lookahead == WRITE)
    	{
    	match(WRITE);
    	match('(');
	match('"');
	while (lookahead != '"'){
	strcat(constante, lexema);
	strcat(constante," ");	
	lookahead = gettoken();
	}	
    	fprintf(arq_objeto, "\n\t movl $4, %eax\n");
    	fprintf(arq_objeto, "\t movl $1, %ebx\n");
    	fprintf(arq_objeto, "\t movl %s, %ecx\n",constante);

	fprintf(arq_objeto, "\t movl %i, %edx\n",strlen(constante));
    	fprintf(arq_objeto, "\t int 0x80\n\n");
    	
	match ('"');	
    	match(')');
	}
}
 
void declara_case (void){
int L, L1;

	if (lookahead == CASE )
	{
	match(CASE);
	fprintf (arq_objeto, "\t movl %s, %eax\n", lexema);
      fprintf (arq_objeto, "\t pushl %eax\n");	
	match(ID);
	match(OF);
	do
	{	
    	 	if (lookahead == ';')
		{
		match(';');
		}
			L= ROTULO++;
	      	fprintf (arq_objeto, ".L%d:\n", L);
      		fprintf (arq_objeto, "\t movl %s, %edx\n",lexema);
			fprintf (arq_objeto, "\t popl %ebx\n");	
			fprintf (arq_objeto, "\t cmpl %edx, %ebx \n");
			fprintf (arq_objeto, "\t seteb %al \n");
      		fprintf (arq_objeto, "\t pushl %ebx\n");
			L1 = ROTULO++;      		
			fprintf (arq_objeto, "\t jnz .L%d\n", L1);
	  		fprintf (arq_objeto, "\t jmp .L%d\n", L1+1);
			match (CT);
			match (':');    	
			fprintf (arq_objeto, ".L%d:\n", L1);
			comando();
			fprintf (arq_objeto, "\t jmp .L%d\n", L1+1);
	}while (lookahead ==';');
	fprintf (arq_objeto, ".L%i:\n", ROTULO);
	match(END);
	}
}	

void lista_expressao (void){
  	expressao ();
  	while (lookahead == ',')
	{
      match (',');
      expressao ();
  	}
}

void expressao (void){
  	expressao_simples ();
  	switch (lookahead)
	{
    	case '=':
      match ('=');
      expressao_simples ();
	fprintf(arq_objeto, "\t movl %eax, %ecx \n");
	fprintf(arq_objeto, "\t popl %ebx \n");
	fprintf(arq_objeto, "\t xorl %eax \n");
	fprintf(arq_objeto, "\t cmpl %ecx, %ebx \n");
	fprintf(arq_objeto, "\t seteb %al \n");
      break;
    	
	case '>':
      match ('>');
      expressao_simples ();
	fprintf(arq_objeto, "\t movl %eax, %ecx \n");
	fprintf(arq_objeto, "\t popl %ebx \n");
	fprintf(arq_objeto, "\t xorl %eax \n");
	fprintf(arq_objeto, "\t cmpl %ecx, %ebx \n");
	fprintf(arq_objeto, "\t setgb %al \n");
      break;
    
	case '<':
      match ('<');
      expressao_simples ();
	fprintf(arq_objeto, "\t movl %eax, %ecx \n");
	fprintf(arq_objeto, "\t popl %ebx \n");
	fprintf(arq_objeto, "\t xorl %eax \n");
	fprintf(arq_objeto, "\t cmpl %ecx, %ebx \n");
	fprintf(arq_objeto, "\t setlb %al \n");
      break;

	case MENORIGUAL:
      match (MENORIGUAL);
      expressao_simples ();
	fprintf(arq_objeto, "\t movl %eax, %ecx \n");
	fprintf(arq_objeto, "\t popl %ebx \n");
	fprintf(arq_objeto, "\t xorl %eax \n");
	fprintf(arq_objeto, "\t cmpl %ecx, %ebx \n");
	fprintf(arq_objeto, "\t setleb %al \n");
      break;

    	case MAIORIGUAL:
      match (MAIORIGUAL);
      expressao_simples ();
	fprintf(arq_objeto, "\t movl %eax, %ecx \n");
	fprintf(arq_objeto, "\t popl %ebx \n");
	fprintf(arq_objeto, "\t xorl %eax \n");
	fprintf(arq_objeto, "\t cmpl %ecx, %ebx \n");
	fprintf(arq_objeto, "\t setgeb %al \n");
      break;

    	case DIFERENTE:
      match (DIFERENTE);
      expressao_simples ();
	fprintf(arq_objeto, "\t movl %eax, %ecx \n");
	fprintf(arq_objeto, "\t popl %ebx \n");
	fprintf(arq_objeto, "\t xorl %eax \n");
	fprintf(arq_objeto, "\t cmpl %ecx, %ebx \n");
	fprintf(arq_objeto, "\t setneb %al \n");
      break;

    }
}

void expressao_simples (void){
  	if (lookahead == '+')
	{
      match ('+');
      termo ();
  	}else if (lookahead == '-')
		{
      	match ('-');
      	termo ();
      	fprintf (arq_objeto, "\t negl %eax\n");
  		}else termo ();
  			while ((lookahead == '+') || (lookahead == '-') || (lookahead == OR) 
					|| (lookahead == AND) )
    			switch (lookahead)
			{
      		case '+':
			match ('+');
			termo ();
			fprintf (arq_objeto, "\t popl %ebx\n");
			fprintf (arq_objeto, "\t addl %ebx, \%eax\n");
			break;
      
			case '-':
			match ('-');
			termo ();
			fprintf (arq_objeto, "\t popl %ebx\n");
			fprintf (arq_objeto, "\t subl %ebx, %eax\n");
			break;
      
			case OR:
			match (OR);
			termo ();
	 		fprintf(arq_objeto, "\t popl %ebx \n");
	 		fprintf(arq_objeto, "\t orl %ebx, %eax \n");
			break;
			
			case AND:
			match (AND);
			termo ();
	 		fprintf(arq_objeto, "\t popl %ebx \n");
	 		fprintf(arq_objeto, "\t andl %ebx, %eax \n");
			break;

      		}
}

void termo (void){
	char auxiliar[33];
	fator ();
	while ((lookahead == '*') || (lookahead == DIV)|| (lookahead== '/'))
	switch (lookahead)
	{
      case '*': match ('*');
	if (lookahead == ID)
	{	
		fprintf(arq_objeto, "\t imul %s, %eax\n", lexema);
	
		verifica(lexema);
		match(ID);
	}
	if (lookahead == CT)
	{
		fprintf(arq_objeto, "\t mull $%s, %eax\n", lexema);
	
		match(CT);
	}
	      lista_expressao();
	      break;

	case '/': match ('/');
	if (lookahead == ID)
	{
		  fprintf(arq_objeto, "\t divl %s, %eax\n", lexema);
		
		  verifica(lexema);	
		  match(ID);
	}else {
	if (lookahead == CT)
	{
		fprintf(arq_objeto, "\t divl $%s, %eax\n", lexema);
	
		match(CT);
	}}
	lista_expressao();
	break;

    	case DIV: match (DIV);
	if (lookahead == ID)
	{
		  fprintf(arq_objeto, "\t divl %s, %eax\n", lexema);

		  verifica(lexema);	
		  match(ID);
	}else {
	if (lookahead == CT)
	{
		fprintf(arq_objeto, "\t divl $%s, %eax\n", lexema);

		match(CT);
	}}
	lista_expressao();
	break;

    	default:  break;
   	}

}

void fator (void){
  	char auxiliar[33];
  	if (lookahead == ID)
	{
      strcpy (auxiliar, lexema);
	verifica(lexema);
      match (ID);
      if (lookahead == '(')
	{
	match ('(');
	lista_expressao ();
	match (')');
      }else if (lookahead == '[')
		{
	  	match ('[');
	  	lista_expressao ();
	 	match (']');
      	}
      fprintf (arq_objeto, "\t pushl %eax\n");
      fprintf (arq_objeto, "\t movl %s, %eax\n", auxiliar);
    	}
  
	if (lookahead == CT)
	{
	fprintf (arq_objeto, "\t pushl %eax \n ");
	fprintf (arq_objeto, "\t movl %s , %eax \n ", lexema);
      match (CT);
  	}
  
	if (lookahead == '(')
	{
      match ('(');
      expressao ();
      match (')');
  	}
  	
  		if (lookahead == '{')
	{
      match ('{');
      expressao ();
      match ('}');
  	}
  
  
}


//---------------------------------MAIN-----------------------------------------------

main(void){

// Inicialização da tabela de símbolos

  insert ("of", OF);
  insert ("var", VAR);
  insert ("procedure", PROCEDURE);
  insert ("function", FUNCTION);
  insert ("begin", BEGIN);
  insert ("end", END);
  insert ("if", IF);
  insert ("then", THEN);
  insert ("else", ELSE);
  insert ("while", WHILE);
  insert ("do", DO);
  insert ("for", FOR);
  insert ("to", TO);
  insert ("repeat", REPEAT);
  insert ("until", UNTIL);
  insert ("or", OR);
  insert ("and", AND);
  insert ("not", NOT);
  insert ("div", DIV);
  insert ("mod", MOD);
  insert ("program", PROGRAM);
  insert ("integer", INTEGER);
  insert ("real", REAL);
  insert ("boolean", BOOLEAN);
  insert ("read", READ);
  insert ("write", WRITE);
  insert ("case", CASE);
  insert ("with", WITH);
  insert ("type", TYPE);
  insert ("packed", PACKED);
  insert ("nil", NIL);
  insert ("label", LABEL);
  insert ("goto", GOTO);
  insert ("downto", DOWNTO);
  insert ("array", ARRAY);
  insert ("char", CHAR);	

arq_entrada = fopen("TesteCompleto.txt","r");

	lookahead = gettoken();
	programa ();

	printf("\n\n \tARQUIVO COMPILADO COM SUCESSO!!!\n");

	getch();

};

