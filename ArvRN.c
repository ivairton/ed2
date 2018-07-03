/**
  * UFMT - CUA - ciência da Computação
  * Disciplina: ED2
  * Professor: Ivairton M. Santos
  * Autor: Ivairton
  *
  * Funções para inserção, remoção e manipulação da
  * estrutura de dados Árvore Rubro-Negra.
  *
  */

#include <stdio.h>
#include <stdlib.h>

/*** Declaração de tipos ***/
typedef enum{PRETO, VERMELHO} tipoCor;

struct estruturaRN {		//Estrutura do nó
	struct estruturaRN *esq;
	struct estruturaRN *dir;
	int valor;
	tipoCor cor ;
};
typedef struct estruturaRN tipoRN;

/*** Prototipação das funções ***/
	//Função princpal de inserção na árvore
	tipoRN *insereRN (int valor, tipoRN *arv);
	//Função de inserção na RN
	int insereEmRubNeg(int valor, tipoRN *no, tipoRN *pai, tipoRN *avo, tipoRN **raiz);
	//Função de balanceamento
	int balanceamento(tipoRN *no, tipoRN *pai, tipoRN *avo, tipoRN *bisavo, tipoRN **raiz);

	//Função principal de remoção na árvore
	tipoRN *removeRN (int valor, tipoRN *arv);
	//Função de remoção na RN
	int removeEmRubNeg(int valor, tipoRN *no, tipoRN *pai, tipoRN *avo, tipoRN **raiz);

/**
 * Função principal de inserção em árvore Rubro-Negra .
 * Esta função será chamada pela função principal.
 *
 * O parâmetro 'valor' corresponde ao valor a ser inserido da árvore
 * O parâmetro 'arv' corresponde ao ponteiro para a raiz da árvore
 */
tipoRN *insereRN (int valor, tipoRN *arv) {
	tipoRN *raiz;	//Este ponteiro será utilizado para apontar para a raiz da árvore
	raiz = arv;
	//A função 'insereEmRubNeg' é a função que será chamada recursivamente para inserção na árvore RN. Ela contém os parâmetros da chave (valor a ser inserido), a raíz da subárvore (no a ser verificado), o filho esq e direito ea raiz da árvore.
	insereEmRubNeg(valor, arv, NULL, NULL, &raiz);
	raiz->cor = PRETO;

	return raiz;
}


/**
 * Função de inserção em árvore Rubro-Negra, a ser chamada pela
 * função principal de inserção. Esta função será executada
 * recursivamente.
 *
 * O parâmetro 'valor' corresponde ao valor a ser inserido na árvore
 * O parâmetro 'no' corresponde ao nó corrente (sendo processado)
 * O parâmetro 'pai' ao pai do nó corrente
 * O parâmetro 'avo' ao avo do nó corrente
 * O parâmetro 'raiz' corresponde à raíz da árvore
 */
int insereEmRubNeg(int valor, tipoRN *no, tipoRN *pai, tipoRN *avo, tipoRN **raiz) {
	tipoRN **noADescer;
	int flagSucesso;

	//Verifica se alcancou a posição onde o novo valor deve ser armazenado (alocando um novo nó).
	if (no == NULL) {
		no = (tipoRN*) malloc (sizeof(tipoRN));
		no->valor = valor;
		no->esq = NULL;
		no->dir = NULL;
		no->cor = VERMELHO;

		//Verifica o caso especial de quando a árvore está vazia, nesse caso o novo nó é a raiz da árvore
		if (*raiz == NULL) {
			*raiz = no;
		} //Atualiza o ponteiro do nó pai do novo nó
		  else {
			if ( valor < pai->valor ) {
				pai->esq = no;
			} else {
				pai->dir = no;
			}
		}
		return 1; 	//Retorna sucesso para a inserção
	} else {	//Caso no NÃO seja nulo
		if ( valor != no->valor ) {
			if ( valor < no->valor ) {
				noADescer = &(no->esq);
			} else {
				noADescer = &(no->dir);
			}
			flagSucesso = insereEmRubNeg(valor, *noADescer, no, pai, &(*raiz) );
			switch (flagSucesso) {
				case 0:		//Caso não ocorreu inserção ou apenas uma atualização de cores, não chama o balanceamento, apenas propaga '1' para os níveis superiores verificarem o balanço
					return 1;
					break;
				case 1:		//Se a inserção ocorreu com sucesso, executa o balanço
					return balanceamento(*noADescer, no, pai, avo, raiz);
					break;
				case 2:		//Caso receba o sinal '2', significa que ocorreu uma rotação no nível inferior.
					return 2;
					break;
			}

		} else {	//Caso 'valor' seja IGUAL a 'no.valor'
			//Então o nó já está presente na árvore. Não faz nada...
			return 0;
		}
	}
	return 0; //Esta ação nunca deve ser alcançada
}


