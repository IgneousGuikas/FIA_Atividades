#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

// Este arquivo procura implementar o algoritmo ID3 para treinamento de árvores
// de decisão, empregando algumas adaptações para permitir o uso de atributos
// quantitativos na partição dos dados.

// Para cada nó da árvora de decisão, cada atributo do conjunto de
// dados que não tenha sido utilizado ainda naquele ramo é testado segundo
// os conceitos da teoria da informação para averiguar se é o atributo que
// melhor particiona os dados fornecidos ao nó pela partição anterior.

// Para cada atributo testado, cada elemento do conjunto de dados fornecido ao
// nó é testado para averiguar qual o melhor valor possível para o atributo
// particionar os dados, promovendo o maior ganho de informação.



// Estrutura para armazenar os nós da árvore de decisão
// attribute -> atributo escolhido para particionar os dados (caso o nó seja um
//              nó folha, attribute assume valor -1)
// partition -> valor de corte escolhido para delimitar a partição dos dados (caso
//              o nó seja um nó folha, partition assume valor -1)
// category -> categoria à qual os dados pertencem ao cairem no nó (caso o nó não
//             seja um nó folha, category assume valor -1)
// lessThan -> referência do nó destino para os dados que sejam menores que ou
//             iguais ao valor de corte escolhido em relação ao atributo escolhido
//             (caso o nó seja um nó folha, lessThan assume valor NULL)
// biggerThan -> referência do nó destino para os dados que sejam maiores que o
//               valor de corte escolhido em relação ao atributo escolhido (caso
//              o nó seja um nó folha, biggerThan assume valor NULL)
typedef struct Node {
  int attribute;
  float partition;
  int category;
  struct Node *lessThan;
  struct Node *biggerThan;
} Node;



// Função que calcula a quantidade de informação necessária pra representar um
// sistema segundo um conjunto de probabilidades
//
// Recebe um vetor com as probabilidas de cada categoria que o conjunto de dados
// possa assumir e reforna o número de bit necessário pra representar esse conjunto
float I(float* prob);

// Função que calcula o ganho de informação proporcionado por uma partição
//
// Recebe uma matriz com a relação do número de dados pertencentes a cada categoria
// em cada partição e o tamanho do conjunto de dados recebido pelo nó e retorna
// o ganho de informação proporcionado por essa partição
// partition[0][] - lessThan / partition[1][] - biggerThan
// partition[][0] - Iris-setosa / partition[][1] - Iris-versicolor / partition[][2] - Iris-virginica
float InfoPartition(int partition[2][3], int n);

// Função que seleciona o melhor valor de corte para um atributo
//
// Recebe o conjunto de dados do nó, o tamanho do conjunto de dados, o índice do
// atributo e retorna o melhor valor de corte para esse atributo, assim como
// o valor do ganho de informação associado (float& info)
float getBestPartition(float data[][5], int n, int attr, float& info);

// Função que realiza o treinamento da árvore de decisão de forma recursiva
//
// Recebe a referência do nó a ser treinado, o conjunto de dados disponível para
// o nó, o tamanho do conjunto de dados, a lista de atributos disponíveis para uso
// e o comprimento dessa lista. Não retorna nada, porém, ao final da execução,
// a árvore de decisão já estará disponível, sendo o primeiro nó fornecido o nó
// raíz
void trainDecisionTree(Node* node, float data[][5], int n, int attr[], int a);

// Função que exibe a árvore de decisão treinada a partir de seu nó raíz
void printDecisionTree(Node* node);

// Função que categoriza um conjunto de atributos dada a referência de uma árvore
// de decisão
int evalData(Node* node, float data[4]);

// Função que categoriza todo um conjunto de dados segundo uma árvora de decisão e
// calcula a eficiência da categorização
//
// Recebe a referência do nó raíz da árvore de decisão, o conjunto de dados incluindo
// as categorias verdadeiras dos dados e o tamanho do conjunto de dados e retorna
// o valor de eficiência da categorização da árvore
float evalEfficiency(Node* node, float data[][5], int n);


