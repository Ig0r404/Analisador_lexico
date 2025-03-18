//Analisador lexico de C em C

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

int analisa_delimitador(char caracter){

    return(caracter == ' ' || caracter == '+' || caracter == '-'
            || caracter == '*' || caracter == '/' || caracter == ','
            || caracter == ';' || caracter == '%' || caracter == '>'
            || caracter == '<' || caracter == '=' || caracter == '('
            || caracter == ')' || caracter == '[' || caracter == ']'
            || caracter == '{' || caracter == '}');
}

int eh_operador(char caracter){

    return (caracter == '+' || caracter == '-' || caracter == '*'
            || caracter == '/' || caracter == '>' || caracter == '<'
            || caracter == '=');

}

int identificador_valido(char * str){

    return(str[0] != '0' && str[0] != '1' && str[0] != '2'
            && str[0] != '3' && str[0] != '4'
            && str[0] != '5' && str[0] != '6'
            && str[0] != '7' && str[0] != '8'
            && str[0] != '9' && !analisa_delimitador(str[0]));

}

int eh_palavra_chave(char* string){

    const char* palavra_chave[]
        = { "auto",     "break",    "case",     "char",
            "const",    "continue", "default",  "do",
            "double",   "else",     "enum",     "extern",
            "float",    "for",      "goto",     "if",
            "int",      "long",     "register", "return",
            "short",    "signed",   "sizeof",   "static",
            "struct",   "switch",   "typedef",  "union",
            "unsigned", "void",     "volatile", "while" };

            const int QTDP = 32;

            for (int i = 0; i<QTDP; i++){

                if(strcmp(string, palavra_chave[i]) == 0){

                    return 1;

                }

            }

    return 0;
}

int eh_inteiro (char* string){

    if (string == NULL || *string == '\0'){

        return 0;

    }

    int i = 0;

    while(isdigit(string[i])){

        i++;

    }

    return string[i] == '\0';

}

/* Retorna o token o qual é auma substring do nosso input */

char* pega_substring(char* string, int ini, int fim){

    int sub_tam = fim - ini + 1;

    char* substring = (char*)malloc((sub_tam + 1)* sizeof(char));

    strncpy(substring, string + ini, sub_tam);

    substring[sub_tam] = '\0';

    return substring;

}

/* Analisa Lexicamente uma string dada.

    Para isso, utilizamos a técnica de Variable Sliding Window,
    por isso iniciamos dois ponteiros direita e a esquerda. */

int lexer(char* input){

    int esq = 0;
    int dir = 0;

    int tam_entrada = strlen(input);

    while(dir <= tam_entrada && esq <= dir){

        if(!analisa_delimitador(input[dir])){
            dir++;
        }

        if(analisa_delimitador(input[dir]) && esq == dir){

            if(eh_operador(input[dir])){

                printf("Token: Operador, valor: %c\n", input[dir]);

            }

            dir++;
            esq = dir;
        }
        else if (analisa_delimitador(input[dir]) && esq !=dir || (dir == tam_entrada && esq != dir)){

                char* sub_string = pega_substring(input, esq, dir - 1);

                if (eh_palavra_chave(sub_string)){

                    printf("Token: Palavra Chave, Valor: %s\n", sub_string);

                }else if(eh_inteiro(sub_string)){

                    printf("Token: Inteiro, valor: %s\n", sub_string);

                }else if(!identificador_valido(sub_string) && !analisa_delimitador(input[dir -1])){

                    printf("Token: Indefinido, valor: %s\n", sub_string);
                }


                //Evita memoryleak

                free(sub_string);
                esq = dir;

            }

        }

    return 0;

}

void interpreta_arquivo(FILE * arquivo_texto){

    char texto[100];

    while(fgets(texto, sizeof(texto), arquivo_texto) != NULL){

        lexer(texto);

    }

}

void ler_arquivo(char * arquivo_patch){

    FILE * arquivo_texto;

    arquivo_texto = fopen(arquivo_patch, "r");

    if(!arquivo_texto){

        printf("Nao foi possivel abrir o arquivo!");

    }else{

        interpreta_arquivo(arquivo_texto);

    }

    fclose(arquivo_texto);

}

int main (){

    char nome_do_arquivo[30];

    printf("Digite o nome do arquivo: ");

    scanf("%s", nome_do_arquivo);

    ler_arquivo(nome_do_arquivo);

return 0;
}
