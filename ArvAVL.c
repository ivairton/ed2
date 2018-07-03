/**
  * UFMT - CUA - Ciência da Computação
  * Disciplina: ED2
  * Professor: Ivairton
  * Autor: Ivairton
  * Data:05/2017
  *
  * Funções para inserção, remoção e manipulação da
  * estrutura de dados Árvore AVL.
  *
  */

#include <stdio.h>
#include <stdlib.h>

/*** Declaração de tipos ***/
struct estruturaAVL {		//Estrutura do nó
	struct estruturaAVL *esq;
	struct estruturaAVL *dir;
	int valor;
	int fb;
};
typedef struct estruturaAVL tipoAVL;

/*** Prototipação das funções ***/
int insereAVL (tipoAVL **T, int dado);	//Função principal de inserção
void verificaInsEsquerda( tipoAVL **T ); 	//Função que verifica a rotação a ser feita em uma inserção à esquerda.
void verificaInsDireita( tipoAVL **T );	//Função que verifica a rotação a ser feita em uma inserção à direita.
void rotacaoDireita( tipoAVL **T );	//Rotação simples à direita
void rotacaoEsqDir( tipoAVL **T );	//Rotação dupla, esquerda e direita
void rotacaoEsquerda( tipoAVL **T );	//Rotação simples à esquerda
void rotacaoDirEsq( tipoAVL **T );	//Rotação dupla, direita e esquerda
void impressaoPreOrdem( tipoAVL *T );	//Impressao em pre-ordem (p/ debugacao)
void removeAVL(tipoAVL **T, int valor); // FUncao principal de remocao
void remAVL(tipoAVL **T, int chave, int *flagMudanca); //Funcao de remoçao
int buscaRemove(tipoAVL **T); //Funcao que encontra o maio valor de um ramos esquerdo da árvore, retornando este valor e removendo seu no



/*** Função principal de inserção em Árvore AVL ***/
// O parâmetro 'T' corresponde à raiz da subárvore onde será inserido o novo dado.
// O parâmetro 'dado' corresponde ao valor a ser inserido da AVL.
int insereAVL (tipoAVL **T, int dado) {
	//Variável utilizada para sinalizar o sucesso do procedimento de inserção na AVL.
	int flagSucesso;
	flagSucesso = 0;

	//Verifica se a raiz informada como parâmetro é nula. Nesse caso será alocado um novo nó na memória.
	if (*T == NULL) {
		*T = (tipoAVL*) malloc(sizeof(tipoAVL));
		if (*T != NULL) {
			(*T)->esq = NULL;
			(*T)->dir = NULL;
			(*T)->valor = dado;
			(*T)->fb = 0;
			return 1; 	//Ao inserir o novo nó, será retornado 1 (verdadeiro), de modo a modificar a variável 'flagSucesso' do procedimento pai (aquele que chamou o procedimento de maneira recursiva). Note que a execução desta função terminaria aqui.
		} else {
			return 0;
		}
	}

	//Verifica se o dado a ser inserido é menor que o valor da raiz da sub-árvore atual. Nesse caso, será necessário descer à esquerda da árvore.
	if ( (*T)->valor > dado ) {
		//Executa chamada recursiva com o filho à esquerda.
		flagSucesso = insereAVL( &(*T)->esq, dado );

		//Se ocorrer sucesso na inserção, verifica o balanceamento da árvore, executando uma rotação (a ser verificada) se necessário.
		if (flagSucesso) {
			switch ( (*T)->fb ) {
				case 1:		//Caso dir > esq, então gera equilíbrio, add um elemento à esq.
					(*T)->fb = 0;
					flagSucesso = 0; //return 0;
					break;	//Finaliza 'case 1'
				case 0:		//Alturas iguais, então esq se torna maior que dir.
					(*T)->fb = -1;
					//return 1;
					break;	//Finaliza 'case 0'
				case -1:	//Será preciso executar uma rotação (a ser verificada).
					verificaInsEsquerda( &(*T) );
					flagSucesso = 0;//return 0;
					break;	//Finaliza 'case -1'
			}
		}
	} else 		//Se não é uma inserção à esq, então é à dir. Nesse caso será necessário descer à direita da árv.
	if ( (*T)->valor < dado ) {
		//Executa chamada recursiva com o filho à direita
		flagSucesso = insereAVL( &(*T)->dir, dado );
		//Se ocorrer sucesso na inserção, verifica o balanceamento da árvore, executando uma rotação (a ser verificada) se necessário.
		if (flagSucesso) {
			switch ( (*T)->fb ) {
				case -1:	//Caso dir < esq, então gera equilíbrio, add um elemento à dir.
					(*T)->fb = 0;
					flagSucesso = 0;
					break;	//Finaliza 'case -1'
				case 0:		//Alturas iguais, então dir se torna maior que esq'.
					(*T)->fb = 1;
					break;	//Finaliza 'case 0'
				case 1:	//Será preciso executar uma rotação (a ser verificada).
					verificaInsDireita( &(*T) );
					flagSucesso = 0;
					break;	//Finaliza 'case -1'
			}
		}
	} else {	//Caso o valor a ser inserido já esteja presente na AVL (não faz nada).
		flagSucesso = 0;
	}
	return flagSucesso;
}


