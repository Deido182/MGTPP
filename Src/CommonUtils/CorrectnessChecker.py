import CheckUtils
import os
dir_path = os.path.dirname(os.path.realpath(__file__))
import sys
sys.path.insert(1, dir_path + '/../TreeBuilder/')
import TreeBuilder

def check(script1, script2):
	INPUT = dir_path + "/input"
	OUTPUT1 = dir_path + "/output1"
	OUTPUT2 = dir_path + "/output2"
	try:
		ok = True
		for n in range(1, 12):
			for h in range(1, n):
				for p in range(1, n):
					for eps in range(1, n + 1):
						w, T = TreeBuilder.weightedTree(n, h)
						CheckUtils.writeInstTo(INPUT, n, w, T, p, eps)
						os.system(script1 + " < " + INPUT + " > " + OUTPUT1)
						os.system(script2 + " < " + INPUT + " > " + OUTPUT2)
						with open(OUTPUT1, 'r') as o1:
							with open(OUTPUT2, 'r') as o2:
								ok &= o1.readline() == o2.readline()
		return ok
	
	finally:
		os.system("rm " + INPUT + " " + OUTPUT1 + " "  + OUTPUT2)
		
