# Import required stuff
import numpy as np
import scipy.io as sio
import operator
# from collections import defaultdict
import scipy.misc as misc
from scipy.misc import imsave

# Load content and pixel values from mnist.mat and mnist_original.mat
content = sio.loadmat('mnist.mat')
pixelVals = sio.loadmat('mnist_original.mat')
lamb = 1

X = content.get('Xtest')
Xpix = pixelVals.get('Xtest')
n = len(X[0])
m = len(X)
val = []
# List to store the prob. values
probList = []
# List to store the label predictions
probListLabel = []
# labelDict = defaultdict(list)
y = content.get('ytest')

# From part b) we know that beta value is all zeros
# So now compute P(y-1/x)
beta = np.zeros((1000,10), dtype = np.int)
for i in range(0, m):
	drsum = 0
	for k in range(0, 10):
		drsum = drsum + np.exp(np.dot(X[i], beta[:,k]))
	for j in range(0, 10):
		val.append(np.exp(np.dot(X[i], beta[:,j])) / float(drsum))
	# prob to a sample to max of all prob. and assign its corresponding class
	max_index, max_value = max(enumerate(val), key=operator.itemgetter(1))
	probList.append(max_value)
	probListLabel.append(max_index)	
	# labelDict[max_value].append(max_index)

# Sort it in decreasing value of probList
probList.sort(reverse=True)

# To store correct and incorrect samples
correct = []
incorrect = []

# Enumerating ground truth labels to check if 
# correctly classified or incorrectly classified
for idx,val in enumerate(y[0]):
	if val != 0 and len(incorrect) <= 5:
		incorrect.append(idx)
	elif val == 0 and len(correct) <= 5:
		correct.append(idx)

# Plotting 5 correctly classified examples
for i in range(0, 5):
	pltdata = np.array(Xpix[correct[i]]).reshape((28,28))
	img = misc.toimage(pltdata)
	# img.show()
	imsave("correct_"+str(i+1)+".png", img)

# Plotting 5 incorrectly classified examples
for i in range(0, 5):
	pltdata = np.array(Xpix[incorrect[i]]).reshape((28,28))
	img = misc.toimage(pltdata)
	# img.show()
	imsave("incorrect_"+str(i+1)+".png", img)