/***Função que verifica a rotação a ser feita em uma inserção à esquerda. ***/
void verificaInsEsquerda( tipoAVL **T ) {
	//Considerando que o novo nó foi inserido à esq e causou desbalanceamento = -2
	tipoAVL *u;
	u = (*T)->esq;

	if ( u->fb == -1) {	//Verifica o sinal do filho (se igual, então rotação simples.
		rotacaoDireita( &(*T) );
	} else {		//Caso contrário, rotação dupla.
		rotacaoEsqDir( &(*T) );
	}

	//Atualiza o balanceamento da nova raiz (esse procedimento pode ser feito diretamente nas funções de rotação).
	(*T)->fb = 0;
}


/*** Função que verifica a rotação a ser feita em uma inserção à direita. ***/
void verificaInsDireita( tipoAVL **T ) {
	//Considerando que o novo nó foi inserido à dir e causou desbalanceamento = -2
	tipoAVL *u;
	u = (*T)->dir;

	if ( u->fb == 1) {	//Verifica o sinal do filho (se igual, então rotação simples.
		rotacaoEsquerda( &(*T) );
	} else {		//Caso contrário, rotação dupla.
		rotacaoDirEsq( &(*T) );
	}

	//Atualiza o balanceamento da nova raiz (esse procedimento pode ser feito diretamente nas funções de rotação).
	(*T)->fb = 0;
}


/*** Função de rotação simples à direita ***/
void rotacaoDireita( tipoAVL **T ) {
	tipoAVL *u;
	u = (*T)->esq;

	(*T)->esq = u->dir;
	u->dir = *T;

	(*T)->fb = 0;
	u->fb = 0;
	(*T) = u;
	//Poderia atualizar o FB do novo *T.
	//(*T)->fb = 0;
}


/*** Função de rotação dupla: esquerda e direita ***/
void rotacaoEsqDir( tipoAVL **T ) {
	tipoAVL *u, *v;
	u = (*T)->esq;
	v= u->dir; 	//Ou, se preferir, (*T)->esq->dir
	u->dir = v->esq;
	v->esq = u;
	(*T)->esq = v->dir;
	v->dir = *T;

	//Verifica os valore do FB do nó 'v', para correta atualização do valor do balanço da raiz (velha) e do antigo filho esquerdo ('u')
	if ( v->fb == -1 ) {
		(*T)->fb = 1;
		u->fb = 0;
	} else if ( v->fb == 0 ) {
		(*T)->fb = 0;
		u->fb = 0;
	} else if ( v->fb == 1 ) {
		(*T)->fb = 0;
		u->fb = -1;
	}

	*T = v;
	//Se preferir, atualize o valor da nova raiz para zero.
}


/*** Função de rotação simples à esquerda ***/
void rotacaoEsquerda( tipoAVL **T ) {
	tipoAVL *u;
	u = (*T)->dir;

	(*T)->dir = u->esq;
	u->esq = *T;

	(*T)->fb = 0;
	(*T) = u;
	//Poderia atualizar o FB do novo *T.
}


/*** Função de rotação dupla: direita e esquerda ***/
void rotacaoDirEsq( tipoAVL **T ) {
	tipoAVL *u, *v;
	u = (*T)->dir;
	v= u->esq; 	//Ou, se preferir, (*T)->dir->esq
	u->esq = v->dir;
	v->dir = u;
	(*T)->dir = v->esq;
	v->esq = *T;

	//Verifica os valore do FB do nó 'v', para correta atualização do valor do balanço da raiz (velha) e do antigo filho esquerdo ('u')
	if ( v->fb == -1 ) {
		(*T)->fb = 0;
		u->fb = 1;
	} else if ( v->fb == 0 ) {
		(*T)->fb = 0;
		u->fb = 0;
	} else if ( v->fb == 1 ) {
		(*T)->fb = -1;
		u->fb = 0;
	}

	*T = v;
	//Se preferir, atualize o valor da nova raiz para zero.
}


