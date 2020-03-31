#include <stdio.h>
#include <stdlib.h>

// Funcao para copiar o estado do jogo em outra lista para permitir manipulacao
// sem alteracao do jogo real
// Recebe como entrada a referencia do estado atual do jogo e a referecia para a
// copia do estado
void copyGameState(int* gameState, int* newGameState);
// Funcao para exibir estado atual do jogo do prompt de comando
void printTTT(int* gameState);

// Funcao que calcula a heuristica do estado de jogo fornecido
int calcHeuristics(int* gameState);
// Funcao para verificar se o jogo atual foi encerrado
int checkWinner(int* gameState);

// Funcao que realiza a analise min-max do estado atual do jogo e retorna a
// melhor jogada encontrada para a maquina
int minimaxDecision(int* gameState);
// Funcao que avalia, para uma jogada da maquina, qual o pior cenario possivel
// para a rodada do usuario e retorna a heuristica desse cenario
int minimaxValue(int* gameState);



int main(int argc, char **argv) {
  GameStart:;
  // Inicializacao do jogo
  int ttt[9] = {0,0,0,0,0,0,0,0,0};
  // 0: maquina joga primeiro / 1: usuario joga primeiro
  int turn = 0;

  system("cls");

  // Introducao do jogo
  printf("\n\tTic-Tac-Toe AI");
  // Input do oponente
  printf("\n\n\n\tDeseja jogar primeiro? (Sim: 1/Nao: 0): ");
  fflush(stdin);
  scanf("%d", &turn);

  // Caso o input seja diferente de 0, o usuario joga primeiro
  if(turn != 0) {
    turn = 1;
  }

  // O loop de jogo consiste em uma serie de inputs da maquina e do usuario ate
  // que um vencedor seja definido, atribuindo um valor diferente de -2 a variavel
  // winner (-2: jogo não acabou / -1: usuario venceu / 0: empate / 1: maquina venceu)
  int jogada;
  int winner = -2;
  while(winner == -2) {
    // Verifica quem deve jogar em seguida
    if(turn == 0) {
      // Caso seja a maquina, a jodada e decidida a partir do resultado da analise
      // min-max do estado atual do jogo
      jogada = minimaxDecision(ttt);

      ttt[jogada] = 1;

      turn = 1;
    } else {
      // Caso seja o usuario, primeiramente e atualizada a interface com o estado
      // atual do jogo para receber o input do usuario
      printTTT(ttt);
      printf("\n\tEscolha a posicao que desega jogar (ctrl+C para sair do jogo): ");
      fflush(stdin);
  		scanf("%d",&jogada);

      // Verifica se o input do usuario foi valido
      if(jogada == 0 || jogada > 9) {
        // Caso não esteja dentro do intervalo de posiccoes possiveis, uma mensagem
        // de erro e mostrada e, apos o usuario pressionar enter, o jogo volta
        // novamente para a tela de input de jogada
        printf("\n\n\tComando Invalido. Aperte enter para tentar novamente.");
				while(getchar() != '\n');
				fflush(stdin);
				getchar();
      } else {
        // Caso o input seja valido, verifica se a posicao selecionada esta ocupada
        // ou nao
        if(ttt[jogada-1] != 0) {
          // Caso a posicao ja esteja ocupada, uma mensagem de erro e mostrada
          // e, apos o usuario pressionar enter, o jogo volta novamente para a
          // tela de input de jogada
          printf("\n\n\tPosicao ja ocupada. Aperte enter para tentar novamente.");
  				while(getchar() != '\n');
  				fflush(stdin);
  				getchar();
        } else {
          // Caso a opcao nao esteja ocupada, a jogada e entao realizada
          ttt[jogada-1] = -1;

          turn = 0;
        }
      }
    }

    // Após cada rodada, verifica-se se o jogo foi finalizado ou nao
    winner = checkWinner(ttt);
  }

  // Caso o jogo tenha sido finalizado, o resultado da partida e então mostrado
  // ao usuario
  printTTT(ttt);
  printf("\n\n\tResultado do jogo: %s\n", (winner == 0) ? "Empate" : ( (winner == 1) ? "Maquina venceu" : "Jogador venceu" ));

  // Questiona se o usuario deseja jogar mais uma partida
  printf("\n\n\n\tDeseja jogar novamente? (Sim: 1/Nao: 0): ");
  fflush(stdin);
  scanf("%d", &turn);

  // Caso o input seja diferente de 0, o jogo e reiniciado
  if(turn != 0) {
    goto GameStart;
  }
}





void copyGameState(int* gameState, int* newGameState) {
  // Esta funcao simplesmente percorre o estado de jogo atual copiando seus valores
  // para uma outra lista com referencia diferente
  for(int i=0; i<9; i++) {
    newGameState[i] = gameState[i];
  }
}



