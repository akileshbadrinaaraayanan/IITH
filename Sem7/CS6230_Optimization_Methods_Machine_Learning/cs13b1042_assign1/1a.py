from cvxpy import *
import numpy

# Reading y.txt - The path should be changed accordingly before running.
with open('/home/akilesh/Desktop/Akilesh_opt/y.txt') as f:
    y = []
    for line in f:
        line = line.split() # to deal with blank 
        if line:            # lines (ie skip them)
            line = [float(i) for i in line]
            y.append(line)


# Reading beta.txt - The path should be changed accordingly before running.
with open('/home/akilesh/Desktop/Akilesh_opt/beta0.txt') as f:
    beta = []
    for line in f:
        line = line.split() # to deal with blank 
        if line:            # lines (ie skip them)
            line = [float(i) for i in line]
            beta.append(line)



# In this problem : y and beta are given, so only variable is lambda.
# Calculate the first term of the 1d fused lasso problem
ybsum = 0

for i in range(0, 100):
	ybsum = ybsum + (y[i][0] - beta[i][0]) ** 2	

ybsum = ybsum/2

# Calculate the second term of the 1d fused lasso problem

betasum = 0
for j in range(0, 99):
	betasum = betasum + abs(beta[j+1][0] - beta[j][0]).value


# l (lambda) is the only variable in this problem
l = Variable()

# Error fn. 
error = ybsum + l*betasum

# Objective is to minimize the error.
obj = Minimize(error)

# The only constraint in this optimization problem is lambda > 0.
prob = Problem(obj, [l > 0])
prob.solve()

# This prints whether the formulated problem is optimal / infeasible / unbounded and gives corresponding value.
print prob.status
print prob.value


