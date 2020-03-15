#include <stdio.h>
#include <stdlib.h>

// Estrutura com as informações do estado
typedef struct state {
  int values[2]; // Quantidade de água em dada jarro [jarro 3L, jarro 4L]
  int childNum; // Variável auxiliar para saber quantos nós filhos já foram gerados em relação a um nó pai
  struct state* parent; // Referência do estado pai do nó
} state;

// Possíveis ações
// - Recebem copo argumentos a referência do estado pai e um handle para receber
// os valores do estado filho resultante
void fill3(state* parent, int* childState); // Enche o jarro de 3L
void fill4(state* parent, int* childState); // Enche o jarro de 4L
void empty3(state* parent, int* childState); // Esvazia o jarro de 3L
void empty4(state* parent, int* childState); // Esvazia o jarro de 4L
void transf34(state* parent, int* childState); // Transfere conteúdo do jarro de 3L para o de 4L
void transf43(state* parent, int* childState); // Transfere conteúdo do jarro de 4L para o de 3L

// Verifica se um estado já foi visitado
bool checkDuplicate(int* s, state* listState, int listLength);
// Adiciona um estado ao final de uma lista de estados
void addList(int* s, int childNum, state* parent, state* listStates, int& listLength);

// Executa o método de busca cega por largura
int** breadthFirstSearch(int* initState, int& steps);
// Executa o método de busca cega por profundidade
int** depthFirstSearch(int* initState, int& steps);
// - Ambos recebem como argumentos o estado inicial e um handle para receber
// o número de passos da solução
// - Retornam uma lista de estados representando a sequência de passos para da
// solução



int main(int argc, char **argv) {

  // Estado inicial
  int initState[2] = {0,0};

  // Handles do número de passos dos métodos de busca
  int stepsBFS, stepsDFS;

  // Solução do problema por busca cega por largura
  int** solutionBFS = breadthFirstSearch(initState, stepsBFS);
  // Solução do problema por busca cega por profundidade
  int** solutionDFS = depthFirstSearch(initState, stepsDFS);

  printf("\nBreadth-First Search\n");
  if(stepsBFS	!= -1) {
    printf("\nEstado inicial - Jarro 3L: %d Jarro 4L: %d\n\n", solutionBFS[0][0], solutionBFS[0][1]);
    for(int i=1; i<stepsBFS; i++) {
      printf("Passo %d - Jarro 3L: %d Jarro 4L: %d\n", i, solutionBFS[i][0], solutionBFS[i][1]);
    }
  } else {
    printf("\nSolucao nao encontrada\n");
  }


  printf("\n\nDepth-First Search\n");
  if(stepsDFS != -1) {
    printf("\nEstado inicial - Jarro 3L: %d Jarro 4L: %d\n\n", solutionDFS[0][0], solutionDFS[0][1]);
    for(int i=1; i<stepsDFS; i++) {
      printf("Passo %d - Jarro 3L: %d Jarro 4L: %d\n", i, solutionDFS[i][0], solutionDFS[i][1]);
    }
  } else {
    printf("\nSolucao nao encontrada\n");
  }

  return 0;
}



