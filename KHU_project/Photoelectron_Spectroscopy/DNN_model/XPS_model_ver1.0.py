import tensorflow as tf
import numpy
from tensorflow import keras
from tensorflow.keras import layers
from tensorflow.keras import regularizers

tf.keras.backend.clear_session()

# Stage 1(Denoising)

#Input(2000x1)
inputs=keras.layers.Input(shape=(2000, 1))
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
#Dense 100(Sigmoid)-regularizer(l2)
x=tf.keras.layers.Dense(units=100, activation=keras.activations.sigmoid, activity_regularizer=regularizers.l2(0.01))(x)
#Dense 1(Sigmoid)-regularizer(l2)
st2=tf.keras.layers.Dense(units=1, activation=keras.activations.sigmoid, activity_regularizer=regularizers.l2(0.01))(x)

# Stage 3(Quantification)

#Concatenate
x=tf.keras.layers.Concatenate(axis=1)([st2, st1])
#Dropout(0.2)
x=tf.keras.layers.Dropout(0.2)(x)
#Dense 2000(ReLU)-Cutomregularizer //
x=tf.keras.layers.Dense(units=2000, activation=keras.activations.relu)(x)
#Dense 81(Sigmoid)-Customregularizer //
st3=tf.keras.layers.Dense(units=81, activation=keras.activations.sigmoid)(x)
#Need Normalization //

model=keras.Model(inputs=inputs, outputs=[st2, st3], name='XPS')
model.summary()
