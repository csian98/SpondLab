import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import os,sys,re
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
from tensorflow.keras import regularizers
from tensorflow.keras import losses
#from keras.utils.vis_utils import plot_model
from tensorflow.keras.layers import Attention, MultiHeadAttention
#from keras_multi_head import MultiHead

tf.keras.backend.clear_session()
tf.random.set_seed(17)
np.random.seed(17)

class Info:
    INPUT_SZ = 2048
    SPECIES = [
    "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P",
    "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni",
    "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr",
    "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te",
    "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Sm", "Eu", "Gd", "Tb",
    "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir",
    "Pt", "Au", "Hg", "Tl", "Pb", "Bi"
    ]
    NSPECIES=None
    T=None

Info.NSPECIES = len(Info.SPECIES)
print(Info.NSPECIES)
apath = "./T.txt"
Info.T = pd.read_csv(apath, header=None)[0]
print(len(Info.T))
if False:
    for i in range(len(Info.T)):
        print(Info.SPECIES[i],Info.T[i])


## Functions
def get_dropout(input_tensor, p=0.1, mc=False):
    if mc:
        return tf.keras.layers.Dropout(p)(input_tensor, training=True)
    else:
        return tf.keras.layers.Dropout(p)(input_tensor)
        
# Stage 1(Denoising)
inputs = keras.layers.Input(shape=(Info.INPUT_SZ, 1))
c1 = keras.layers.Conv1D(filters=1,
                         kernel_size=11,
                         strides=1,
                         padding='same',
                         activation=keras.activations.relu,
                         use_bias=False)(inputs)
c2 = keras.layers.Conv1D(filters=1,
                         kernel_size=21,
                         strides=1,
                         padding='same',
                         activation=keras.activations.relu,
                         use_bias=False)(inputs)
c3 = keras.layers.Conv1D(filters=1,
                         kernel_size=41,
                         strides=1,
                         padding='same',
                         activation=keras.activations.relu,
                         use_bias=False)(inputs)
c10 = keras.layers.Conv1D(filters=1,
                          kernel_size=9,
                          strides=1,
                          padding='same',
                          activation=keras.activations.relu,
                          use_bias=False)(inputs)
c20 = keras.layers.Conv1D(filters=1,
                          kernel_size=7,
                          strides=1,
                          padding='same',
                          activation=keras.activations.relu,
                          use_bias=False)(inputs)
if True:
    layer = MultiHeadAttention(num_heads=4, key_dim=64)
    target = tf.keras.Input(shape=[Info.INPUT_SZ, 1])
    source = tf.keras.Input(shape=[Info.INPUT_SZ, 1])
    output_tensor, weights = layer(target, source,
                                   return_attention_scores=True)
    c30 = layer(inputs, inputs)
    x = tf.keras.layers.Concatenate(axis=2)([c1, c2, c3, c10, c20, c30])
else:
    # Concatenate(axis=2)
    x = tf.keras.layers.Concatenate(axis=2)([c1, c2, c3, c10, c20])
print(x.shape)
# Conv1D layers with 4 filters
x = tf.keras.layers.Conv1D(filters=4,
                           kernel_size=5,
                           strides=1,
                           padding='same',
                           activation=keras.activations.relu,
                           use_bias=False)(x)
x = tf.keras.layers.MaxPooling1D(pool_size=4, strides=None)(x)
st1 = tf.keras.layers.Flatten()(x)
# Stage 2(Contamination)
x = get_dropout(st1, p=0.1, mc=True)
x = tf.keras.layers.Dense(units=100, activation=keras.activations.sigmoid,
                          )(x)
x = get_dropout(x, p=0.1, mc=True)
x = tf.keras.layers.Dense(units=100, activation=keras.activations.sigmoid,
                          )(x)
st2 = tf.keras.layers.Dense(units=1,
                            activation=keras.activations.sigmoid,
                            # kernel_regularizer=regularizers.L1L2( l1=1e-5, l2=1e-4), bias_regularizer=regularizers.L2(1e-4),
                            name="conta")(x)
# Stage 3(Quantification)
x = tf.keras.layers.Concatenate(axis=1)([st2, st1])
x = get_dropout(x, p=0.1, mc=True)
# Dense 2048(ReLU)-Cutomregularizer
x = tf.keras.layers.Dense(units=Info.INPUT_SZ, activation=keras.activations.relu
                          )(x)
x = get_dropout(x, p=0.1, mc=True)
x = tf.keras.layers.Dense(units=Info.INPUT_SZ, activation=keras.activations.relu
                          )(x)
# Dense NSPECIES(Sigmoid)-Customregularizer
st3 = tf.keras.layers.Dense(units=Info.NSPECIES,
                            activation=keras.activations.softmax
                            )(x)
model = keras.Model(inputs=inputs, outputs=[st2, st3], name='XPS_CSIAN')
model.summary()
