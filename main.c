#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

typedef struct
{
	char category[30], nm[20], price[12], quant[20], data[20];

} Product;
typedef struct {
	float price; int quant, data;

} ProductBoolean ;

typedef struct Node{
	Product product;
	struct  Node *next;
	

}Node;

void carregarLista(FILE* f, Node** head, Node** tail) {
	Product temp;
	rewind(f);
	while (fread(&temp, sizeof(Product), 1, f) == 1) {
		Node* novo = (Node*)malloc(sizeof(Node));
		if (novo == NULL) return;

		novo->product = temp;
		novo->next = NULL;

		// Adicionar na lista encadeada
		if (*head == NULL) {
			*head = novo;
			*tail = novo;
		}
		else {
			(*tail)->next = novo;
			*tail = novo;
		}
	}
}

void buildCsv(Node* head, FILE* p) {
	 
	//if (p == NULL) return;

	Node* atual = head;
	fprintf(p, "\xEF\xBB\xBF");
	fprintf(p, "Categoria;Nome;Valor;Quantidade;Data\n");
	while (atual != NULL) {
		char precoParaCsv[12];
		strcpy(precoParaCsv, atual->product.price);
		for (int i = 0; precoParaCsv[i]; i++) {
			if(precoParaCsv[i] == '.') precoParaCsv[i] = ',';
		}
		fprintf(p, "%s;%s;%s;%s;%s\n", atual->product.category, atual->product.nm,
			precoParaCsv, atual->product.quant, atual->product.data);
		atual = atual->next;
	}
	fflush(p);
	fclose(p);
	printf("relatorio criado com sucesso!!\n");


	/**/

}

void visualizeData(FILE *p) {
	fflush(p);
	rewind(p);
	Product bole;
	char catg[30];
	int c;

	while ((c = getchar()) != '\n' && c != EOF);
	printf("\nDigite a categoria: ");
	if (fgets(catg, sizeof(catg), stdin) != NULL) {
		catg[strcspn(catg, "\n")] = '\0';
		int t = strlen(catg);
		for (int i = 0; i < t; i++) {
			catg[i] = tolower((unsigned char)catg[i]);
		}
		//catg[strcspn(catg, "\n")] = 0;
	}
	rewind(p);
	while (fread(&bole, sizeof(Product), 1, p) == 1) {
		if (strcmp(catg, bole.category) == 0) {
			printf("\n Categoria: %s \n nome: %s \n valor: %s \n quantidade: %s \n data: %s \n \n", bole.category, bole.nm, bole.price, bole.quant, bole.data);
		}
	}


};

void armazenarData(FILE* p, Node** head, Node** tail) {
	int c;
	Product prd;
	char categoryPreviel[30];
	rewind(p);
	int vrf = 0;
	time_t t = time(NULL);
	struct tm* dt = localtime(&t);
	strftime(prd.data, sizeof(prd.data), "%d/%m/%Y", dt);

	while ((c = getchar()) != '\n' && c != EOF);

	printf("\nDigite a categoria do item: ");
	if (fgets(categoryPreviel, sizeof(categoryPreviel), stdin) != NULL) {
		categoryPreviel[strcspn(categoryPreviel, "\n")] = '\0';
		int len = strlen(categoryPreviel);
		for (int i = 0; i < len; i++) {
			categoryPreviel[i] = tolower((unsigned char)categoryPreviel[i]);
			prd.category[i] = categoryPreviel[i];
		}
		prd.category[len] = '\0';
		vrf = 1;
	}
	printf("\nDigite o nome do item: ");
	if (fgets(prd.nm, sizeof(prd.nm), stdin) != NULL) {
		prd.nm[strcspn(prd.nm, "\n")] = 0;
		vrf += 1;
	}
	printf("\nDigite o valor do item: ");
	if (fgets(prd.price, sizeof(prd.price), stdin) != NULL) {
		prd.price[strcspn(prd.price, "\n")] = 0;
		vrf += 1;
	}
	printf("\nDigite a quantidade do item: ");
	if (fgets(prd.quant, sizeof(prd.quant), stdin) != NULL) {
		prd.quant[strcspn(prd.quant, "\n")] = 0;
		vrf += 1;
	}

	if (vrf == 4) {
		printf("\nAdicao bem sucessida!!\n");
	}
	Node* novo = malloc(sizeof(Node));
	novo->product= prd;
	novo->next = NULL;
	if (*head == NULL) {
		*head = novo;
		*tail = novo;
	}
	else {
		(*tail)->next = novo;
		*tail = novo;
	}
	fseek(p, 0, SEEK_END);
	fwrite(&prd, sizeof(prd), 1, p);
	fflush(p);
}


