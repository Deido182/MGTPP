import numpy
from sklearn.linear_model import LinearRegression
from time import time
from math import log
from math import e
import os
dir_path = os.path.dirname(os.path.realpath(__file__))
import sys
sys.path.insert(1, dir_path + '/../../TreeBuilder/')
import TreeBuilder
sys.path.insert(1, dir_path + '/../../CommonUtils/')
import CorrectnessChecker
import CheckUtils
	
ROOT = 0
As, Ad = [], []
bs, bd = [], []

SCRIPTS = dir_path + "/../Main -std"
SCRIPTD = dir_path + "/../Main -diam"
TRIVIAL = dir_path + "/../TrivialSolver/TrivialSolver"
INPUT = dir_path + "/input"
OUTPUTS = dir_path + "/outputs"
OUTPUTD = dir_path + "/outputd"

try:
	assert CorrectnessChecker.check(SCRIPTS, TRIVIAL)
	assert CorrectnessChecker.check(SCRIPTD, TRIVIAL)

	for n in range(70, 161, 15):
		for h in range(1, n, 20):
			for p in range(2, n, 10):
				for eps in range(1, n // p, 10):
					w, T = TreeBuilder.weightedTree(n, h)
					assert CheckUtils.height(ROOT, T) == h
					CheckUtils.writeInstTo(INPUT, n, w, T, p, eps)
					
					print("n:", n, ", h:", h, ", p:", p, ", eps:", eps)
					# The time is independent of epsilon
					start = time()
					os.system(SCRIPTS + " < " + INPUT + " > " + OUTPUTS)
					t = time() - start
					print("   std: ", t, "s")
					# t = n^y * p^k => log(t) = ylog(n) + klog(p)
					As.append([log(n), log(p)])
					bs.append(log(t))
					CheckUtils.checkConsistency(OUTPUTS, n, w, T, p, eps, max)
					
					start = time()
					os.system(SCRIPTD + " < " + INPUT + " > " + OUTPUTD)
					t = time() - start
					print("   diam:", t, "s")
					# t = n^y * min(p, h)^z * p^k => log(t) = ylog(n) + zlog(min(p, h)) + klog(p)
					Ad.append([log(n), log(min(p, h)), log(p)])
					bd.append(log(t))
					CheckUtils.checkConsistency(OUTPUTD, n, w, T, p, eps, max)
					
					with open(OUTPUTS, 'r') as fs:
						with open(OUTPUTD, 'r') as fd:
							assert fs.readline() == fd.readline() # Same opt (or both unfeasible possibly)
					
					print()
		
	coeffs = LinearRegression().fit(numpy.array(As), numpy.array(bs)).coef_
	coeffd = LinearRegression().fit(numpy.array(Ad), numpy.array(bd)).coef_
		
	print("std: ", " n^(", coeffs[0], ") p^(", coeffs[1], ")")
	print("diam:", " n^(", coeffd[0], ") min(p, h)^(", coeffd[1], ") p^(", coeffd[2], ")")

finally:
	os.system("rm " + INPUT + " " + OUTPUTS + " "  + OUTPUTD)
	
