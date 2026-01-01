#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

//used char for better filling on file
typedef struct {
char Rendtd[20], gastos[20],qntPedds[10], qntPddsCnc[10], qntContatos[10], data[20];

} Infor;
//struct used to convert char from original file to double and int correctly
typedef struct {
double Rendtd, gastos; int qntPedds, qntPddsCnc, qntContatos, data;

} Infor2;



typedef struct Node{
Infor f;
struct Node *next;

} Node;

int desvioPadrao(Node *head);


int balancoDiario(FILE *p, Node **head, Node **tail){
    int c;
    Infor f;
    int r = 0;
    time_t t= time(NULL);
    struct tm *dt = localtime(&t);
    strftime(f.data,sizeof(f.data), "%d/%m/%Y",dt);

    while((c=getchar()) != '\n' && c != EOF);
    printf("\n-- PLATAFORMA DE BALANCO DIARIO ---\n");
    printf("Qnt. pedidos: \n");
    if(fgets(f.qntPedds, sizeof(f.qntPedds), stdin) != NULL){
        f.qntPedds[strcspn(f.qntPedds, "\n")] = 0;
        //parametro for test if everything s going good
        r = 1;
    }
    printf("Qnt. pedidos concluidos: \n");
    if(fgets(f.qntPddsCnc, sizeof(f.qntPddsCnc), stdin) != NULL){
        f.qntPddsCnc[strcspn(f.qntPddsCnc, "\n")] = 0;  r = 1;
    }
    printf("Qnt. contatos: \n");
     if(fgets(f.qntContatos, sizeof(f.qntContatos), stdin) != NULL){
        f.qntContatos[strcspn(f.qntContatos, "\n")] = 0;   r = 1;
    }
    printf("Rendimento do dia: \n");
    if(fgets(f.Rendtd, sizeof(f.Rendtd), stdin) != NULL){
        f.Rendtd[strcspn(f.Rendtd, "\n")] = 0;
          r = 1;
    }
    printf("Despesa: \n");
    if(fgets(f.gastos, sizeof(f.gastos), stdin) != NULL){
        f.gastos[strcspn(f.gastos, "\n")] = 0;
        r = 1;
    }
    if(r==1){
        printf("\n--DADOS ARMAZENADOS COM SUCESSO--\n");
    }
    Node *novo = malloc(sizeof(Node));
    novo->f = f;
    novo->next =NULL;
    if(*head == NULL){
        *head = novo;
        *tail = novo;
    } else{
        (*tail)->next = novo;
        *tail = novo;
    }
    fseek(p, 0, SEEK_END);
    fwrite(&f, sizeof(f), 1, p);
    fflush(p);
return 1;



};

void verificarBalnco(FILE *p){
    fflush(p);
rewind(p);
Infor f;
char dt[20];
int c;
        while((c=getchar()) != '\n' && c!= EOF);
        printf("Qual data procurada?(dia/mes/ano)\n");
        if(fgets(dt, sizeof(dt),stdin) != NULL){
            dt[strcspn(dt, "\n")] = 0; }

rewind(p);

while(fread(&f, sizeof(Infor), 1, p) == 1){

        if(strcmp(dt,f.data) == 0){
        printf("\nQnt. pedidos: %s - Pedidos concluidos: %s - Pedidos cancelados: %s - Rendimento: %s - Despesa: %s - Data: %s\n", f.qntPedds, f.qntPddsCnc, f.qntContatos, f.Rendtd, f.gastos,f.data);
    }}
}

void historico(Node *head){
Node *atual = head;
while(atual != NULL){
        printf("\nQnt. pedidos: %s - Pedidos concluidos: %s - Pedidos cancelados: %s - Rendimento: %s - Despesa: %s - Data: %s\n",
               atual->f.qntPedds,
               atual->f.qntPddsCnc,
               atual->f.qntContatos,
               atual->f.Rendtd,
               atual->f.gastos,
               atual->f.data);

    atual = atual->next;
    }

}
void carregarLista(FILE *p, Node **head, Node **tail){
fflush(p);
rewind(p);
Infor f;
while(fread(&f,sizeof(Infor), 1, p) == 1){
    Node *novo = malloc(sizeof(Node));
    novo->f = f;
    novo->next = NULL;
    if(*head == NULL){
        *head=novo;
        *tail = novo;
    } else{
        (*tail)->next =novo;
        *tail = novo;
    }
}


}

int main(){
FILE *info;
int opt, qntTd;
Node *tail =NULL;
Node *head = NULL;


info = fopen("balanco_diario.bin", "ab+");
carregarLista(info,&head,&tail);
if(info==NULL){
    printf("ERRO NA CRIAÇÃO DO ARQUIVO");
}

printf("\n-- PLATAFORMA DE BALANCO DIARIO ---\n");
do{
    printf("\n-- SEGUE O MENU DE OPCOES ABAIXO --");
    printf("\n  OPCOES DISPONIVEIS: \n  1-CADASTRAR BALANCO DO DIA  \n  2-VERIFICAR BALANCO  \n  3-HISTORICO  \n  4-DESVIO PADRAO \n  5-SAIR\n  Escolha:");
    scanf("%d", &opt);
    switch(opt){
case 1:
        balancoDiario(info, &head, &tail);
    break;

case 2:
        verificarBalnco(info);
    break;
case 3:
        historico(head);
    break;
case 4:
    double f = desvioPadrao(head);
    printf("Variancia: %.2lf\n",f);
    if(f>1000){
        printf("-- ATENCAO --\n");
        printf("-- VALORES INSTAVEIS CARECEM DE PLANEJAMENTO --\n");
    }
    break;
case 5:

    break;
default:
    printf("Opcao invalida");
    break;


    }

} while(opt != 5);
fclose(info);
}

int desvioPadrao(Node *head){
if(head == NULL){
    printf("Lista vazia");
}

Node *atual = head;
int contador = 0;
while(atual!= NULL){
    contador++;
    atual = atual->next;
}

Infor2 *list_struct = (Infor2 *)malloc(contador*sizeof(Infor2));

atual = head;
for(int i=0; i<contador; i++){
    sscanf(atual->f.Rendtd, "%lf", &list_struct[i].Rendtd);
     sscanf(atual->f.gastos, "%lf", &list_struct[i].gastos);
      sscanf(atual->f.qntPedds, "%d", &list_struct[i].qntPedds);
       sscanf(atual->f.qntPddsCnc, "%d", &list_struct[i].qntPddsCnc);
        sscanf(atual->f.qntContatos, "%d", &list_struct[i].qntContatos);
         sscanf(atual->f.data, "%d", &list_struct[i].data);

         atual = atual->next;
}

double med = 0;
double f=0,g=0;
for(int i=0; i<contador;i++){
    f+=list_struct[i].Rendtd;
}
if(contador>0){
med = f/contador;
}
if(contador>1){
for(int i =0; i<contador; i++){
    g+= (list_struct[i].Rendtd - med)*(list_struct[i].Rendtd - med);
}
g=g/(contador-1);
}else{
g=0;
}
g = sqrt(g);
return g;

}