/*** Função para impressao em pré-ordem, para debugação da árvore AVL ***/
 void impressaoPreOrdem( tipoAVL *T ) {
	 if (T != NULL ) {
		 printf("[%d] ", T->valor);
		 impressaoPreOrdem( T->esq );
		 impressaoPreOrdem( T->dir );
	 }
 }


/*** Funcao principal de remoção em AVL ***/
void removeAVL(tipoAVL **T, int valor) {
	// Chama funcao secundária para remoçao
	int flag;
	remAVL (T, valor, &flag);
}

 /*** Função de remoção em árvore AVL ***/
 void remAVL (tipoAVL **T, int chave, int *flagMudanca) {
	if (*T == NULL) {
		//Nó da árvore está nulo, então a chave não foi encontrada
		*flagMudanca = 0;
	} else {	//Busca pela chave na árvore
		if ( (*T)->valor > chave) {		//Busca pela chave na sub-árvore filho esq.
			//Executa chamada recursiva da função, atualizando o filho esquerdo com o retorno da função (no caso de uma remoção o filho será alterado, caso contrário, permanecerá como está).
			remAVL(&(*T)->esq, chave, flagMudanca);
			if (*flagMudanca == 1) {
				//verificaInsDireita( &T );
			}
		} else if ((*T)->valor < chave) {	//Busca pela chave na sub-arvore filho dir.
			//Executa chamada recursiva da função (...)
			remAVL(&(*T)->dir, chave, flagMudanca);
			if (*flagMudanca == 1) {
				//verificaInsEsquerda( &T );
			}
		} else {	//O valor chave buscado foi encontrado
			//Se o nó é uma folha:
			if ( ((*T)->esq == NULL) && ((*T)->dir == NULL) ) {
				free(*T);
				flagMudanca = 1;
				(*T) = NULL;
			} else	if ( ((*T)->dir == NULL) && ((*T)->esq != NULL) ) {	//Se apenas o filho direito é nulo, então basta "subir" com o filho esquerdo.

				tipoAVL *aux;
				aux = *T;
				(*T) = (*T)->esq;
				free(aux);	//Desaloca o nó encontrado da memória.
				*flagMudanca = 1;
			} else if ( ((*T)->esq == NULL) && ((*T)->dir != NULL) ) {	//Se apenas o filho esquerdo é nulo, então basta "subir" com o filho direito.
				tipoAVL *aux;
				aux = (*T);
				(*T) = (*T)->dir;
				free(aux);	//Desaloca o nó encontrado da memória.
				*flagMudanca = 1;
			} else {	//O nó possui os dois filhos, então busca pelo elemento mais à direita do nó esquerdo.
				(*T)->valor = buscaRemove(&(*T)->esq);
				//Deverá ser verificado se há a necessidade de executar uma rotação, pois estamos descendo à esquerda da árvore.
				if (*flagMudanca) {
				//	verificaInsDireita( &(T) );
				}
			}
		}
	}
 }

