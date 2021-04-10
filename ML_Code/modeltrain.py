from tensorflow import keras
from keras.utils import np_utils
from tensorflow.keras.models import Sequential
#from keras.models import Sequential
from tensorflow.keras.layers import Convolution2D,Dense,MaxPool2D,Activation,Dropout,Flatten
from tensorflow.keras.layers import GlobalAveragePooling2D
from tensorflow.keras.optimizers import Adam
from sklearn.model_selection import train_test_split
from keras.layers.normalization import BatchNormalization
import os 
import pandas as pd
import plotly.graph_objs as go
import matplotlib.ticker as ticker
import matplotlib.pyplot as plt
import cv2
import numpy as np
from sklearn.model_selection import train_test_split
from tensorflow.keras.layers import Input, Add, Dense, Activation, ZeroPadding2D, BatchNormalization, Flatten, Conv2D, AveragePooling2D, MaxPooling2D, GlobalMaxPooling2D
import glob
from tensorflow.keras.callbacks import ReduceLROnPlateau
def get_files(directory):        #Function to that fetches all the files from the directory path.
  if not os.path.exists(directory):
    return 0
  count=0
  for current_path,dirs,files in os.walk(directory):
    for dr in dirs:
      count+= len(glob.glob(os.path.join(current_path,dr+"/*")))
  return count
train_dir =r"C:\Users\hp\AppData\Local\Programs\Python\Python36\New folder\Train"  #Training Imagedata Path
test_dir=r"C:\Users\hp\AppData\Local\Programs\Python\Python36\New folder\Valid"    #Validation Imagedata Path
train_samples =get_files(train_dir)
num_classes=len(glob.glob(train_dir+"/*"))
test_samples=get_files(test_dir)
print(num_classes,"Classes")
print(train_samples,"Train images")
print(test_samples,"Test images")
from keras.preprocessing.image import ImageDataGenerator
train_datagen=ImageDataGenerator(rescale=1./255,
                                   shear_range=0.2,
                                   zoom_range=0.2,
                                   horizontal_flip=True)
test_datagen=ImageDataGenerator(rescale=1./255)
img_width,img_height =224,224
input_shape=(img_width,img_height,3)
batch_size =32
train_generator =train_datagen.flow_from_directory(train_dir,
                                target_size=(img_width,img_height),batch_size=batch_size)
test_generator=test_datagen.flow_from_directory(test_dir,shuffle=True,target_size=(img_width,img_height),batch_size=batch_size)
model = Sequential()
model.add(Conv2D(32, (5, 5),input_shape=input_shape,activation='relu',name="Layer1"))
model.add(MaxPooling2D(pool_size=(3, 3),name="Layer2"))
model.add(Conv2D(32, (3, 3),activation='relu',name="Layer3"))
model.add(MaxPooling2D(pool_size=(2, 2),name="Layer4"))
model.add(Conv2D(64, (3, 3),activation='relu',name="Layer5"))
model.add(MaxPooling2D(pool_size=(2, 2),name="Layer6"))   
model.add(Flatten(name="flatten_1"))
model.add(Dense(512,activation='relu'))
model.add(Dropout(0.25))
model.add(Dense(128,activation='relu'))          
model.add(Dense(num_classes,activation='softmax'))
model.summary()
validation_generator = train_datagen.flow_from_directory(
                       test_dir,
                       target_size=(img_height, img_width),
                       batch_size=batch_size)
model.compile(optimizer='adam',
                  loss = 'categorical_crossentropy',
                  metrics=['accuracy'])
history_1 = model.fit(train_generator,                                    
                                  steps_per_epoch=None, 
                                  epochs=10,validation_data=validation_generator,validation_steps=None
                                  ,verbose=1,callbacks=[ReduceLROnPlateau(monitor='val_loss', factor=0.3,patience=3, min_lr=0.000001)],use_multiprocessing=False,
               shuffle=True)
from keras.models import load_model

model.save('cropdiseasev1.h5')


