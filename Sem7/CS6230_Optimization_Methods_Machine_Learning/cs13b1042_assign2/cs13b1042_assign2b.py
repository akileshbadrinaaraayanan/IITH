# Import required stuff
import numpy as np
import matplotlib.pyplot as plt 
import scipy.io as sio

# Load data and define other constants
content = sio.loadmat('mnist.mat')
maxIters = 50
step_size = 10 ** -4
lamb = 1


'''
Proximal function -> Soft Thresholding operator
'''
def prox(param, lamb):
	for i in range(0, len(param)):
		for j in range(0, len(param[i])):
			if param[i][j] > lamb:
				param[i][j] = param[i][j] - lamb
			elif param[i][j] < -lamb:
				param[i][j] = param[i][j] + lamb
			elif param[i][j] >= -lamb and param[i][j] <= lamb:
				param[i][j] = 0

	return param

# Test data
X = content.get('Xtest')
n = len(X[0])
m = len(X)

y = content.get('ytest')
f_star = content.get('f_star')

# Compute Y matrix
Y = [[0 for i in range(10)] for j in range(m)]

for i in range(0, m):
	t = y[0][i]
	Y[i][t] = 1

# To store f - f* at each iteration
# accgvalList = []
# To store iteration count
iterList = []
#Final value
finVal = 0
#Final value list
# finValList = []
modfinValList = []
# Lambda values
# Define 20 logarithmically spaced values from 10^1 to 10^-2.
lambvals = np.logspace(-2, 1, 20)

'''
Acc. proximal descent implementation.
Same implementation as in prev. prob
'''
### Accelereated proximal gradient descent
for lamb in lambvals:
	print('lamda value is ' + str(lamb))
	beta0 = np.random.randn(1000, 10)
	beta_0 = beta0
	Z = [[0 for i in range(m)] for j in range(m)]
	for i in range(0, m):
		drsum = 0
		for k in range(0, 10):
			drsum = drsum + np.exp(np.dot(X[i], beta0[:,k]))

		Z[i][i] = 1 / float(drsum) 

	for iters in range(1, maxIters+1):
		print(' acc. iteration no ' + str(iters))
		proxDiff = (beta0 - beta_0)
		cons = (iters-2) / float(iters + 1)
		modproxDiff = [x * cons for x in proxDiff]
		
		v = beta0 + modproxDiff 
		gradV = np.dot(np.transpose(X), (np.dot(Z, np.exp(np.dot(X, v))) - Y))
		Vtmp = step_size * gradV
		beta = prox((v - Vtmp), lamb)
		
		for i in range(0, m):
			drsum = 0
			for k in range(0, 10):
				drsum = drsum + np.exp(np.dot(X[i], beta[:,k]))

			# print(drsum)
			Z[i][i] = 1 / float(drsum) 

		## Find g value for every beta or for every iteration		
		agval = 0
		for i in range(0, m):
			nrsum = 0
			
			for k in range(0, 10):
				nrsum = nrsum + np.exp(np.dot(X[i], beta[:,k]))

			temp = np.log(nrsum) - np.dot(X[i], beta[:, y[0][i]]) 
			agval = agval + temp

		betaabs = 0
		for k in range(0, 10):
			betaabs = betaabs + np.linalg.norm(beta[:,k], ord=1)
		
		agval  = agval +  lamb * betaabs

		beta_0 = beta0
		beta0 = beta

		# accgvalList.append(agval)
		finVal = agval - f_star[0][0] 	
		# print('value is ' + str(finVal))
	# finValList.append(finVal)
	# Measuring the error rate
	modfinValList.append(finVal - f_star[0][0])


print(modfinValList)
