# Import required stuff
import numpy as np
import matplotlib.pyplot as plt 
import scipy.io as sio

# Load content
content = sio.loadmat('mnist.mat')
# Define maxIters, step_size, lambda
maxIters = 5000
step_size = 10 ** -4
lamb = 1

'''
Proximal function - Soft Thresholding
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

# Read content X
X = content.get('X')
n = len(X[0])
m = len(X)

# Read content y and f_star
y = content.get('y')
f_star = content.get('f_star')

# Initialize Z,Y and beta
Z = [[0 for i in range(m)] for j in range(m)]
beta = np.random.randn(1000, 10)
Y = [[0 for i in range(10)] for j in range(m)]

# Compute Y and Z
for i in range(0, m):
	t = y[0][i]
	Y[i][t] = 1
	drsum = 0
	for k in range(0, 10):
		drsum = drsum + np.exp(np.dot(X[i], beta[:,k]))

	Z[i][i] = 1 / float(drsum) 

# Initial gradB value
gradB = np.dot(np.transpose(X), (np.dot(Z, np.exp(np.dot(X, beta))) - Y))

# To store f at every iteration.
gvalList = []
accgvalList = []
# To store f - f* at each iteration
modgvalList = []
modaccgvalList = []
# To store iteration count
iterList = []

# Iteration starts
for iters in range(0, maxIters):
	print('iteration no ' + str(iters))
	tmp = step_size * gradB
	beta = prox((beta - tmp), lamb)

	for i in range(0, m):
		drsum = 0
		for k in range(0, 10):
			drsum = drsum + np.exp(np.dot(X[i], beta[:,k]))

		Z[i][i] = 1 / float(drsum) 
		

	gradB = np.dot(np.transpose(X), (np.dot(Z, np.exp(np.dot(X, beta))) - Y))
	## Find g value for every beta or for every iteration		
	gval = 0
	for i in range(0, m):
		nrsum = 0
		
		for k in range(0, 10):
			nrsum = nrsum + np.exp(np.dot(X[i], beta[:,k]))

		temp = np.log(nrsum) - np.dot(X[i], beta[:, y[0][i]]) 
		gval = gval + temp

	betaabs = 0
	for k in range(0, 10):
		betaabs = betaabs + np.linalg.norm(beta[:,k], ord=1)
	
	# Appending into required lists
	gval  = gval +  lamb * betaabs
	iterList.append(iters)
	gvalList.append(gval)	
	print(gval - f_star[0][0])
  	modgvalList.append(gval - f_star[0][0])

'''
Acc. Proximal Gradient Descent
'''
### Implementation Accelereated proximal gradient descent
beta0 = np.random.randn(1000, 10)
beta_0 = beta0
# Compute Z value
Z = [[0 for i in range(m)] for j in range(m)]
for i in range(0, m):
	drsum = 0
	for k in range(0, 10):
		drsum = drsum + np.exp(np.dot(X[i], beta0[:,k]))

	Z[i][i] = 1 / float(drsum) 
# Start iterations
for iters in range(1, maxIters+1):
	print(' acc. iteration no ' + str(iters))
	proxDiff = (beta0 - beta_0)
	cons = (iters-2) / float(iters + 1)
	modproxDiff = [x * cons for x in proxDiff]
	
	v = beta0 + modproxDiff 
	gradV = np.dot(np.transpose(X), (np.dot(Z, np.exp(np.dot(X, v))) - Y))
	Vtmp = step_size * gradV
	beta = prox((v - Vtmp), lamb)
	
	# Recompute Z based on new beta
	for i in range(0, m):
		drsum = 0
		for k in range(0, 10):
			drsum = drsum + np.exp(np.dot(X[i], beta[:,k]))

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

	# Update beta_0 and beta0 for the next iteration
	beta_0 = beta0
	beta0 = beta

	accgvalList.append(agval)	
	print(agval - f_star[0][0])
	modaccgvalList.append(agval - f_star[0][0])


# plt.subplot(211)
#Plotting - f-f* against iterations
logmodgvalList = np.log(modgvalList)
plt.plot(iterList, logmodgvalList)
plt.xlabel(' iterations ')
plt.ylabel(' proximal gradient ')
plt.title(' prox gradient vs iterations ')

plt.tight_layout()
plt.show()

# logmodaccgvalList = np.log(modaccgvalList)
# plt.plot(iterList, logmodaccgvalList)
# plt.xlabel(' iterations ')
# plt.ylabel(' acc. proximal gradient ')
# plt.title(' acc. prox gradient vs iterations ')

# plt.tight_layout()
# plt.show()
