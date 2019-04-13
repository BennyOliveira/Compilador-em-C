#include<stdio.h>
#include<conio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<locale.h>


//validar todos caracteres especiais e condições de parada em cada substring
bool Parar(char ch) 
{ 
    if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '/*' || ch == '*/' || ch == '//' || ch == ':=' || ch == '<>'
		|| ch == '<=' || ch == '>=' || ch == '=' || ch == '->' || ch == ',' || ch == ';' || ch == ':' || ch == '(' || ch == ')' || ch == '[' || ch == ']' 
		|| ch == '{' || ch == '}' || ch == '..' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '<' || ch == '>') 
        return (true); 
    return (false); 
}

//validar vazio, tab e nova linha
bool Vazio(char ch){
	if(ch == ' ' || ch == '\t' || ch == '\n')
		return (true);
	return (false);
} 

//validar simblos especiais
bool SimbolosEspeciais(char ch) 
{ 
    if (ch == ',' || ch == ';' || ch == ':' || ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == '..' 
		|| ch == '/*' || ch == '*/' || ch == '//' || ch == '.') 
        return (true); 
    return (false); 
}

//validar operadores
bool Operador(char ch) 
{ 
    if (ch == ':' || ch == '<>' || ch == '<=' || ch == '>=' || ch == '->' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '>' || ch == '<' || ch == '=' ) 
        return (true); 
    return (false); 
} 

//validar ou não identificadores
bool Identificador(char* str) 
{ 
    if (str[0] == '0' || str[0] == '1' || str[0] == '2' || str[0] == '3' || str[0] == '4' || str[0] == '5' || str[0] == '6' || str[0] == '7' || str[0] == '8' || str[0] == '9' || Parar(str[0]) == true) 
        return (false); 
    return (true); 
} 

//validar palavras reservadas
bool PalavrasReservadas(char* str) 
{ 
    if (!strcmp(str, "and") || !strcmp(str, "array") || !strcmp(str, "begin") || !strcmp(str, "case") || !strcmp(str, "char") || !strcmp(str, "const") || !strcmp(str, "div")
		|| !strcmp(str, "do") || !strcmp(str, "downto") || !strcmp(str, "else") || !strcmp(str, "end") || !strcmp(str, "file") || !strcmp(str, "for") || !strcmp(str, "function")
		|| !strcmp(str, "goto") || !strcmp(str, "if") || !strcmp(str, "in") || !strcmp(str, "integer") || !strcmp(str, "label") || !strcmp(str, "mod") || !strcmp(str, "nil")
		|| !strcmp(str, "not") || !strcmp(str, "of") || !strcmp(str, "or") || !strcmp(str,"packed") || !strcmp(str,"procedure") || !strcmp(str,"program") || !strcmp(str,"real")
		|| !strcmp(str,"record") || !strcmp(str,"repeat") || !strcmp(str,"set") || !strcmp(str,"string") || !strcmp(str,"then") || !strcmp(str,"to") || !strcmp(str,"type")
		|| !strcmp(str,"until") || !strcmp(str,"var") || !strcmp(str,"while") || !strcmp(str,"with")) 
        return (true); 
    return (false); 
} 

//validar numeros inteiros
bool Inteiro(char* str) 
{ 
    int i, len = strlen(str); 
  
    if (len == 0) 
        return (false); 
    for (i = 0; i < len; i++) { 
        if (str[i] != '0' && str[i] != '1' && str[i] != '2' && str[i] != '3' && str[i] != '4' && str[i] != '5' && str[i] != '6' && str[i] != '7' && str[i] != '8' && str[i] != '9' || (str[i] == '-' && i > 0)) 
            return (false); 
    } 
    return (true); 
} 

//validar números reais
bool NumeroReal(char* str) 
{ 
    int i, len = strlen(str); 
    bool Decimal = false; 
  
    if (len == 0) 
        return (false); 
    for (i = 0; i < len; i++) { 
        if (str[i] != '0' && str[i] != '1' && str[i] != '2' && str[i] != '3' && str[i] != '4' && str[i] != '5' && str[i] != '6' && str[i] != '7' && str[i] != '8' && str[i] != '9' && str[i] != '.' || (str[i] == '-' && i > 0)) 
            return (false); 
        if (str[i] == '.') 
            Decimal = true; 
    } 
    return (Decimal); 
} 

//coletando partes da string para fazer a verificação
char* subString(char* str, int left, int right) 
{ 
    int i; 
    char* subStr = (char*)
	malloc(sizeof(char) * (right - left + 2)); 
  
    for (i = left; i <= right; i++) 
        subStr[i - left] = str[i]; 
    	subStr[right - left + 1] = '\0'; 
    return (subStr); 
} 

//Printando cada validação
void verificar(char* str) 
{ 
    int left = 0, right = 0; 
    int len = strlen(str); 
  
    while (right <= len && left <= right) { 
        if (Parar(str[right]) == false) 
            right++; 
			 
        if (Parar(str[right]) == true && left == right) { 
            if ((Operador(str[right]) == true) && (Operador(str[right +1]) == true)){
            	printf("Operador: \t\t%c%c\n", str[right], str[right +1]);	
            	right++;
            	left = right;
			}else if (Operador(str[right]) == true){
            	printf("Operador: \t\t%c\n", str[right]);            	
			}else if(SimbolosEspeciais(str[right]) == true){
				printf("Símbolo especial: \t%c\n", str[right]);			
			}
  			right++; 
            left = right;
					
        } else if (Parar(str[right]) == true && left != right 
                   || (right == len && left != right)) { 
            char* subStr = subString(str, left, right - 1); 
  
            if (PalavrasReservadas(subStr) == true) 
                printf("Palavra reservada: \t%s\n", subStr); 
  
            else if (Inteiro(subStr) == true) 
                printf("Inteiro: \t\t%s\n", subStr); 
  
            else if (NumeroReal(subStr) == true) 
                printf("Número Real: \t\t%s\n", subStr); 
  
            else if (Identificador(subStr) == true
            	&& Parar(str[right - 1]) == false)
                printf("Identificador: \t\t%s\n", subStr); 
	
            else if (Identificador(subStr) == false
                     && SimbolosEspeciais(str[right - 1]) == false) 
                printf("Identificador inválido: %s\n", subStr); 
            left = right; 
        } 
    } 
    return; 
}

int main() 
{    
 setlocale(LC_ALL, "");
 
  //Pegando o arquivo para realizar a leitura léxica
 FILE *arquivo;
 char str[10000];
 
 arquivo = fopen("C:\\Users\\caminho\\.txt","r");
 
 if(arquivo == NULL)
     printf("Erro, não foi possível abrir o arquivo\n");
 	else
     	while((fgets(str, sizeof(str), arquivo)) != NULL )
     	   verificar(str);

 fclose(arquivo);   
 
 return 0;  
    
} 
