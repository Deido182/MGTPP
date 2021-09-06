
def writeInstTo(file_name, n, w, T, p, eps):
	with open(file_name, 'w') as f:
		f.write(str(n) + "\n")
		for wi in w:
			f.write(str(wi) + " ")
		f.write("\n")
		for i in range(0, n):
			for j in T[i]:
				f.write(str(i) + " " + str(j) + "\n")
		f.write(str(p) + " " + str(eps) + "\n")

def height(curr, T):
	h = 0
	for next in T[curr]:
		h = max(h, 1 + height(next, T))
	return h
	
def gap(w, comp):
	weights = [w[j] for j in comp]
	return max(weights) - min(weights)	

def checkConsistency(sol_file, n, w, T, p, eps, fun):
	with open(sol_file, 'r') as f:
		first = f.readline()
		if first == "The problem is unfeasible\n":
			return
		opt, obt = int(first), 0
		CC = list(map(int, f.readline().split()))
		for i in range(1, p + 1):
			comp = [j for j in range(0, n) if CC[j] == i]
			obt = fun(obt, gap(w, comp))
			assert len(comp) >= eps
			assert len(list(filter(lambda j: [z for z in comp if j in T[z]] == [], comp))) == 1
		assert obt == opt
