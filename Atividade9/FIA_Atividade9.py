# Importa as funções da biblioteca do Keras
import tensorflow.keras.applications as apps
from tensorflow.keras.applications.imagenet_utils import preprocess_input
from tensorflow.keras.applications.imagenet_utils import decode_predictions
from tensorflow.keras.preprocessing import image
import numpy as np

# Declara as funções das aplicações
ResNet50 = apps.resnet50.ResNet50
DenseNet201 = apps.DenseNet201
NASNetLarge = apps.NASNetLarge

# Declara cada método com a relação de pesos do pré-treino com a base de images
# Imagenet
model_ResNet50 = ResNet50(weights="imagenet")
model_DenseNet201 = DenseNet201(weights="imagenet")
model_NASNetLarge = NASNetLarge(weights="imagenet")

# Carrega a imagem a ser classificada e a processa para o formato aceito pelos modelos
img_path = 'catapulta.jpg'
# img = image.load_img(img_path, target_size=(224, 224))
img = image.load_img(img_path, target_size=(331, 331)) # para o modelo NASNetLarge
x = image.img_to_array(img)
x = np.expand_dims(x, axis=0)

print('\n\n');
print(img_path);

# Realiza a classificação da imagem e exibe no prompt de comando a lista das 5
# classes mais prováveis segundo o modelo

# Dado algum conflito interno da biblioteca, é mais confiável executar um modelo
# por vez. Comente todos os modelos exceto o que se deseja executar.


# preds_ResNet50 = model_ResNet50.predict(preprocess_input(x))
# print('\nPredicted_ResNet50:')
# print(decode_predictions(preds_ResNet50, top=5)[0])


# preds_DenseNet201 = model_DenseNet201.predict(preprocess_input(x))
# print('\nPredicted_DenseNet201:')
# print(decode_predictions(preds_DenseNet201, top=5)[0])


preds_NASNetLarge = model_NASNetLarge.predict(preprocess_input(x))
print('\nPredicted_NASNetLarge:')
print(decode_predictions(preds_NASNetLarge, top=5)[0])
