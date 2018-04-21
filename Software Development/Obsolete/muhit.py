import numpy as np
import pandas as pd

from sklearn.model_selection import train_test_split

read_mat = pd.read_csv("bank-additional-full.csv", delimiter=';')
y = np.array(read_mat.y)
y[y == "no"] = -1
y[y == "yes"] = 1

features = read_mat.drop('y', axis=1)
feature_weight = np.ones_like(y)/np.shape(features)[0]
print(np.shape(features)[0])

sample = np.random.choice(y, size=(np.shape(y)), p=feature_weight)

print(sum(feature_weight))
#print(sample)