void printTTT(int* gameState) {
  // Esta funcao atualiza o prompt de comando com uma representacao visual do jogo

  char gc[9];

  // O valor de cada posicao no jogo e subtituido pelo seu pespectivo simbolo
  // (0: espaço vazio / 1: "X" / 2: "O")
  for(int i=0; i<9; i++) {
    gc[i] = (gameState[i] == 0) ? ' ' : ( (gameState[i] == 1) ? 'X' : 'O' );
  }

  system("cls");

  printf("\n\n\tSeu simbolo: O\n\n");

  printf("\t1    |2    |3    \n");
  printf("\t  %c  |  %c  |  %c  \n", gc[0], gc[1], gc[2]);
  printf("\t     |     |     \n");
  printf("\t-----------------\n");
  printf("\t4    |5    |6    \n");
  printf("\t  %c  |  %c  |  %c  \n", gc[3], gc[4], gc[5]);
  printf("\t     |     |     \n");
  printf("\t-----------------\n");
  printf("\t7    |8    |9    \n");
  printf("\t  %c  |  %c  |  %c  \n", gc[6], gc[7], gc[8]);
  printf("\t     |     |     \n");
}



int calcHeuristics(int* gameState) {
  // Esta funcao calcula a heuristica do estado de jogo fornecido

  // Verifica-se uma vez cada linha, coluna e diagonal do jogo para avaliar as
  // chances de cada jogador ganhar, atribuindo um certo numero de pontos segundo
  // um conjunto de regras:
  //
  // - Pelo modo como a jogada de cada jogador foi representada, as jogadas da
  // maquina valem 1 ponto e as do usuario valem -1 pontos
  // - Caso todos os espaços em uma linha/coluna/diagonal estejam ocupados, se
  // todos forem iguais a 1 essa linha/coluna/diagonal vale 20 pontos, se forem
  // todos iguais a -1, vale -10 pontos, e se forem diferentes entre si, vale 0
  // pontos (dessa forma, atribui-se maior peso a chance de a maquina vencer e
  // descarta-se uma linha/coluna/diagonal em que nenhum dos jogadores pode vencer)
  // - Caso haja duas jogadas iguais em uma mesma linha/coluna/diagonal, essa
  // linha/coluna/diagonal vale o dobro da soma dos valores das jogadas (ou seja, -4
  // ou 4 dependendo de quem jogou)
  // - Em qualquer outro caso, o valor da linha/coluna/diagonal equivale a soma
  // dos valores das jogadas presentes nela (0: caso haja 0 jogadas ou duas jogadas
  // de jogadores diferentes / -1: caso haja apenas 1 jogada do usuario / 1: caso
  // haja apenas uma jogada da maquina)
  //
  // O valor final da heuristica do estado de jogo consiste em somar os valores
  // individuais de cada linha, coluna e diagonal
  //
  // Esse conjunto de regras foi concebido de modo que seguências de jogadas iguais
  // em uma mesma direcao pesassem de forma progressiva no valor total da heuristica,
  // ao mesmo tempo que permite que as jogadas da maquina sejam rebatidas pelas jogadas
  // do usuario ate o ponto em há uma chance concreta de a maquina vencer na jogada seguinte

  int tempHeuristics;
  int heuristics = 0;

  // Avalia a pontuacao para as colunas do jogo
  for(int i=0; i<3; i++) {
    // Verifica se ha 3 jogadas iguais na coluna
    if(gameState[i] != 0 && gameState[i] == gameState[i+3] && gameState[i] == gameState[i+6]) {
      // Caso haja, verifica se as jogadas sao da maquina ou do usuario
      if(gameState[i] == 1) {
        // Caso seja da maquina, acrescenta-se 20 pontos a heuristica
        heuristics += 20;
      } else {
        // Caso seja da maquina, subtrai-se 10 pontos a heuristica
        heuristics -= 10;
      }
    } else if(gameState[i] == 0 || gameState[i+3] == 0 || gameState[i+6] == 0) {
      // Caso não haja, soma-se os valores da coluna e verifica se a soma igual a
      // -2 ou 2, indicando haver duas jogadas iguais na coluna
      tempHeuristics = gameState[i] + gameState[i+3] + gameState[i+6];
      if(tempHeuristics == -2 || tempHeuristics == 2) {
        // Caso seja igual, acrescenta-se o dobro da soma a heuristica
        heuristics += tempHeuristics*2;
      } else {
        // Caso nao seja, acrescenta-se apenas a soma a heuristica
        heuristics += tempHeuristics;
      }
    }
    // Para o caso se haver 3 jogadas na coluna, porém diferentes entre si, o valor
    // resultante da coluna seria 0, logo, nao seria necessario ser acrescentado a
    // heuristica
  }

  // Avalia a pontuacao para as linhas do jogo
  for(int i=0; i<9; i += 3) {
    if(gameState[i] != 0 && gameState[i] == gameState[i+1] && gameState[i] == gameState[i+2]) {
      if(gameState[i] == 1) {
        heuristics += 20;
      } else {
        heuristics -= 10;
      }
    } else if(gameState[i] == 0 || gameState[i+1] == 0 || gameState[i+2] == 0) {
      tempHeuristics = gameState[i] + gameState[i+1] + gameState[i+2];
      if(tempHeuristics == -2 || tempHeuristics == 2) {
        heuristics += tempHeuristics*2;
      } else {
        heuristics += tempHeuristics;
      }
    }
  }

  // Avalia a pontuacao para a primeira diagonal do jogo
  if(gameState[0] != 0 && gameState[0] == gameState[4] && gameState[0] == gameState[8]) {
    if(gameState[0] == 1) {
      heuristics += 20;
    } else {
      heuristics -= 10;
    }
  } else if(gameState[0] == 0 || gameState[4] == 0 || gameState[8] == 0) {
    tempHeuristics = gameState[0] + gameState[4] + gameState[8];
    if(tempHeuristics == -2 || tempHeuristics == 2) {
      heuristics += tempHeuristics*2;
    } else {
      heuristics += tempHeuristics;
    }
  }

  // Avalia a pontuacao para a segunda diagonal do jogo
  if(gameState[2] != 0 && gameState[2] == gameState[4] && gameState[2] == gameState[6]) {
    if(gameState[2] == 1) {
      heuristics += 20;
    } else {
      heuristics -= 10;
    }
  } else if(gameState[2] == 0 || gameState[4] == 0 || gameState[6] == 0) {
    tempHeuristics = gameState[2] + gameState[4] + gameState[6];
    if(tempHeuristics == -2 || tempHeuristics == 2) {
      heuristics += tempHeuristics*2;
    } else {
      heuristics += tempHeuristics;
    }
  }

  return heuristics;
}