void alterarData(FILE* p,FILE* g ,char new[], char old[]) {
	rewind(p);
	rewind(g);
	Product b;
	long posicao;

	fprintf(g, "\xEF\xBB\xBF");
	fprintf(g, "Categoria;Nome;Valor;Quantidade;Data\n");

	while (fread(&b, sizeof(Product), 1, p) == 1) {
		
		if (strcmp(old, b.category) == 0) {
			posicao = ftell(p);
			strcpy(b.category, new);
			fseek(p, -(long)sizeof(Product), SEEK_CUR);
			fwrite(&b, sizeof(Product), 1,p);

			fseek(p, posicao, SEEK_SET);
		}
		if (strcmp(old, b.nm) == 0) {
			posicao = ftell(p);
			strcpy(b.nm, new);
			fseek(p, -(long)sizeof(Product), SEEK_CUR);
			fwrite(&b, sizeof(Product), 1, p);

			fseek(p, posicao, SEEK_SET);
		}
		if (strcmp(old, b.price) == 0) {
			posicao = ftell(p);
			strcpy(b.price, new);
			fseek(p, -(long)sizeof(Product), SEEK_CUR);
			fwrite(&b, sizeof(Product), 1, p);

			fseek(p, posicao, SEEK_SET);
		}
		


		char precoParaCsv[12];
		strcpy(precoParaCsv, b.price);
		for (int i = 0; precoParaCsv[i]; i++) {
			if (precoParaCsv[i] == '.') precoParaCsv[i] = ',';
		}

		fprintf(g, "%s;%s;%s;%s;%s\n", b.category, b.nm, precoParaCsv, b.quant, b.data);
	}
	clearerr(p);
	fflush(p); fflush(g);
	

}

void atualizeData(FILE* p, Node** head, Node** tail, FILE* g) {
	rewind(p);
	int c, d, w, opp;
	char categ[30];
	Product new;

	Node* atual = *head;

	while ((c = getchar()) != '\n' && c != EOF);
	printf("\n O que vai atualizar: \n 1- categ. \n 2- nome \nOption: ");
	scanf("%d", &d);
	
	switch (d) {
	case 1:
		
		while ((w = getchar()) != '\n' && w != EOF);
		printf("\nQual a categoria? ");
		if (fgets(categ, sizeof(categ), stdin) != NULL) {
			categ[strcspn(categ, "\n")] = 0;
			int len = strlen(categ);
			for (int i = 0; i < len; i++) {
				categ[i] = tolower(categ[i]);
			}
			
		}
		
		while (atual != NULL && strcmp(atual->product.category, categ) != 0){
			atual = atual->next;
		}
		if (atual != NULL) {
			
			printf("\nDigite a nova categoria: ");
			char cat[30];
			if (fgets(cat, sizeof(cat), stdin) != NULL) {
				cat[strcspn(cat, "\n")] = 0;
				int len = strlen(cat);
				for (int i = 0; i < len; i++) {
					cat[i] = tolower(cat[i]);
				}
				alterarData(p, g, cat, categ);
				strcpy(atual->product.category, cat);
			}
			
		}
		
		break;

	case 2:
		do {


			while ((w = getchar()) != '\n' && w != EOF);
			printf("\nQual o nome? ");
			if (fgets(categ, sizeof(categ), stdin) != NULL) {
				categ[strcspn(categ, "\n")] = 0;
				int len = strlen(categ);
				for (int i = 0; i < len; i++) {
					categ[i] = tolower(categ[i]);
				}

			}

			while (atual != NULL && strcmp(atual->product.nm, categ) != 0) {
				atual = atual->next;
			}
			if (atual != NULL) {

				printf("\nDigite o novo nome: ");
				char cat[20];
				if (fgets(cat, sizeof(cat), stdin) != NULL) {
					cat[strcspn(cat, "\n")] = 0;
					int len = strlen(cat);
					for (int i = 0; i < len; i++) {
						cat[i] = tolower(cat[i]);
					}
					alterarData(p, g, cat, categ);
					strcpy(atual->product.nm, cat);
				}

				printf("\nDeseja alterar o valor? (1-S / 2-N)\nOption:");
				scanf("%d", &opp);

				if (opp == 1) {
					while ((w = getchar()) != '\n' && w != EOF);
					printf("\nDigite o novo valor para %s:", atual->product.nm);
					char newPrice[12];
					if (fgets(newPrice, sizeof(newPrice), stdin) != NULL) {
						newPrice[strcspn(newPrice, "\n")] = 0;
						alterarData(p, g, newPrice, atual->product.price);
						strcpy(atual->product.price, newPrice);
						
					}

				}
				

			}
			

			opp = 2;
		} while (opp != 2);

		break;


	default:
		printf("Opcao invalida");
		break;
	}


}

int main() {
	FILE* f;
	FILE* g;
	int opt = 0;
	Node* tail = NULL;
	Node* head = NULL;

	
	 g = fopen("meu_relatorio_produtos.csv", "w+");
	f = fopen("Arquivotest_product.bin", "rb+");
	if(f == NULL) {
		printf("Erro na criação");
		f = fopen("Arquivotest_product.bin", "wb+");

	}

	carregarLista(f, &head, &tail);

	printf("\n__PLATAFORMA DE CADASTRO DE ITENS__\n");
	do {
		printf("   Selecione uma opcao abaixo: \n 1- Adicionar item \n 2- Verificar itens \n 3- Atualizar item \n 4- Gerar csv \n 5- Exit\n Option: ");
		scanf("%d", &opt);
		switch (opt) {
		case 1:
			//printf("Adiconar");
			armazenarData(f, &head, &tail);
			break;
		case 2:
			//printf("Verificar");
			visualizeData(f);
			break;
		case 3:
			atualizeData(f, &head, &tail, g);
			//printf("Atualizar item");
			break;
		case 4:
			buildCsv(head, g);
			break;
		case 5:
			printf("Exiting...");
			free(f);
			free(g);
			free(head);
			free(tail);
			break;
		default:
			printf("algume erro");
			break;



		};




	} while (opt != 5);


	fclose(f); fclose(g);
	return 0;

}