int main(int argc, char **argv) {

  int a = 4; // Numero de atributos para cada dado

  // 0 - sepal_length / 1 - sepal_width / 2 - petal_length / 3 - petal_width
  int attributes[4] = {0,1,2,3}; // Conjunto inicial de atributos disponíveis

  int n = 150; // Numero de dados

  // Conjunto de dados
  // A quinta coluna contém as categorias atribuidas a cada dado segundo a relação
  // abaixo
  // 0 - Iris-setosa / 1 - Iris-versicolor / 2 - Iris-virginica
  // Esta representação facilitará o processo de treinamento posteriormente
  float data[150][5] = {
    { 5.1,3.5,1.4,0.2,0.0 },
    { 4.9,3.0,1.4,0.2,0.0 },
    { 4.7,3.2,1.3,0.2,0.0 },
    { 4.6,3.1,1.5,0.2,0.0 },
    { 5.0,3.6,1.4,0.2,0.0 },
    { 5.4,3.9,1.7,0.4,0.0 },
    { 4.6,3.4,1.4,0.3,0.0 },
    { 5.0,3.4,1.5,0.2,0.0 },
    { 4.4,2.9,1.4,0.2,0.0 },
    { 4.9,3.1,1.5,0.1,0.0 },
    { 5.4,3.7,1.5,0.2,0.0 },
    { 4.8,3.4,1.6,0.2,0.0 },
    { 4.8,3.0,1.4,0.1,0.0 },
    { 4.3,3.0,1.1,0.1,0.0 },
    { 5.8,4.0,1.2,0.2,0.0 },
    { 5.7,4.4,1.5,0.4,0.0 },
    { 5.4,3.9,1.3,0.4,0.0 },
    { 5.1,3.5,1.4,0.3,0.0 },
    { 5.7,3.8,1.7,0.3,0.0 },
    { 5.1,3.8,1.5,0.3,0.0 },
    { 5.4,3.4,1.7,0.2,0.0 },
    { 5.1,3.7,1.5,0.4,0.0 },
    { 4.6,3.6,1.0,0.2,0.0 },
    { 5.1,3.3,1.7,0.5,0.0 },
    { 4.8,3.4,1.9,0.2,0.0 },
    { 5.0,3.0,1.6,0.2,0.0 },
    { 5.0,3.4,1.6,0.4,0.0 },
    { 5.2,3.5,1.5,0.2,0.0 },
    { 5.2,3.4,1.4,0.2,0.0 },
    { 4.7,3.2,1.6,0.2,0.0 },
    { 4.8,3.1,1.6,0.2,0.0 },
    { 5.4,3.4,1.5,0.4,0.0 },
    { 5.2,4.1,1.5,0.1,0.0 },
    { 5.5,4.2,1.4,0.2,0.0 },
    { 4.9,3.1,1.5,0.1,0.0 },
    { 5.0,3.2,1.2,0.2,0.0 },
    { 5.5,3.5,1.3,0.2,0.0 },
    { 4.9,3.1,1.5,0.1,0.0 },
    { 4.4,3.0,1.3,0.2,0.0 },
    { 5.1,3.4,1.5,0.2,0.0 },
    { 5.0,3.5,1.3,0.3,0.0 },
    { 4.5,2.3,1.3,0.3,0.0 },
    { 4.4,3.2,1.3,0.2,0.0 },
    { 5.0,3.5,1.6,0.6,0.0 },
    { 5.1,3.8,1.9,0.4,0.0 },
    { 4.8,3.0,1.4,0.3,0.0 },
    { 5.1,3.8,1.6,0.2,0.0 },
    { 4.6,3.2,1.4,0.2,0.0 },
    { 5.3,3.7,1.5,0.2,0.0 },
    { 5.0,3.3,1.4,0.2,0.0 },
    { 7.0,3.2,4.7,1.4,1.0 },
    { 6.4,3.2,4.5,1.5,1.0 },
    { 6.9,3.1,4.9,1.5,1.0 },
    { 5.5,2.3,4.0,1.3,1.0 },
    { 6.5,2.8,4.6,1.5,1.0 },
    { 5.7,2.8,4.5,1.3,1.0 },
    { 6.3,3.3,4.7,1.6,1.0 },
    { 4.9,2.4,3.3,1.0,1.0 },
    { 6.6,2.9,4.6,1.3,1.0 },
    { 5.2,2.7,3.9,1.4,1.0 },
    { 5.0,2.0,3.5,1.0,1.0 },
    { 5.9,3.0,4.2,1.5,1.0 },
    { 6.0,2.2,4.0,1.0,1.0 },
    { 6.1,2.9,4.7,1.4,1.0 },
    { 5.6,2.9,3.6,1.3,1.0 },
    { 6.7,3.1,4.4,1.4,1.0 },
    { 5.6,3.0,4.5,1.5,1.0 },
    { 5.8,2.7,4.1,1.0,1.0 },
    { 6.2,2.2,4.5,1.5,1.0 },
    { 5.6,2.5,3.9,1.1,1.0 },
    { 5.9,3.2,4.8,1.8,1.0 },
    { 6.1,2.8,4.0,1.3,1.0 },
    { 6.3,2.5,4.9,1.5,1.0 },
    { 6.1,2.8,4.7,1.2,1.0 },
    { 6.4,2.9,4.3,1.3,1.0 },
    { 6.6,3.0,4.4,1.4,1.0 },
    { 6.8,2.8,4.8,1.4,1.0 },
    { 6.7,3.0,5.0,1.7,1.0 },
    { 6.0,2.9,4.5,1.5,1.0 },
    { 5.7,2.6,3.5,1.0,1.0 },
    { 5.5,2.4,3.8,1.1,1.0 },
    { 5.5,2.4,3.7,1.0,1.0 },
    { 5.8,2.7,3.9,1.2,1.0 },
    { 6.0,2.7,5.1,1.6,1.0 },
    { 5.4,3.0,4.5,1.5,1.0 },
    { 6.0,3.4,4.5,1.6,1.0 },
    { 6.7,3.1,4.7,1.5,1.0 },
    { 6.3,2.3,4.4,1.3,1.0 },
    { 5.6,3.0,4.1,1.3,1.0 },
    { 5.5,2.5,4.0,1.3,1.0 },
    { 5.5,2.6,4.4,1.2,1.0 },
    { 6.1,3.0,4.6,1.4,1.0 },
    { 5.8,2.6,4.0,1.2,1.0 },
    { 5.0,2.3,3.3,1.0,1.0 },
    { 5.6,2.7,4.2,1.3,1.0 },
    { 5.7,3.0,4.2,1.2,1.0 },
    { 5.7,2.9,4.2,1.3,1.0 },
    { 6.2,2.9,4.3,1.3,1.0 },
    { 5.1,2.5,3.0,1.1,1.0 },
    { 5.7,2.8,4.1,1.3,1.0 },
    { 6.3,3.3,6.0,2.5,2.0 },
    { 5.8,2.7,5.1,1.9,2.0 },
    { 7.1,3.0,5.9,2.1,2.0 },
    { 6.3,2.9,5.6,1.8,2.0 },
    { 6.5,3.0,5.8,2.2,2.0 },
    { 7.6,3.0,6.6,2.1,2.0 },
    { 4.9,2.5,4.5,1.7,2.0 },
    { 7.3,2.9,6.3,1.8,2.0 },
    { 6.7,2.5,5.8,1.8,2.0 },
    { 7.2,3.6,6.1,2.5,2.0 },
    { 6.5,3.2,5.1,2.0,2.0 },
    { 6.4,2.7,5.3,1.9,2.0 },
    { 6.8,3.0,5.5,2.1,2.0 },
    { 5.7,2.5,5.0,2.0,2.0 },
    { 5.8,2.8,5.1,2.4,2.0 },
    { 6.4,3.2,5.3,2.3,2.0 },
    { 6.5,3.0,5.5,1.8,2.0 },
    { 7.7,3.8,6.7,2.2,2.0 },
    { 7.7,2.6,6.9,2.3,2.0 },
    { 6.0,2.2,5.0,1.5,2.0 },
    { 6.9,3.2,5.7,2.3,2.0 },
    { 5.6,2.8,4.9,2.0,2.0 },
    { 7.7,2.8,6.7,2.0,2.0 },
    { 6.3,2.7,4.9,1.8,2.0 },
    { 6.7,3.3,5.7,2.1,2.0 },
    { 7.2,3.2,6.0,1.8,2.0 },
    { 6.2,2.8,4.8,1.8,2.0 },
    { 6.1,3.0,4.9,1.8,2.0 },
    { 6.4,2.8,5.6,2.1,2.0 },
    { 7.2,3.0,5.8,1.6,2.0 },
    { 7.4,2.8,6.1,1.9,2.0 },
    { 7.9,3.8,6.4,2.0,2.0 },
    { 6.4,2.8,5.6,2.2,2.0 },
    { 6.3,2.8,5.1,1.5,2.0 },
    { 6.1,2.6,5.6,1.4,2.0 },
    { 7.7,3.0,6.1,2.3,2.0 },
    { 6.3,3.4,5.6,2.4,2.0 },
    { 6.4,3.1,5.5,1.8,2.0 },
    { 6.0,3.0,4.8,1.8,2.0 },
    { 6.9,3.1,5.4,2.1,2.0 },
    { 6.7,3.1,5.6,2.4,2.0 },
    { 6.9,3.1,5.1,2.3,2.0 },
    { 5.8,2.7,5.1,1.9,2.0 },
    { 6.8,3.2,5.9,2.3,2.0 },
    { 6.7,3.3,5.7,2.5,2.0 },
    { 6.7,3.0,5.2,2.3,2.0 },
    { 6.3,2.5,5.0,1.9,2.0 },
    { 6.5,3.0,5.2,2.0,2.0 },
    { 6.2,3.4,5.4,2.3,2.0 },
    { 5.9,3.0,5.1,1.8,2.0 }
  };

  Node DecisionTree; // Nó raíz da árvore de decisão

  trainDecisionTree(&DecisionTree, data, n, attributes, a); // Treinamento da árvore

  printDecisionTree(&DecisionTree); // Exibição da árvore

  printf("\nEfficiency: %.2f\n", evalEfficiency(&DecisionTree, data, n)); // Eficiência da árvore

  return 0;
}