int** breadthFirstSearch(int* initState, int& steps) {

  // Verifica se o estado inicial é a solução
  if(initState[0] == 2 || initState[1] == 2) {
    steps = 1;
    int** solution = new int*;
    solution[0] = new int[2];
    solution[0][0] = initState[0];
    solution[0][1] = initState[1];
    return solution;
  }

  // Comprimento da fila (indica também o final da fila)
  int queueLength = 1;
  // Índice do início da fila
  int queueFront = 0;

  // Declara a fila e insere o nó inicial
  state queue[30];
  queue[0].values[0] = initState[0];
  queue[0].values[1] = initState[1];
  queue[0].childNum = 0;
  queue[0].parent = NULL;


  state* parent;
  int childState[2];
  int childNum = 1;

  // Variáveis de verificação
  bool found = false; // Indica se uma solução foi encontrada
  int iter = 0; // Número de iterações
  do {
    // Seleciona nó na frente da fila
    parent = &queue[queueFront];

    // Realiza uma das ações para gerar um nó filho
    switch(childNum) {
    	case 1:
    		fill3(parent, childState);
    		break;
    	case 2:
    		fill4(parent, childState);
    		break;
      case 3:
    		empty3(parent, childState);
    		break;
      case 4:
    		empty4(parent, childState);
    		break;
    	case 5:
    		transf34(parent, childState);
    		break;
      case 6:
    		transf43(parent, childState);
    		break;
  	}
    // Verifica se o estado do nó filho é a solução
    if(childState[0] == 2 || childState[1] == 2) {
      // Caso verdadeiro, o nó é adicionado ao final da fila e o do while loop é quebrado
      addList(childState, childNum, parent, queue, queueLength);
      found = true;
      continue;
    }
    // Verifica se o estado do nó filho não é duplicado
    if(!checkDuplicate(childState, queue, queueLength)) {
      // Caso não seja, o nó é adicionado ao final da fila
    	addList(childState, childNum, parent, queue, queueLength);
    }

    // Verifica se todos os filhos do nó pai foram produzidos
    if(childNum >= 6) {
      // Caso verdadeiro, a frente da fila é movida para o próximo nó
      childNum = 1;
      queueFront++;
    } else {
      // Caso falso, o próximo nó filho é gerado
      childNum++;
    }

    iter++;
  } while(!found && iter < 500);

  int** solution = 0;

  // Verifica se uma solução foi encontrada
  if(found) {
    // Caso verdadeiro, a lista de passos é gerada

    // Pelo fato da fila provavelmente não conter apenas os passos da solução,
    // conta-se primeiro o número de nós na hierarquia do ramo escolhido
    parent = &queue[queueLength-1];
    steps = 0;
    do {
      steps++;
      parent = parent->parent;
    } while(parent);

    solution = new int*[steps];

    // Com o número de nós, percorre-se novamente os nós do ramo, agora ordernando
    // eles na sequência certa de passos
    parent = &queue[queueLength-1];
    for(int i=steps-1; i>=0; i--) {
      solution[i] = new int[2];
      solution[i][0] = parent->values[0];
      solution[i][1] = parent->values[1];
      parent = parent->parent;
    }
  } else {
    steps = -1;
  }

  return solution;
}




int** depthFirstSearch(int* initState, int& steps) {

  // Verifica se o estado inicial é a solução
  if(initState[0] == 2 || initState[1] == 2) {
    steps = 1;
    int** solution = new int*;
    solution[0] = new int[2];
    solution[0][0] = initState[0];
    solution[0][1] = initState[1];
    return solution;
  }

  // Índice do topo da pilha ( considera o próximo índice após o nó no topo
  // da pilha para facilitar reuso da função addList)
  int pileTop = 1;

  // Declara a pilha e insere o nó inicial
  state pile[50];
  pile[0].values[0] = initState[0];
  pile[0].values[1] = initState[1];
  pile[0].childNum = 0;
  pile[0].parent = NULL;


  state* parent;
  int childState[2];
  int childNum = 1;

  // Variáveis de verificação
  bool found = false; // Indica se uma solução foi encontrada
  int iter = 0; // Número de iterações
  do {
    // Seleciona nó do topo da pilha
    parent = &pile[pileTop-1];

    // Realiza uma das ações para gerar um nó filho
    switch(childNum) {
    	case 1:
    		fill3(parent, childState);
    		break;
    	case 2:
    		fill4(parent, childState);
    		break;
      case 3:
    		empty3(parent, childState);
    		break;
      case 4:
    		empty4(parent, childState);
    		break;
    	case 5:
    		transf34(parent, childState);
    		break;
      case 6:
    		transf43(parent, childState);
    		break;
  	}
    // Verifica se o estado do nó filho é a solução
    if(childState[0] == 2 || childState[1] == 2) {
      // Caso verdadeiro, o nó é adicionado ao topo da pilha e o do while loop é quebrado
      addList(childState, childNum, parent, pile, pileTop);
      found = true;
      continue;
    }
    // Verifica se o estado do nó filho não é duplicado
    if(checkDuplicate(childState, pile, pileTop)) {
      // Caso verdadeiro, verifica se todos os filhos do nó pai atual foram testados
    	if(childNum >= 6) {
        // Caso verdadeiro, volta um nó de cada vez verificando se ainda há nós
        // filhos para serem gerados daquele nó
    		do {
    			childNum = pile[pileTop-1].childNum;
    			pileTop--;
    		} while(childNum == 6);
    	}
    	childNum++;
    } else {
      // Caso false, o nó é adicionado ao topo da pilha e o childNum volta para 1
      // para reiniciar a contagem de filhos
    	addList(childState, childNum, parent, pile, pileTop);
    	childNum = 1;
    }

    iter++;
  } while(!found && iter < 500);

  int** solution = 0;

  // Verifica se uma solução foi encontrada
  if(found) {
    // Caso verdadeiro, a lista de passos é gerada

    // Pelo fato de o backtracking decrementar o pileTop para testar novos ramos
    // da árvore de possibilidades, apenas os nós pertencentes à solução estam
    // presentes pilha, logo, basta apenas percorrer a pilha para listar a
    // sequência de passos
    steps = pileTop;

    solution = new int*[pileTop];

    for(int i=0; i<pileTop; i++) {
      solution[i] = new int[2];
      solution[i][0] = pile[i].values[0];
      solution[i][1] = pile[i].values[1];
  	}
  } else {
    steps = -1;
  }

  return solution;
}



