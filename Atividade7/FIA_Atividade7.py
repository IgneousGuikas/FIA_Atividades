# Biblioteca de funções matemáticas
import numpy as np

# Biblioteca para plotar gráficos
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

# Importa o conjunto de dados de Iris de Fisher
from sklearn import datasets
iris = datasets.load_iris()
X_data = iris.data;
Y_data = iris.target;

# Importa classe de regressão linear
from sklearn.linear_model import LinearRegression

# Declara uma instância da classe de regressão linear
regr = LinearRegression()
# Ajusta o hiper-plano ao conjunto de dados
regr.fit(X_data, Y_data)
# Realiza a predição do conjunto de dados a partir do hiper-plano ajustado
Y_pred_MQ = regr.predict(X_data)

# Calcula a eficiência da classificação
eff = 0
for i in range(0, len(Y_pred_MQ)):
    # Compara o valor real da classificação com o valor arredontado da predição
    if(Y_data[i] == np.round(Y_pred_MQ[i])):
        # Caso sejam iguais, contabiliza-se uma predição bem sucedida
        eff += 1
# Divide pelo total de predições para obtar a proporção de acertos
eff = eff/len(Y_pred_MQ)

# Exibe os resultados obtidos
print("\nMínimos quadrados\n")
print("Intersection: ", regr.intercept_)
print("Coefficients: ", regr.coef_)
print("Efficiency: ", eff)

# Salva um arquivo de imagem com o gráfico da comparação entre as classificações
# reais e o que foi previsto
fig = plt.figure()
ax = fig.add_subplot(111)
ax.plot(Y_data, Y_pred_MQ, "b.")
ax.set_xlim(-0.5, 2.5)
ax.set_ylim(-0.5, 2.5)
ax.set_xlabel('Real Target')
ax.set_ylabel('Predicted Target')
ax.set_title("Mínimos Quadrados")
fig.savefig('MQ.png')

# Importa classe de PCA
from sklearn.decomposition import PCA

# Testa o desempenho da classificação para diferentes quantidades de componentes
# principais
print("\n\nMínimos quadrados com PCA")
for i in range(1, 5):
    # Declara uma instância da classe de PCA, definindo o número de componentes
    # desejadas
    pca = PCA(n_components = i)
    # Ajusta a relação de componentes principais com base no conjunto de dados
    pca.fit(X_data, Y_data)
    # Converte o conjunto de dados para o sistema de coordenadas das componentes
    # principais
    X_data_transf = pca.transform(X_data)

    # Declara uma instância da classe de regressão linear
    regr2 = LinearRegression()
    # Ajusta o hiper-plano ao conjunto de dados
    regr2.fit(X_data_transf, Y_data)
    # Realiza a predição do conjunto de dados a partir do hiper-plano ajustado
    Y_pred_PCA_MQ = regr2.predict(X_data_transf)

    # Calcula a eficiência da classificação
    eff2 = 0
    for j in range(0, len(Y_pred_PCA_MQ)):
        # Compara o valor real da classificação com o valor arredontado da predição
        if(Y_data[j] == np.round(Y_pred_PCA_MQ[j])):
            # Caso sejam iguais, contabiliza-se uma predição bem sucedida
            eff2 += 1
    # Divide pelo total de predições para obtar a proporção de acertos
    eff2 = eff2/len(Y_pred_PCA_MQ)

    # Exibe os resultados obtidos
    print("\nNúmero de componentes: ", i)
    print("Representação dos componentes: ", pca.explained_variance_ratio_)
    print("Intersection: ", regr2.intercept_)
    print("Coefficients: ", regr2.coef_)
    print("Efficiency: ", eff2)

    # Salva um arquivo de imagem com o gráfico da comparação entre as classificações
    # reais e o que foi previsto
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.plot(Y_data, Y_pred_PCA_MQ, "b.")
    ax.set_xlim(-0.5, 2.5)
    ax.set_ylim(-0.5, 2.5)
    ax.set_xlabel('Real Target')
    ax.set_ylabel('Predicted Target')
    ax.set_title("Mínimos Quadrados com PCA")
    fig.savefig('PCA_MQ{}.png'.format(i))


# Importa da classe de LDA
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis

# Deaclara uma instância da classe de LDA
lda = LinearDiscriminantAnalysis()
# Ajusta o modelo com base no conjunto de dados
lda.fit(X_data, Y_data)
# Realiza a predição do conjunto de dados a partir do modelo ajustado
Y_pred_LDA = lda.predict(X_data)

# Calcula a eficiência da classificação
eff3 = 0
for i in range(0, len(Y_pred_LDA)):
    # Compara o valor real da classificação com o valor arredontado da predição
    if(Y_data[i] == np.round(Y_pred_LDA[i])):
        # Caso sejam iguais, contabiliza-se uma predição bem sucedida
        eff3 += 1
# Divide pelo total de predições para obtar a proporção de acertos
eff3 = eff3/len(Y_pred_LDA)

# Exibe os resultados obtidos
print("\n\nLinear Discriminant Analysis\n")
print("Intersection: ", lda.intercept_)
print("Coefficients: \n", lda.coef_)
print("Efficiency: ", eff3)

# Salva um arquivo de imagem com o gráfico da comparação entre as classificações
# reais e o que foi previsto
fig = plt.figure()
ax = fig.add_subplot(111)
ax.plot(Y_data, Y_pred_LDA, "b.")
ax.set_xlim(-0.5, 2.5)
ax.set_ylim(-0.5, 2.5)
ax.set_xlabel('Real Target')
ax.set_ylabel('Predicted Target')
ax.set_title("Linear Discriminant Analysis")
fig.savefig('LDA.png')


# Importa a classe de SVM
from sklearn import svm as SVM

# Declara uma instância da classe de SVM
svm = SVM.SVC(kernel = "rbf", gamma = 'auto')
# Ajusta o modelo com base no conjunto de dados
svm.fit(X_data,Y_data)
# Realiza a predição do conjunto de dados a partir do modelo ajustado
Y_pred_SVM = svm.predict(X_data)

# Calcula a eficiência da classificação
eff4 = 0
for i in range(0, len(Y_pred_SVM)):
    # Compara o valor real da classificação com o valor arredontado da predição
    if(Y_data[i] == np.round(Y_pred_SVM[i])):
        # Caso sejam iguais, contabiliza-se uma predição bem sucedida
        eff4 += 1
# Divide pelo total de predições para obtar a proporção de acertos
eff4 = eff4/len(Y_pred_SVM)

# Exibe os resultados obtidos
print("\n\nSupport Vector Machine\n")
print("Support Vectors:\n", svm.support_vectors_)
print("Efficiency: ", eff4)

# Salva um arquivo de imagem com o gráfico da comparação entre as classificações
# reais e o que foi previsto
fig = plt.figure()
ax = fig.add_subplot(111)
ax.plot(Y_data, Y_pred_SVM, "b.")
ax.set_xlim(-0.5, 2.5)
ax.set_ylim(-0.5, 2.5)
ax.set_xlabel('Real Target')
ax.set_ylabel('Predicted Target')
ax.set_title("Support Vector Machine")
fig.savefig('SVM.png')
