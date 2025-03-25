#DeepNeural network for x-ray photoelectron spectroscopy data analysis

import tensorflow as tf
import numpy as np
from tensorflow import keras
from tensorflow.keras import layers
from tensorflow.keras import regularizers
from tensorflow.keras import losses

tf.keras.backend.clear_session()
tf.random.set_seed(17)
np.random.seed(17)

# ATOMS NUMBER
ATOMS=80
# INPUT SIZE
INPUT_SZ=2048

def HighPassLoss(y_true, y_pred):
  # High-pass Filters
  y_true=tf.cast(y_true, tf.float64)
  y_pred=tf.cast(y_pred, tf.float64)
  y=tf.math.multiply(tf.pow(y_true, 2), tf.pow(y_pred-y_true, 2))
  loss=tf.reduce_sum(y, 0)
  return loss

# Stage 1(Denoising)

#Input(2000x1)
inputs=keras.layers.Input(shape=(INPUT_SZ, 1))
#Conv1D layers(x3)
c1=keras.layers.Conv1D(filters=1, kernel_size=11, strides=1, padding='same', activation=keras.activations.relu, use_bias=False)(inputs)
c2=keras.layers.Conv1D(filters=1, kernel_size=21, strides=1, padding='same', activation=keras.activations.relu, use_bias=False)(inputs)
c3=keras.layers.Conv1D(filters=1, kernel_size=41, strides=1, padding='same', activation=keras.activations.relu, use_bias=False)(inputs)
#Concatenate(axis=2)
x=tf.keras.layers.Concatenate(axis=2)([c1, c2, c3])
#Conv1D layers with 4 filters
x=tf.keras.layers.Conv1D(filters=4, kernel_size=5, strides=1, padding='same', activation=keras.activations.relu, use_bias=False)(x)
#AvgPooling1D(size=4)
x=tf.keras.layers.AveragePooling1D(pool_size=4, strides=None)(x)
#Flatten(2000)
st1=tf.keras.layers.Flatten()(x)

# Stage 2(Contamination)

#Dropout(0.2)
x=tf.keras.layers.Dropout(0.2)(st1)
#Dense 100(Sigmoid)
x=tf.keras.layers.Dense(units=100, activation=keras.activations.sigmoid)(x)
#Dense 1(Sigmoid)
st2=tf.keras.layers.Dense(units=1, activation=keras.activations.sigmoid, name="conta")(x)

# Stage 3(Quantification)

#Concatenate
x=tf.keras.layers.Concatenate(axis=1)([st2, st1])
#Dropout(0.2)
x=tf.keras.layers.Dropout(0.2)(x)
#Dense 2000(ReLU)-Cutomregularizer //
x=tf.keras.layers.Dense(units=INPUT_SZ, activation=keras.activations.relu)(x)
#Dense ATOMS(Sigmoid)-Customregularizer //
x=tf.keras.layers.Dense(units=ATOMS, activation=keras.activations.sigmoid)(x)
st3=tf.keras.layers.Lambda(lambda x: x/tf.reduce_sum(x, 1), name="ratio")(x)

model=keras.Model(inputs=inputs, outputs=[st2, st3], name='XPS_CSIAN')
# Contamination-L2_loss, Quantification-HighPassLoss
model.compile(optimizer=tf.keras.optimizers.Adam(learning_rate=1e-4), loss=[losses.MeanSquaredError(), HighPassLoss])