float I(float* prob) {
  // Esta função percorre a lista de probabilidades fornecida calculando a
  // quantidade acumulada de informação necessária para representar o conjunto
  double info = 0;
  double temp;
  for(int i=0; i<3; i++) {
    if(prob[i] > 0.0) {
      temp = (double)(prob[i]);
      info -= temp * log2(temp);
    }
  }
  return (float)(info);
}


float InfoPartition(int partition[2][3], int n) {
  // Esta função se utiliza da relação de partições fornecida para calcular o
  // ganho de informações proporcionado pelo particionamento

  // Inicializa alguns vetores para auxiliar nos cálculos
  int partitionTotal[2] = {0,0}; // Relação do número de dados total em cada partição
  float lessThanPartition[3] = {0.0,0.0,0.0}; // Relação das probabilidades de ocorrência de cada categorias dentre os dados da partição menor que ou igual ao valor de corte
  float biggerThanPartition[3] = {0.0,0.0,0.0}; // Relação das probabilidades de ocorrência de cada categorias dentre os dados da partição mmaior que o valor de corte
  float categoryPartition[3] = {0.0,0.0,0.0}; // Relação das probabilidades de ocorrência de cada categorias dentre os dados de ambas as partiÇões

  for(int i=0; i<3; i++) {
    // Calcula o número total de dados para cada partição somando o número de
    // dados de cada cateroria na partição
    partitionTotal[0] += partition[0][i];
    partitionTotal[1] += partition[1][i];
    // Calcula a probabilidade de ocorrência de cada categoria no conjunto total
    // de dados somando o número de dados de ambas as partições e dividindo pelo
    // número de dados total
    categoryPartition[i] += (( partition[0][i] + partition[1][i] ) != 0) ? ( (float)( partition[0][i] + partition[1][i] ) / (float)(n) ) : 0.0;
  }

  for(int i=0; i<3; i++) {
    // Calcula a probabilidade de ocorrência de cada categoria no conjunto de cada
    // partição dividindo o número de dados de cada categoria pelo total de dados
    // da partição
    lessThanPartition[i] = (partitionTotal[0] != 0) ? ( ( (float)(partition[0][i]) ) / ( (float)(partitionTotal[0]) ) ) : 0.0;
    biggerThanPartition[i] = (partitionTotal[1] != 0) ? ( ( (float)(partition[1][i]) ) / ( (float)(partitionTotal[1]) ) ) : 0.0;
  }

  // Calcula o total de informação necessária para representar o particionamento
  float infoAttr = 0.0;
  infoAttr += ( (float)(partitionTotal[0]) ) / ( (float)(n) ) * I(lessThanPartition);
  infoAttr += ( (float)(partitionTotal[1]) ) / ( (float)(n) ) * I(biggerThanPartition);

  // Calcula o total de informação necessária para representar o conjunto total
  // de dados
  float infoTotal = I(categoryPartition);

  // Calcula o ganho de informação proporcionado pelo particionamento
  float info = infoTotal - infoAttr;
  return info;
}


