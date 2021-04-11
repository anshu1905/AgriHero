classes=['Apple___Apple_scab', 'Apple___Black_rot', 'Apple___Cedar_apple_rust', 'Apple___healthy', 'Corn_(maize)___Cercospora_leaf_spot Gray_leaf_spot', 'Corn_(maize)___Common_rust_', 'Corn_(maize)___Northern_Leaf_Blight', 'Corn_(maize)___healthy', 'Potato___Early_blight', 'Potato___Late_blight', 'Potato___healthy', 'Tomato___Bacterial_spot', 'Tomato___Early_blight', 'Tomato___Late_blight', 'Tomato___Leaf_Mold', 'Tomato___Septoria_leaf_spot', 'Tomato___Spider_mites Two-spotted_spider_mite', 'Tomato___Target_Spot', 'Tomato___Tomato_Yellow_Leaf_Curl_Virus', 'Tomato___Tomato_mosaic_virus', 'Tomato___healthy']
print(classes) #Printing classes of the disease taken from the dataset
from tensorflow import keras
from keras.optimizers import Adam
import os 
import cv2
import numpy as np
from sklearn.model_selection import train_test_split
from keras.models import load_model
import cv2

model=keras.models.load_model("cropdiseasev1.h5")
print("Model Succefully loaded")
import matplotlib.pyplot as plt
from keras.preprocessing import image
videoCaptureObject = cv2.VideoCapture(0)
result = True
while(result):
     ret,frame = videoCaptureObject.read()
     cv2.imwrite("leaftest.jpg",frame)
     result = False
    
videoCaptureObject.release()
cv2.destroyAllWindows()

def prepare(img_path):
    img = image.load_img(img_path, target_size=(224,224))
    x = image.img_to_array(img)
    x = x/255
    return np.expand_dims(x, axis=0)

result = model.predict([prepare(r'C:\Users\hp\AppData\Local\Programs\Python\Python36\leaftest.jpg')])
d=image.load_img(r'C:\Users\hp\AppData\Local\Programs\Python\Python36\leaftest.jpg')
plt.imshow(d)

import numpy as np
classresult=np.argmax(result,axis=-1)
print("The Disease of the crop detected is: ")
print(classes[classresult[0]])

