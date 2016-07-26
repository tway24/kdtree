from sklearn.neighbors import NearestNeighbors
import numpy as np
from numpy import genfromtxt
import pandas as pd


pointData = genfromtxt('sample_data.csv', delimiter=',')
nbrs = NearestNeighbors(n_neighbors=1, algorithm='brute').fit(pointData)
queryData = genfromtxt('query_data.csv', delimiter=',')
distances, indices = nbrs.kneighbors(queryData)
columns = ['index','distance']
dtype = [('index','int32'), ('distance','float32')]
result = np.append(indices, distances, axis=1)
result.astype(dtype,copy=False)
#df = pd.DataFrame(result, columns=columns)
df = pd.DataFrame(indices, columns = ['index'])
df.to_csv("python_output.csv", index=False)


