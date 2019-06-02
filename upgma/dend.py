from scipy.cluster.hierarchy import dendrogram, linkage  
from matplotlib import pyplot as plt
import numpy as np

X = np.array(
	[
		[5,3],  
	    [1.2,15],
	    [15.6,12],
	    [24,10.43],
	    [3,34.0],
	    [85,50],
	    [7,17],
	    [60,78],
	    [7,5.55],
	    [10.5,91],
    ])

linked = linkage(X, 'single')
print(linked)

labelList = range(1, 11)

plt.figure(figsize=(10, 7))  
dendrogram(linked,  
            orientation='top',
            labels=labelList,
            distance_sort='descending',
            show_leaf_counts=True)
plt.show()  