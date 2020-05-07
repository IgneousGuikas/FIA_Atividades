# Biblioteca de funções matemáticas
import numpy as np

# Biblioteca para plotar gráficos
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

# Importa o conjunto de dados de Iris de Fisher
# 0 - Iris-setosa / 1 - Iris-versicolor / 2 - Iris-virginica
from sklearn import datasets
iris = datasets.load_iris()
X_data = iris.data;
Y_data = iris.target;


# Importa classe de Perceptron
from sklearn.linear_model import Perceptron

# Declara uma instância da classe Perceptron
clf1 = Perceptron(tol=1e-3, random_state=0)
# Ajustar o modelo do Perceptron com o conjunto de dados
# (utilizando apenas os dados das classes setosa e versicolor)
clf1.fit(X_data[:100,:], Y_data[:100])
# Realiza a predição do conjunto de dados a partir do Perceptron ajustado
Y_pred_perc_1 = clf1.predict(X_data[:100,:])

print("\nPerceptron (apenas para setosa e versicolor)")
print(clf1.score(X_data[:100,:], Y_data[:100]))

# Salva um arquivo de imagem com o gráfico da comparação entre as classificações
# reais e o que foi previsto
fig = plt.figure()
ax = fig.add_subplot(111)
ax.plot(Y_data[:100], Y_pred_perc_1, "b.")
ax.set_xlim(-0.5, 2.5)
ax.set_ylim(-0.5, 2.5)
ax.set_xlabel('Real Target')
ax.set_ylabel('Predicted Target')
ax.set_title("Perceptron (setosa e versicolor)")
fig.savefig('Perceptron (Se e Ver).png')


# Declara uma instância da classe Perceptron
clf2 = Perceptron(tol=1e-3, random_state=0)
# Ajustar o modelo do Perceptron com o conjunto de dados
# (utilizando todos os dados do conjunto)
clf2.fit(X_data, Y_data)
# Realiza a predição do conjunto de dados a partir do Perceptron ajustado
Y_pred_perc_2 = clf2.predict(X_data)

print("\nPerceptron (para todas as classes)")
print(clf2.score(X_data, Y_data))

# Salva um arquivo de imagem com o gráfico da comparação entre as classificações
# reais e o que foi previsto
fig = plt.figure()
ax = fig.add_subplot(111)
ax.plot(Y_data, Y_pred_perc_2, "b.")
ax.set_xlim(-0.5, 2.5)
ax.set_ylim(-0.5, 2.5)
ax.set_xlabel('Real Target')
ax.set_ylabel('Predicted Target')
ax.set_title("Perceptron")
fig.savefig('Perceptron.png')

# Importa classe de Multi Layer Perceptron Classifier
from sklearn.neural_network import MLPClassifier

clf3 = MLPClassifier(hidden_layer_sizes=(10,6), max_iter=5000, alpha=0.0001, solver="sgd", verbose=False, random_state=21, tol=1e-5)
clf3.fit(X_data, Y_data)
Y_pred_MLPC = clf3.predict(X_data)

print("\nMulti Layer Perceptron")
print(clf3.score(X_data, Y_data))

# Salva um arquivo de imagem com o gráfico da comparação entre as classificações
# reais e o que foi previsto
fig = plt.figure()
ax = fig.add_subplot(111)
ax.plot(Y_data, Y_pred_MLPC, "b.")
ax.set_xlim(-0.5, 2.5)
ax.set_ylim(-0.5, 2.5)
ax.set_xlabel('Real Target')
ax.set_ylabel('Predicted Target')
ax.set_title("MLPClassifier")
fig.savefig('MLPC.png')
