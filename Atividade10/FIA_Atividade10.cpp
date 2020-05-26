#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>

// Função que executa o processo de Q Learning para o sistema Mundo de Grades
// Recebe como parâmetros o número de linhas e o número de colunas da grade desejada,
// a posição do estado objetivo (segundo a numeração das posições na grade), a
// taxa de aprendizagem alpha, o parâmetro gamma, o número de iterções desejadas
// e uma matriz para conter os valroes ótimos de estado (V[][0]) e as políticas
// ótimas associadas a cada estado (V[][1])
//
// A numeração das posições na grade é realizada sempre da esquerda para a direita
// e de cima para baixo em relação à posição 0,0
// ----------------
// |0   |1   |2   |
// |    |    |    |
// ----------------
// |3   |4   |5   |
// |    |    |    |
// ----------------
void QLearningGridWorld(int lines, int columns, int goal_state, float alpha, float gamma, int iter, float V[][2]);
// Função que fornece a numeração do novo estado decorrente de uma ação (action)
// realizada a partir de um estado inicial (state) dada uma grade de dimensões
// específicas
// Caso uma ação resulte em um estado fora dos limites da grade fornecida, a função
// returna -1, caso contrário retorna a númeração do estado novo resultante
// action == 0 (UP)    -> move agente uma linha para cima de sua posição
// action == 1 (RIGHT) -> move agente uma coluna para a direita de sua posição
// action == 2 (DOWN)  -> move agente uma linha para baixo de sua posição
// action == 3 (LEFT)  -> move agente uma coluna para a esquerda de sua posição
// Ex.: transicao(2, 3, 4, 0) -> 1
//      transicao(2, 3, 2, 2) -> 5
//      transicao(2, 3, 1, 0) -> -1
int transicao(int lines, int columns, int state, int action);







int main(int argc, char **argv) {
  // Inicializa semente para seleção aleatória de ações e estados iniciais
  srand(time(0));

  // Declara as dimensões da grade desejada
  int lines = 2;
  int columns = 3;

  // Declara a posição (estado) objetivo
  int goal_state = 2;

  // Declara a matriz para conter os valores de estado e a política ótimas após
  // a execução do algoritmo
  float V[lines*columns][2];

  // Executa a função com os valores especificados de taxa de aprendizagem e gamma
  QLearningGridWorld(lines, columns, goal_state, 0.1f, 0.9f, 100, V);

  // Exige no prompt os resultados obtidos
  printf("\n");
  for(int i=0; i<lines*columns; i++) {
    if(i != goal_state) {
      printf("V*(%d)=%.2f\t\tpi*(%d)=go to %.0f\n",i,V[i][0],i,V[i][1]);
    } else {
      printf("V*(%d)=%.2f\t\tpi*(%d)=end\n",i,V[i][0],i);
    }
  }

  return 0;
}






int transicao(int lines, int columns, int state, int action) {
  // Verifica se, para cada tipo de ação, o estado resultante não ultrapassará os
  // limites da grade definida
  if(action == 0 && state-columns > -1) {
    // Caso o resultado da ação seja válido, retornação a numeração do estado na
    // grade
    return state-columns;
  } else if(action == 1 && (state+1)%columns > 0) {
    return state+1;
  } else if(action == 2 && state+columns < lines*columns) {
    return state+columns;
  } else if(action == 3 && (state-1)%columns < columns-1) {
    return state-1;
  }
  // Caso a ação não seja válida, retorna-se -1 para auxiliar no processamente de erros
  return -1;
}

void QLearningGridWorld(int lines, int columns, int goal_state, float alpha, float gamma, int iter, float V[][2]) {
  // Declara a função Q como uma matriz para conter as relações de recompensa entre
  // cada posição
  int n = lines*columns;
  float Q[n][n];

  // Inicializa os valores de Q com 0.0
  for(int i=0; i<n; i++) {
    for(int j=0; j<n; j++) {
      Q[i][j] = 0.0f;
    }
  }

  // Inicializa as posições adjacentes ao estado objetivo com recompensas altas
  // pelas ações que levem até o estado objetivo
  if(goal_state-1 > -1 && (goal_state-1)%columns < columns-1) {
    Q[goal_state-1][goal_state] = 100.0f;
  }
  if(goal_state+1 < n && (goal_state+1)%columns > 0) {
    Q[goal_state+1][goal_state] = 100.0f;
  }
  if(goal_state-columns > -1) {
    Q[goal_state-columns][goal_state] = 100.0f;
  }
  if(goal_state+columns < n) {
    Q[goal_state+columns][goal_state] = 100.0f;
  }

  // Exibe no prompt a função Q inicializada
  printf("\n");
  for(int i=0; i<lines*columns; i++) {
    printf("[\t");
    for(int j=0; j<lines*columns; j++) {
      printf("%.2f",Q[i][j]);
      if(j != lines*columns-1) {
        printf(",\t");
      }
    }
    printf("\t]\n");
  }

  // Declara algumas variáveis auxiliares
  float maxQ, tempQ;
  int state = rand()%n;
  int newState, tempState1, tempState2, bestState;
  int iterLocal;
  int iterGlobal = 0;
  while(iterGlobal < iter) {
    // Verifica se o estado da iteração atual é o estado objetivo
    if(state == goal_state) {
      // Caso seja o estado objetivo, seleciona aleatoriamente outro estado e
      // volta para o início do loop
      // Esse processo é realizado até que um estado diferente do objetivo seja escolhido
      state = rand()%n;
      continue;
    }

    do {
      // Seleciona aleatoriamente uma ação para ser realizada a partir do estado atual
      newState = transicao(lines, columns, state, rand()%4);
      // Se a ação selecionada resultar em um novo estado válido, o loop é quebrado
      if(newState != -1) {
        break;
      }
    } while(true);

    maxQ = 0.0f;
    // Se o novo estado não for o estado objetivo, procura-se pela maior recompensa
    // possível decorrente de uma ação realizada à partir do novo estado
    if(newState != goal_state) {
      // Testa-se cada ações possível
      for(int i=0; i<4; i++) {
        tempState1 = transicao(lines, columns, newState, i);
        // Se a ação resultar em um estado novo válido e a recompensa associada
        // for maior do que a máxima até então, salva-se a a nova recompensa máxima
        if(tempState1 != -1 && Q[newState][tempState1] > maxQ) {
          maxQ = Q[newState][tempState1];
        }
      }
    }

    // Atualiza a função Q com o novo valor de recompensa esperado
    Q[state][newState] = (1.0-alpha)*Q[state][newState] + alpha*( Q[state][newState] + gamma*maxQ );
    state = newState;

    iterGlobal++;
  }

  // Após o final das iterações, percorre-se a função Q selecionando os maiores
  // valores de recompensa para cada estado, obtendo assim a função de valor de
  // estado e a política ótimos
  for(int i=0; i<n; i++) {
    V[i][0] = 0.0;
    V[i][1] = 0.0;
    for(int j=0; j<n; j++) {
      if(Q[i][j] > V[i][0]) {
        V[i][0] = Q[i][j];
        V[i][1] = j;
      }
    }
  }

  // Exibe no prompt a função Q pós treinamento
  printf("\n");
  for(int i=0; i<lines*columns; i++) {
    printf("[\t");
    for(int j=0; j<lines*columns; j++) {
      printf("%.2f",Q[i][j]);
      if(j != lines*columns-1) {
        printf(",\t");
      }
    }
    printf("\t]\n");
  }
}
