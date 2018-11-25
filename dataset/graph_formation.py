import networkx as nx 
import matplotlib.pyplot as plt


G = nx.Graph()

nodes = []

with open("dataset_btp.txt", 'r') as fobj:
    for line in fobj:
        numbers = [int(num) for num in line.split(",")]
        G.add_edge(int(numbers[1]),int(numbers[2]))
 

L = G.nodes()
for i in L:
	for j in G.neighbors(i):
		print (i,j)