float getBestPartition(float data[][5], int n, int attr, float& info) {
  // Esta função percorre o conjunto de dados fornecido para identificar qual
  // elemento do conjunto melhor particiona o conjunto em relação ao atributo
  // definido

  // Inicializa uma matriz para conter a relação de categorais para cada partição
  int partition[2][3];

  // Inicializa as variáveis responsáveis por guardar os dados acerca da melhor
  // partição
  float bestPartition = 0;
  float bestPartitionInfo = -1;
  float tempInfo;

  // Percorre o conjunto de dados definindo a cada iteração um dos seus elementos
  // como o valor de corte para a partição
  for(int i=0; i<n; i++) {
    // Zera as relações de categorias das partições
    for(int j=0; j<3; j++) {
      partition[0][j] = 0;
      partition[1][j] = 0;
    }

    // Percorre o conjunto de dados separando-os primeiro por partição e depois
    // por categoria
    for(int j=0; j<n; j++) {
      // Verifica se o valor do atributo do dado é menor ou igual ao valor de
      // corte da iteração atual
      if(data[j][attr] <= data[i][attr]) {
        // Caso menor ou igual o valor de corte, incrementa 1 na categoria do dado
        // na primeira partição
        partition[0][(int)(data[j][4])] += 1;
      } else {
        // Caso maior que o valor de corte, incrementa 1 na categoria do dado
        // na segunda partição
        partition[1][(int)(data[j][4])] += 1;
      }
    }

    // Calcula o ganho de informação proporcionado pela partição usando o valor
    // de corte da iteração atual
    tempInfo = InfoPartition(partition, n);

    // Verifica se o ganho de informação obtido é maior do que o ganho do melhor
    // valor de corte selecionado até então
    if(tempInfo > bestPartitionInfo) {
      // Caso seja maior, o melhor valor de corte até então é substituido pelo
      // valor da iteração atual
      bestPartition = data[i][attr];
      bestPartitionInfo = tempInfo;
    }
  }

  // Atribui o ganho de informação da melhor escolha de corte de partição à variável
  // info fornecida como argumento para a função
  info = bestPartitionInfo;

  // Retorna o melhor valor de corte encontrado
  return bestPartition;
}


