from cvxpy import *
import h5py
import numpy as np 
import matplotlib.pyplot as plt

with h5py.File('toy.hdf5', 'r') as hf:
	# print('List of arrays in this file: \n', hf.keys())
	X_data = hf.get('X')
	y_data = hf.get('y')

	X_data_np = np.array(X_data)
	y_data_np = np.array(y_data)


#print('Shape of the array dataset_X: \n', X_data_np.shape)
print('Shape of the array dataset_y: \n', y_data_np.shape)

X_data_np = X_data_np.transpose()
print('Shape of the array dataset_X: \n', X_data_np.shape)

# Problem data
C1 = 1
C2 = 10

# Where yi = 1
list_1 = []
# Where yi = -1
list_2 = []

# print(X_data_np.shape[1])
n = X_data_np.shape[0]
p = X_data_np.shape[1]
y_n = y_data_np.shape[0]


for i in range(y_n):
	# print(y_data_np[i])
	if y_data_np[i] == 1:
		list_1.append(i)
	elif y_data_np[i] == -1:
		list_2.append(i)

# print(len(list_2))
# print(list_2)

'''
Primal problem solution
'''
#Construct problem
beta = Variable(p)
beta_0 = Variable()
psi = Variable(n)

term1 = 0.5* square(norm(beta,2)) 

term2 = 0
for i in range(len(list_1)):
	term2 += psi[list_1[i]]

term2 = C1 * term2

term3 = 0
for i in range(len(list_2)):
	term3 += psi[list_2[i]]

term3 = C2 * term3

objective = Minimize(term1 + term2 + term3)
# Constraints
constraints = []

# print(beta)
for i in range(n):
	constraints.append(0 <= psi[i])
	val1 = y_data_np[i] * (X_data_np[i]*beta + beta_0)
	val2 = 1 - psi[i]
	constraints.append(val2 <= val1)

# print(objective)

prob = Problem(objective, constraints)

result = prob.solve()

print ('primal result is \n',result)

beta_val = beta.value
a = float(beta_val[0])
# print('a is ', a**2)
b = float(beta_val[1])
beta0_val = float(beta_0.value)
psi_val = psi.value

# For plotting dist in primal vs alpha  of dual
dist = []
dr = a**2 + b**2
dr = dr ** 0.5
# print('dr is ', dr)

def abs(n):
	if n >= 0:
		return n
	elif n < 0:
		return -n

for i in range(n):
	d = float(abs(a*X_data_np[i][0] + b*X_data_np[i][1] + beta0_val)) / dr
	# print(d)
	dist.append(d)

x_range = range(-7,12)
x = np.array(x_range)

def line(a,b,x,beta0):
	return (-a*x -beta0) / b

def left_margin(a,b,x,beta0):
	return (-1-a*x-beta0) / b

def right_margin(a,b,x,beta0):
	return (1-a*x-beta0) / b

y = line(a,b,x,beta0_val)
plt.plot(x,y,color='black')

y_left = left_margin(a,b,x,beta0_val)
plt.plot(x,y_left,color='brown', linestyle='-.')

y_right = right_margin(a,b,x,beta0_val)
plt.plot(x,y_right,color='brown', linestyle='-.')

# To plot the points
points1 = []
points2 = []
points_mis = []

for i in range(n):
	if psi_val[i] > 1:
		points_mis.append(X_data_np[i])

for i in range(n):
	if(y_data_np[i] == 1):
		points1.append(X_data_np[i])
	elif(y_data_np[i] == -1):
		points2.append(X_data_np[i])

points1 = np.array(points1)
points2 = np.array(points2)
points_mis = np.array(points_mis)

plt.scatter(points1[:,0], points1[:,1], color='green')
plt.scatter(points2[:,0], points2[:,1], color='blue')
plt.scatter(points_mis[:,0], points_mis[:,1], color='red')
# plt.scatter(points_mis[:,0], points_mis[:,1], color='red')
plt.title('Cost Sensitive SVM primal with C1=10 C2=1')
plt.show()

'''
Dual problem solution
'''

alpha = Variable(n)

s = (n,n)
t = (1,n)
# diag_y = np.zeros(s)
ones = np.ones(t)

# for i in range(n):
# 	diag_y[i][i] = y_data_np[i]

X_tilde = diag(y_data_np)*X_data_np
X_tilde_t = X_tilde.T

alpha_t = alpha.T

# term1 = -0.5*np.dot(np.dot(np.dot(alpha_t, X_tilde), X_tilde_t), alpha)
# term1 = -0.5*(((alpha_t*X_tilde)*X_tilde_t)*alpha)
temp = X_tilde*X_tilde_t
term1_1 = -0.5*quad_form(alpha, temp)
term2_1 = ones*alpha



objective1 = Maximize(term1_1+term2_1)
# Constraints
y_transpose = y_data_np.T
const1 = y_transpose*alpha

constraints1 = []
constraints1.append(const1 == 0)

for i in range(len(list_1)):
	constraints1.append(0 <= alpha[list_1[i]])
	constraints1.append(alpha[list_1[i]] <= C1)

for i in range(len(list_2)):
	constraints1.append(0 <= alpha[list_2[i]])
	constraints1.append(alpha[list_2[i]] <= C2)


prob1 = Problem(objective1, constraints1)

result1 = prob1.solve()

print ('dual result is \n',result1)
alpha_val = alpha.value
plt.scatter(alpha_val, dist)
plt.title('Signed dist. in primal vs alpha dual for C1=10 C2=1')
plt.show()
# print(dist)
# Error calculation
# print('y data ', y_data_np[i])
# print('x data ',X_data_np[0]*beta_val)
'''
Obtain classification error
'''
classification_error = 0
for i in range(n):
	if(y_data_np[i] > 0):
		if((X_data_np[i]*beta_val + beta0_val) < 0):
			# print('true')
			classification_error += C1
	elif(y_data_np[i] < 0):
		if((X_data_np[i]*beta_val + beta0_val) > 0):
			classification_error += C2

print('classification error ', classification_error)