// FUnçao que busca pelo maior elemento de um ramo esquerdo
int buscaRemove(tipoAVL **T) {
	if ( ((*T)->esq == NULL) && ((*T)->dir == NULL) ) {
		int i;
		i = (*T)->valor;
		free((*T));
		(*T) = NULL;
		return i;
	} else if ( ((*T)->esq != NULL) && ((*T)->dir == NULL) ) {
		int i;
		i = (*T)->valor;
		(*T) = (*T)->esq;
		return i;
	} else {
		return buscaRemove(&(*T)->dir);
	}
}

 /*
 /*** Função que busca pelo nó mais à direita de uma sub-árvore, para tomar o lugar do nó encontrado para remoção ***
 //	O parâmetro 'T' corresponde ao nó filho esquerdo do nó a ser removido
 //	O parãmetro 'noChave' corresponde ao nó a ser removido
 tipoAVL *buscaRemove ( tipoAVL *T, tipoAVL *noChave, int *flagMudanca) {
	 tipoAVL *noAux;
	 //Busca pelo nó mais à direita
	 if (T->dir != NULL ) {
		 T->dir = buscaRemove( T->dir, noChave, flagMudanca);
		 if (*flagMudanca) {
			 verificaInsEsquerda( &(T) );
		 }
	 } else {		//Encontrou o nó mais à direita da árvore
		 noChave->valor = T->valor;
		 noAux = T;
		 T = T->esq;
		 free(noAux);
		 *flagMudanca = 1;	//Para propagar a informação da mudança na árvore, para que os outros nós possam verificar o FB.
	 }
	 return T;
 }


 /**-------------------------------------------------------------------
  * Remoção com empilhamento
  * ------------------------------------------------------------------
  *
  * Empilha cada nó visitado durante a descida para encontrar o nó a ser removido,
  * na subida, verifica o balanço de cada nó.
  */

 /*
*  Atualização do FB e balanceamento para a raíz esquerda
*
AVL *balanceamento_esquerdo(AVL *no, bool *h) {

    AVL *f_dir;
    int fb_dir;

    switch (no->fb) {
        case 1:
            no->fb = 0;
            break;
        case 0:
            no->fb = -1;
            *h = false;
            break;
        case -1:
                f_dir = no->dir;
                fb_dir = f_dir->fb;
                if (fb_dir <= 0) {
                    f_dir = rotacaoRR(no);
                 if (fb_dir == 0) {
                    no->fb = -1;
                       f_dir->fb = 1;
                       *h = false;
                    }
                    else {
                        no->fb = 0;
                        f_dir->fb = 0;
                    }
                    no = f_dir;
                }
                else {
                    no = rotacaoRL(no);
                    no->fb = 0;
                }
    }
    return(no);
}

/*
*  Atualização do FB e balanceamento para a raiz direita
*
AVL *balanceamento_direito(AVL *no, bool *h) {

    AVL *f_esq;
    int fb_esq;

    switch (no->fb) {
        case -1:
            no->fb = 0;
            break;
        case 0:
            no->fb = 1;
            *h = false;
            break;
        case 1:
            f_esq = no->esq;
            fb_esq = f_esq->fb;
            if (fb_esq >= 0) {
                f_esq = rotacaoLL(no);
                if (fb_esq == 0) {
                    no->fb = 1;
                    f_esq->fb = -1;
                    *h = false;
                }
                else {
                    no->fb = 0;
                    f_esq->fb = 0;
                }
                no = f_esq;
            }
            else {
                no = rotacaoLR(no);
                no->fb = 0;
            }
    }
    return(no);
}

/*
*  Busca nó substituto e realizada a remoção (busca o mais à direita do nó esquerdo
*
AVL *busca_remove(AVL *no, AVL *no_chave, bool *h) {

    AVL *no_removido;
    if (no->dir != NULL) {
        no->dir = busca_remove(no->dir, no_chave, h);
        if (*h)
            no = balanceamento_direito(no, h);
    }
    else {
        no_chave->info = no->info;
        no_removido = no;
        no = no->esq;
        if (no != NULL)
            no->pai = no_removido->pai;
        *h = true;     //Deve propagar a atualização dos FB
        free(no_removido);
    }
    return(no);
}

/*
*  Remoção da Árvore AVL
*
AVL *remove(AVL *raiz, int info, bool *h) {

    if (raiz == NULL) {
        printf("Chave não localizada !");
        *h = false;
    }
    else {
        if (raiz->info > info) {
            raiz->esq = remove(raiz->esq, info, h);
            if (*h)
                raiz = balanceamento_esquerdo(raiz, h);
        }
        else
            if (raiz->info < info) {
                raiz->dir = remove(raiz->dir, info, h);
                if (*h)
                    raiz = balanceamento_direito(raiz,h);
            }
            else { //Encontrou o elemento a ser removido
                if (raiz->dir == NULL) {
                    if (raiz->esq != NULL) //Escolhe o nó à esquerda como substituto
                        raiz->esq->pai = raiz->pai;
                    raiz = raiz->esq;
                    *h = true;
                }
                else
                    if (raiz->esq == NULL) {
                        if (raiz->dir != NULL) //Escolhe o nó à direita como substituto
                            raiz->dir->pai = raiz->pai;
                        raiz = raiz->dir;
                        *h = true;
                    }
                    else { // Busca o elemento mais à direita do nó esquerdo
                        raiz->esq = busca_remove(raiz->esq, raiz, h);
                        //Se necessário efetua balanceamento (Esquerdo pois a função
                        //busca_remove foi para o nó esquerdo)
                        if (*h)
                            raiz = balanceamento_esquerdo(raiz, h);
                    }
            }
    }
    return(raiz);
}
 */