void trainDecisionTree(Node* node, float data[][5], int n, int attr[], int a) {
  // Esta função procura treinar os nós de uma árvore de decisão com base em um
  // conjunto de dados

  // Verifica se o conjunto de dados está vazio
  if(n == 0) {
    // Caso esteja vazio, define todos os parâmetros do nó com -1 ou NULL,
    // indicando ser um nó fracassado
    node->attribute = -1;
    node->partition = -1;
    node->category = -1;
    node->lessThan = NULL;
    node->biggerThan = NULL;
    return;
  }

  // Caso não esteja vazio, percorre o conjunto de dados para verificar se todos
  // pertencem à mesma categoria
  bool sameCategory = true;
  for(int i=1; i<n; i++) {
    if(data[0][4] != data[i][4]) {
      sameCategory = false;
    }
  }
  if(sameCategory) {
    // Caso sejam da mesma categorias, atribui a categoria do conjunto ao parâmetro
    // category e define o restante dos parâmetrso do nó com valores padrão
    node->attribute = -1;
    node->partition = -1;
    node->category = data[0][4];
    node->lessThan = NULL;
    node->biggerThan = NULL;
    return;
  }

  // Caso não sejam da mesma categoria, verifica se ainda há atributos para serem
  // utilizados
  if(a == 0) {
    // Caso não existam mais atributos, percorre o conjunto de dados para verificar
    // qual a categoria mais numerosa

    // Percorre o conjunto de dados contando o número de dados em cada categoria
    int categories[3] = {0,0,0};
    for(int i=0; i<n; i++) {
      categories[((int)(data[i][4]))] += 1;
    }

    // Percorre a relação das categorias para averiguar qual é a mais numerosa
    int category = 0;
    for(int i=0; i<3; i++) {
      if(categories[i] > categories[category]) {
        category = i;
      }
    }

    // Atribui a categoria encontrada ao parâmetro category do nó e os demais
    // parâmetros recebem valores padrão
    node->attribute = -1;
    node->partition = -1;
    node->category = category;
    node->lessThan = NULL;
    node->biggerThan = NULL;
    return;
  }

  // Caso ainda existam atributos para serem utilizados, percorre a lista de
  // atributos procurando pelo que melhor particiona os dados, promovendo o maior
  // ganho de informação

  // Inicializa as variáveis responsáveis por guardar os dados acerca do melhor
  // atributo
  int bestAttr = 0;
  float bestAttrInfo = -1;
  float bestPartition;
  float tempPartition;
  float tempPartitionInfo;

  for(int i=0; i<a; i++) {
    // Calcula o melhor valor de corte para o atributo da iteração, assim como o
    // ganho de informação associado
    tempPartition = getBestPartition(data, n, attr[i], tempPartitionInfo);

    // Verifica se o ganho de informação obtido é melhor do que o da melhor opção
    // de atributo até então
    if(tempPartitionInfo > bestAttrInfo) {
      // Caso seja melhor, o atributo da iteração e seu melhor valor de corte
      // passam a ser a melhor opção até então
      bestAttr = attr[i];
      bestAttrInfo = tempPartitionInfo;
      bestPartition = tempPartition;
    }
  }

  // Obtêm o número de linhas necessário para cada partição do conjunto de dados
  int partitionLength[2] = {0,0};
  for(int i=0; i<n; i++) {
    if(data[i][bestAttr] <= bestPartition) {
      partitionLength[0] += 1;
    } else {
      partitionLength[1] += 1;
    }
  }

  // Inicializa as matrizes que devem conter os dados particionados
  int lessThanIdx = 0;
  float lessThan[partitionLength[0]][5];
  int biggerThanIdx = 0;
  float biggerThan[partitionLength[1]][5];

  // Percorre o conjunto de dados separando-os nas partições
  for(int i=0; i<n; i++) {
    if(data[i][bestAttr] <= bestPartition) {
      for(int j=0; j<5; j++) {
        lessThan[lessThanIdx][j] = data[i][j];
      }
      lessThanIdx++;
    } else {
      for(int j=0; j<5; j++) {
        biggerThan[biggerThanIdx][j] = data[i][j];
      }
      biggerThanIdx++;
    }
  }

  // Cria um novo vetor de atributos sem o atribudo utilizado no nó atual
  int newAttr[a-1];
  int i = 0;
  int j = 0;
  while(j < a) {
    if(attr[j] != bestAttr) {
      newAttr[i] = attr[j];
      i++;
    }
    j++;
  }

  // Atribui o atributo escolhido e o seu valor de corte aos parâmetros do nó
  // atual e aloca espaço na memória para a referência dos nós filhos
  node->attribute = bestAttr;
  node->partition = bestPartition;
  node->category = -1;
  node->lessThan = (Node*) malloc(sizeof(Node));
  node->biggerThan = (Node*) malloc(sizeof(Node));

  // Chama a própria função, uma vez para cada nó filho, atribuindo os parâmetros
  // de referência do nó atual, os conjuntos da dados particionados e o novo vector
  // de atributos
  trainDecisionTree(node->lessThan, lessThan, partitionLength[0], newAttr, a-1);
  trainDecisionTree(node->biggerThan, biggerThan, partitionLength[1], newAttr, a-1);
}

