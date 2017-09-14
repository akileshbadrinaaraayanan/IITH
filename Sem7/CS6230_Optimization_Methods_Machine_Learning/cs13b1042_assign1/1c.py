# Import required stuff
from cvxpy import *
import numpy as np
import matplotlib.pyplot as plt 

# In this problem, we need both y.txt and beta.txt, so read them from input files.
# Please note that the path has to be changed accordingly before running.
with open('/home/akilesh/Desktop/Akilesh_opt/y.txt') as f:
    y = []
    for line in f:
        line = line.split() # to deal with blank 
        if line:            # lines (ie skip them)
            line = [float(i) for i in line]
            y.append(line)

with open('/home/akilesh/Desktop/Akilesh_opt/beta0.txt') as f:
    betaorg = []
    for line in f:
        line = line.split() # to deal with blank 
        if line:            # lines (ie skip them)
            line = [float(i) for i in line]
            betaorg.append(line)

# Define 100 logarithmically spaced values from 10^1 to 10^-2.
lvals = np.logspace(-2, 1, 100)
mseList = []	# List for storing MSEs as we vary the lambda.
changepointsList = []	# List for storing changepoint as we vary the lambda.
lvalsList = []	# List for storing beta computed as we vary the lambda.

for val in lvals:
	# As in prev problem, beta is the variable.
	beta = Variable(1, 100)
	error = sum_squares(y - beta)
	error = error/2

	obj = Minimize(error + val*tv(beta))
	prob = Problem(obj)
	prob.solve()

# to compute mse for a particular lambda.
# Threshold is defined as 10^-8.
	mse = 0
	thresh = 0.00000001
	changepoints = 0

	# Iterate over points
	for i in range(0, 100):
		req = (beta[i].value - betaorg[i][0])
		a = abs(req).value
		# Compute MSE.
		mse = mse +  req ** 2
		# If abs. value is greater than thresh
		if(a > thresh):
			changepoints = changepoints + 1

	# Appending stuff to corresponding list
	mse = mse / 100
	mseList.append(mse)	
	changepointsList.append(changepoints)
	lvalsList.append(val)

#print(mseList)
# For plotting - mse vs lambda. 
plt.subplot(211)
plt.plot(lvalsList, mseList)
plt.xlabel('lambda ')
plt.ylabel('mse')
plt.title(' mse vs lambda ')

# For plotting - changepoints vs lambda.
plt.subplot(212)
plt.plot(lvalsList, changepointsList)
plt.xlabel('lambda ')
plt.ylabel(' changepoints')
plt.title(' changepoints vs lambda ')

plt.tight_layout()
plt.show()