/**
 * Função que verifica o balanceamento da árvore Rubro-Negra. Esta
 * função é chamada pela função de inserção (recursiva) e avalia as
 * cores dos nós da árvore, aplicando a atualização de cores, ou a
 * rotação nos nós.
 *
 * O parâmetro 'no' aponta para o nó atual
 * O parâmetro 'pai' aponta para o pai do nó
 * O parâmetro 'avo' aponta para o avo de nó
 * O parâmetro 'bisavo' aponta para o bisavo do nó
 */
int balanceamento(tipoRN *no, tipoRN *pai, tipoRN *avo, tipoRN *bisavo, tipoRN **raiz) {
	tipoRN *tio, *raizSubArv;
	int flagBalanco;	//Será utilizada para sinalizar o tipo de ajuste executado (atualização de cores=1, rotação=2). No caso de atualização de cores, os níveis superiores precisam continar a verificar o balanço. No caso de rotação, não há necessidade. Por isso sua inicialização é feita = 2.
	flagBalanco = 2;

	if (pai != NULL) {
		//Verifica o caso onde há necessidade de executar uma atualização de cores ou rotação
		if ( pai->cor == VERMELHO ) {
			//Identifica o tio do nó
			if ( pai == avo->esq) {
				tio = avo->dir;
			} else {
				tio = avo->esq;
			}

			//Verifica a colocação do tio
			//Verifica se o tio é preto
			if ( tio == NULL || tio->cor == PRETO ) { //Há necessidade de executar uma rotação
				//Identifica qual rotação deve ser feita (simples ou dupla) à direita
				if ( pai == avo->esq ) {
					if ( no == pai->esq ) {	//Identifica rotação 'Simples'
						raizSubArv = pai;
						avo->esq = pai->dir;
						pai->dir = avo;
						pai->cor = PRETO;
						avo->cor = VERMELHO;
					} else {	//Identifica rotação 'Dupla'
						raizSubArv = no;
						pai->dir = no->esq;
						no->esq = pai;
						avo->esq = no->dir;
						no->dir = avo;
						no->cor = PRETO;
						avo->cor = VERMELHO;
					}
				} else { //Identifica qual rotação deve ser feita (simples ou dupla) à esquerda
					if ( no == pai->dir ) {	//Identifica rotação 'Simples'
						raizSubArv = pai;
						avo->dir = pai->esq;
						pai->esq = avo;
						pai->cor = PRETO;
						avo->cor = VERMELHO;
					} else {	//Identifica rotação 'Dupla'
						raizSubArv = no;
						pai->esq = no->dir;
						no->dir = pai;
						avo->dir = no->esq;
						no->esq = avo;
						no->cor = PRETO;
						avo->cor = VERMELHO;
					}
				}

				if ( bisavo != NULL) {
					if ( no->valor < bisavo->valor ) {
						bisavo->esq = raizSubArv;
					} else {
						bisavo->dir = raizSubArv;
					}
				} else {
					*raiz = raizSubArv;
				}
			} else {	//Se não, o tio é VERMELHO, então basta atualizar as cores dos pai, tio e avo.
				pai->cor = PRETO;
				tio->cor = PRETO;
				avo->cor = VERMELHO;
				flagBalanco = 0;
			}
		}
		return flagBalanco;
	} else {
		//Caso o pai seja nulo, não há mais nada a fazer, estamos em um nó filho da raiz.
		return 2;
	}
}


