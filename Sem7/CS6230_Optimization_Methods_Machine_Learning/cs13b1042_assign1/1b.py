# Import required stuff
from cvxpy import *
import numpy as np

# In this problem only y.txt is given. Read the values from text file.
# Please note that the path has to be changed accordingly before running.
with open('/home/akilesh/Desktop/Akilesh_opt/y.txt') as f:
    y = []
    for line in f:
        line = line.split() # to deal with blank 
        if line:            # lines (ie skip them)
            line = [float(i) for i in line]
            y.append(line)

# beta is a variable in this problem, while l(lambda) is given to be 1. 
beta = Variable(1, 100)

# The first term of the 1d fused lasso problem
error = sum_squares(y - beta)
error = error/2

# tv is a python cvx fn. which takes absolute value of diff. of consecutive terms.
obj = Minimize(error + tv(beta))
prob = Problem(obj)
prob.solve()

# Prints the status, value
print prob.status
print prob.value
print beta.value
