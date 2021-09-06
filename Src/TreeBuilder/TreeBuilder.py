import random
import numpy

def tree(n, h):
	T = [[] for i in range(0, n)]
	for i in range(1, h + 1):
		T[i - 1].append(i)
	done = h + 1
	while done < n:
		i = random.randint(0, h - 1)
		hi = random.randint(0, min(n - done - 1, h - i - 1))
		ni = random.randint(0, n - done - (hi + 1)) + (hi + 1)
		if hi == 0:
			for j in range(0, ni):
				T[i].append(done + j)
		else:
			Ti = tree(ni, hi)
			for j in range(0, ni):
				for z in Ti[j]:
					T[done + j].append(done + z)
			T[i].append(done)
		done += ni
	return T
	
def weightedTree(n, h):
	return (numpy.random.permutation(n), tree(n, h))