/**
 * Função principal de remoção em árvore Rubro-Negra
 *
 * Parâmetro 'valor' corresponde ao valor a ser encontrado para remoção
 * Parâmetro 'arv' aponta para a raiz da árvore
 */
tipoRN *removeRN (int valor, tipoRN *arv) {
	tipoRN *raiz;
	raiz = arv;

	removeEmRubNeg( valor, raiz, NULL, NULL, &raiz );
	if (raiz != NULL)
		raiz->cor = PRETO;

	return raiz;
}


/**
 * Função de remoção em árovre Rubro-Negra
 *
 */
int removeEmRubNeg(int valor, tipoRN *no, tipoRN *pai, tipoRN *avo, tipoRN **raiz) {
	int flagRemove;
	tipoRN *noADescer;
	tipoRN *aux;

	//Verifica se o nó é diferente de nulo (se for igual, então o valor nao existe na árvore)
	if (no != NULL) {
		if (no->valor != valor) { 	//Se o valor ainda não foi encontrado, desce para o filho adequado
			if (valor < no->valor) {	//Verifica o filho a descer
				noADescer = no->esq;
			} else {
				noADescer = no->dir;
			}

			flagRemove = removeEmRubNeg ( valor, noADescer, no, pai, raiz );
			switch (flagRemove) {
				case 0:
					return 1;
					break;
				case 1:
					return balanceamento(noADescer, no, pai, avo, raiz);
					break;
				case 2:
					return 2;
					break;
			}


		} else {	// O nó foi encontrado
			// Verifica se o nó a ser removido é uma folha
			if ( (no->esq == NULL) && (no->dir == NULL) ) {
				//Caso especial, quando ocorre a remoção da raiz e não há mais nós da árvore.
				if (no == *raiz) {
					free (no);
					*raiz = NULL;
					return 2;
				}

				if (no == pai->esq)
					pai->esq = NULL;
				else
					pai->dir = NULL;
				pai->cor = VERMELHO;

				free(no);
				return 0;
			} else {	//Remove um elemento intermediário da árvore.
				if (no->esq == NULL) {	//Se o filho esquerdo é nulo, então basta subir com o filho direito.
					aux = no;
					pai->cor = VERMELHO;
					if (no == pai->esq) {
						pai->esq = no->dir;
						//TODO é preciso chamar o balanceamento para o irmao

					} else {
						pai->dir = no->dir;
						//TODO é preciso chamar o balanceamento para o irmao
					}
					pai->cor = VERMELHO;
					free (aux);
					return 0; //Retorna um código 3
				} else if (no->dir == NULL ) { 	//Se o filho direito é nulo, então basta subir com o filho esquerdo.
					aux = no;
					if (no == pai->esq) {
						pai->esq = no->esq;
					} else {
						pai->dir = no->esq;
					}
					pai->cor = VERMELHO;
					free (aux);
					return 0;
				} else {	//Se tem os dois filhos, seleciona o filho mais à direita do nó esquerdo.
					//TODO executa a função buscaRemove
				}
			}
		}
	}

	return 0; //Se o valor não existe, retorna zero
}


 /*** Função que busca pelo nó mais à direita de uma sub-árvore, para tomar o lugar do nó encontrado para remoção ***/
 //	O parâmetro 'filho' corresponde ao nó filho esquerdo do nó a ser removido
 //	O parãmetro 'noChave' corresponde ao nó a ser removido
 tipoRN *buscaRemove ( tipoRN *filho, tipoRN *noChave, int *flagMudanca) {
	 tipoRN *noAux;
	 //Busca pelo nó mais à direita
	 if (filho->dir != NULL ) {
		 filho->dir = buscaRemove( filho->dir, noChave, flagMudanca);
		 if (flagMudanca) {
			 //TODO verifica coloração desta sub-árvore;
		 }
	 } else {		//Encontrou o nó mais à direita da árvore
		 noChave->valor = filho->valor;
		 noAux = filho;
		 filho = filho->esq;
		 free(noAux);
		 *flagMudanca = 1;	//Para propagar a informação da mudança na árvore, para que os outros nós possam verificar a coloração.
	 }
	 return filho;
 }