void fill3(state* parent, int* childState) {
  childState[0] = 3; // Define 3L no jarro de 3L
  childState[1] = parent->values[1]; // Mantêm a quantidade dentro do jarro de 4L
}

void fill4(state* parent, int* childState) {
  childState[0] = parent->values[0]; // Mantêm a quantidade dentro do jarro de 3L
  childState[1] = 4; // Define 4L no jarro de 4L
}

void empty3(state* parent, int* childState) {
  childState[0] = 0; // Define 0L no jarro de 3L
  childState[1] = parent->values[1]; // Mantêm a quantidade dentro do jarro de 4L
}

void empty4(state* parent, int* childState) {
  childState[0] = parent->values[0]; // Mantêm a quantidade dentro do jarro de 3L
  childState[1] = 0; // Define 0L no jarro de 4L
}

void transf34(state* parent, int* childState) {
  int new3 = 0;
  int new4 = parent->values[0] + parent->values[1];
  // Verifica se a soma das quantidades dos dois jarros é maior do que a capacidade
  // do jarro de 4L
  if(new4 > 4) {
    // Caso verdadeiro, a satura o jarro de 4L e joga a quantidade restante
    // para o jarro de 3L
    new3 = new4 - 4;
    new4 = 4;
  }

  childState[0] = new3;
  childState[1] = new4;
}

void transf43(state* parent, int* childState) {
  int new3 = parent->values[0] + parent->values[1];
  int new4 = 0;
  // Verifica se a soma das quantidades dos dois jarros é maior do que a capacidade
  // do jarro de 3L
  if(new3 > 3) {
    // Caso verdadeiro, a satura o jarro de 3L e joga a quantidade restante
    // para o jarro de 4L
    new3 = 3;
    new4 = new3 - 3;
  }

  childState[0] = new3;
  childState[1] = new4;
}

bool checkDuplicate(int* s, state* listStates, int listLength) {
  // Percorre todos os itens da lista de estados e verifica se o estado s está
  // presente nela
  for(int i=0; i<listLength; i++) {
    if(listStates[i].values[0] == s[0] && listStates[i].values[1] == s[1]) {
      return true;
    }
  }
  return false;
}

void addList(int* s, int childNum, state* parent, state* listStates, int& listLength) {
  // Insere um novo nó no fim da lista fornecida e incrementa o comprimento da
  // lista
  listStates[listLength].values[0] = s[0];
  listStates[listLength].values[1] = s[1];
  listStates[listLength].childNum = childNum;
  listStates[listLength].parent = parent;
  listLength++;
}