void printDecisionTree(Node* node) {
  // Esta função exibe os parâmetros de cada nó, percorrendo a árvore de decisão
  // no estilo pesquisa profunda
  printf("\nNode pointer: %p\nAttribute: %d\nPartition: %.1f\nCategory: %d\nLessThan pointer: %p\nBiggerThan pointer: %p\n",node, node->attribute,node->partition,node->category,node->lessThan,node->biggerThan);
  if(node->lessThan != NULL) {
    printDecisionTree(node->lessThan);
  }
  if(node->biggerThan != NULL) {
    printDecisionTree(node->biggerThan);
  }
}

int evalData(Node* node, float data[4]) {
  // Esta função categoriza o conjunto de atributos fornecido segundo a lógica da
  // árvore de decisão fornecida

  // Realiza uma cópia da referência do nó raíz para uma variável temporária
  Node *temp = (Node*) malloc(sizeof(Node));
  temp = node;

  // A cada iteração, verifica se a variável temporária possui nós filho, indicando
  // que o nó atual não é um nó folha
  int j=0;
  while((temp->lessThan != NULL || temp->biggerThan != NULL) && j < 20) {
    // Verifica se, para o atributo do nó em questão, o valor do atributo de entrada
    // é menor ou igual ao valor de corte do nó
    if(data[temp->attribute] <= temp->partition) {
      // Caso seja, a variável temporária assume o valor do nó filho lessThan
      temp = temp->lessThan;
    } else {
      // Caso não seja, a variável temporária assume o valor do nó filho biggerThan
      temp = temp->biggerThan;
    }
    j++;
  }
  // Com o loop quebrado, ou chegou-se a um nó folha, indicando que uma categoria
  // foi encontrada, ou o limite de iterações foi atingido, indicando uma classificação
  // fracassada

  // Retorna a categoria do último nó atingido antes do loop ser quebrado
  return temp->category;
}

float evalEfficiency(Node* node, float data[][5], int n) {
  // Esta função avalia a eficiência da árvore de decisão fornecida comparando
  // a classificação da árvore com os valores de referência dos dados fornecidos

  float eff = 0.0;
  float tempEval;
  float temp[4];
  for(int i=0; i<n; i++) {
    for(int j=0; j<4; j++) {
      temp[j] = data[i][j];
    }
    tempEval = evalData(node, temp);
    if((float)(tempEval) == data[i][4]) {
      eff += 1.0;
    }
  }
  eff /= (float)(n);
  return eff;
}