int checkWinner(int* gameState) {
  // Esta funcao verifica se a partida do jogo fou finalizada

  // Verifica se ha colunas com jogadas iguais
  for(int i=0; i<3; i++) {
    if(gameState[i] != 0 &&
       gameState[i] == gameState[i+3] &&
       gameState[i] == gameState[i+6]) {
      return gameState[i];
    }
  }

  // Verifica se ha linhas com jogadas iguais
  for(int i=0; i<9; i += 3) {
    if(gameState[i] != 0 &&
       gameState[i] == gameState[i+1] &&
       gameState[i] == gameState[i+2]) {
      return gameState[i];
    }
  }

  // Verifica se ha colunas com jogadas iguais
  if(gameState[0] != 0 &&
     gameState[0] == gameState[4] &&
     gameState[0] == gameState[8]) {
    return gameState[0];
  }
  if(gameState[2] != 0 &&
     gameState[2] == gameState[4] &&
     gameState[2] == gameState[6]) {
    return gameState[2];
  }

  // Verifica se todas as posicoes foram preenchidas
  for(int i=0; i<9; i++) {
    if(gameState[i] == 0) {
      return -2;
    }
  }
  return 0;
}



int minimaxDecision(int* gameState) {
  // Esta funcao realiza a analise min-max em relacao ao estado de jogo fornecido

  // Cria uma copia do estado de jogo para permitir a manipulacao do estado sem
  // interferir no jogo original
  int tempGameState[9];
  copyGameState(gameState, tempGameState);

  int bestPlay = 0;
  int bestPlayHeuristic = -100;
  int tempHeuristics;
  // Para cada posicao vazia do jogo, chama-se a funcao minimaxValue, que retornara
  // o menor valor de heuristica avaliado para as ramificacoes da rodada seguinte
  // em resposta a uma jogada nessa posicao especifica
  // Caso essa valor obtido seja maior do que melhor valor de heuristica calculado
  // ate entao, essa valor passa a ser o novo melhor valor e a posicao da respectiva
  // jogada e guardado
  for(int i=0; i<9; i++) {
    if(gameState[i] == 0) {
      tempGameState[i] = 1;
      tempHeuristics = minimaxValue(tempGameState);
      if(tempHeuristics > bestPlayHeuristic) {
        bestPlay = i;
        bestPlayHeuristic = tempHeuristics;
      }
      tempGameState[i] = 0;
    }
  }

  // Ao final da iteracao, a posicao da melhor jogada e retornada
  return bestPlay;
}



int minimaxValue(int* gameState) {
  // Esta funcao itera pelas possiveis jogadas do usuario em funcao do estado de
  // jogo fornecido

  // Cria uma copia do estado de jogo para permitir a manipulacao do estado sem
  // interferir no jogo original
  int tempGameState[9];
  copyGameState(gameState, tempGameState);

  int worstCaseHeuristic = 100;
  int tempHeuristics;
  // Para cada posicao vazia do jogo, calcula-se a heuristica do jogo como resultado
  // de uma jogada do usuario nessa posiao
  // Caso essa valor obtido seja menor do que pior valor de heuristica calculado
  // ate entao, essa valor passa a ser o novo pior valor
  for(int i=0; i<9; i++) {
    if(gameState[i] == 0) {
      tempGameState[i] = -1;
      tempHeuristics = calcHeuristics(tempGameState);
      if(tempHeuristics < worstCaseHeuristic) {
        worstCaseHeuristic = tempHeuristics;
      }
      tempGameState[i] = 0;
    }
  }

  // Ao final da iteracao, o pior valor de heuristica calculado e retornado
  return worstCaseHeuristic;
}
