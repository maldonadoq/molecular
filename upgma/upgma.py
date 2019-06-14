from scipy.cluster.hierarchy import dendrogram
from matplotlib import pyplot as plt

upgma_labels = ["YGR149W","YIR038C","YJL155C","YLR297W","YLR423C","YOL083W","YOR049C","YPL087W","YBR052C","YKL141W","YLR038c","YLR149C","YER006w","YER025w","YER036c","YDR450W","YDR471W","YEL026w","YGL078C","YJR071W"]
upgma_values = [
		[15, 16, 0.122372, 2],
		[0, 7, 0.163707, 2],
		[12, 13, 0.166208, 2],
		[20, 19, 0.215278, 3],
		[2, 4, 0.240676, 2],
		[24, 6, 0.265945, 3],
		[5, 9, 0.308950, 2],
		[1, 25, 0.350087, 4],
		[22, 17, 0.358197, 3],
		[26, 10, 0.372319, 3],
		[21, 27, 0.380415, 6],
		[28, 18, 0.430180, 4],
		[31, 23, 0.462492, 7],
		[29, 11, 0.506304, 4],
		[32, 14, 0.524975, 8],
		[30, 8, 0.533535, 7],
		[35, 3, 0.590302, 8],
		[36, 33, 0.747921, 12],
		[37, 34, 2.569094, 20],
	]


plt.figure(figsize=(7, 5), num='Dendogram')
dendrogram(upgma_values,
			orientation='left',
			labels=upgma_labels,
			leaf_font_size=6,
			distance_sort='descending',
			show_leaf_counts=True)
plt.show()
