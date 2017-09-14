# Two phase simplex method implemented by Akilesh B, CS13B1042
# Helper function used for finding the pivot and returns the modified tableau
# Input is in standard form
def pivotFind(mat, row, col):
  i = j = 0
  pivot = mat[row][col]
  
  for x in mat[row]:
    mat[row][j] = mat[row][j] / pivot
    j = j + 1
 #i = 0
  for ei in mat:
    if i != row:
      ratio = ei[col]
      j = 0
      for eij in ei:
        eij = eij - ratio * mat[row][j]
        mat[i][j] = eij
        j = j + 1
    i = i + 1
  
  return mat

# Phase 1, where we transform the original lp to another and check if
# Zj - Cj = 0 and there are no artificial variables present in the final tableau
def simplex_phase_1(mat):
  opt   = False
  unbounded  = False
  r = len(mat[0])
  c = len(mat) - 2

  while ((not unbounded) and (not opt)):
    pivotColInd = j = 1
    least = 0.0
  
    while(j < (r-c)):
      cj = mat[1][j]
      if (cj < least):
        pivotColInd = j
        least = cj
      j = j + 1
  
    if least == 0.0:
      opt = True
      continue
    
    INF = -1 
    pivotRowInd = i = 0
    minTh = INF

    for ei in mat:
      if (i > 1):
        eij = ei[pivotColInd]
        if eij > 0:
          th = (ei[0] / eij)
          if (minTh == INF) or (th < minTh):
            minTh = th
            pivotRowInd = i
      i = i + 1
    
    if minTh == INF:
      unbounded = True
      continue
    
    mat = pivotFind(mat, pivotRowInd, pivotColInd)
  
  print "optimal = %s" % opt
  print "unbounded = %s" % unbounded
  return mat

#Traditional Simplex 
def simplex_orig(mat):
  opt   = False
  unbounded  = False
  r = len(mat[0])
  c = len(mat) - 1

  while ((not unbounded) and (not opt)):
    pivotColInd = j = 0
    least = 0.0
    
    while(j < (r - c)):
      cj = mat[0][j]
      if (cj < least) and (j > 0):
        pivotColInd = j
        least = cj
      j = j + 1   
  
    if least == 0.0:
      opt = True
      continue
  
    INF = -1
    pivotRowInd = i = 0
    minTh = INF

    for ei in mat:
      if (i > 0):
        eij = ei[pivotColInd]
        if eij > 0:
          th = (ei[0] / eij)
          if (minTh == INF) or (th < minTh):
            minTh = th
            pivotRowInd = i
      i = i + 1

    if minTh == INF:
      unbounded = True
      continue
  
    mat = pivotFind(mat, pivotRowInd, pivotColInd)
    # #print 'opt = '.format(opt) 
    # print "optimal = %s" % opt
    # print "unbounded = %s" % unbounded
  return mat


# Two phase - First checks if it is infeasible
# If not calls first phase, then there is movement of artificial variables
# Finally returns the final tableau  
def simplex_two_phase(mat):
  infeasible  = False
  mat = simplex_phase_1(mat)
  slackVar = mat[1][0]

  if (slackVar > 0):
    infeasible  = True
    print 'infeasible'

  else:
  #slackVar is equals to zero
    #mat = artificial_var_send(mat)
    basisCheck = True
    r = len(mat[0])
    j = r - 1
    
    while(j > 0):
      present = False
      i = -1
      row = 0
      for ei in mat:
        i = i + 1
        if (ei[j] == 1):
          if (present == True):
            basisCheck = False
            continue

          elif (i > 1):
            row = i
            present = True

        elif (ei[0] != 0):
          basisCheck = False
          continue
      
      if (basisCheck and present):
        if (r <= j):
          mat = pivotFind(mat, row, j)

        else:
          print 'do nothing'
      
      j = j - 1

    m = len(mat) - 2
    n = len(mat[0])
    n = n - m
    mat.pop(1)
  
    i = 0
    while (i < len(mat)):
      mat[i] = mat[i][:n]
      i = i + 1

    mat = simplex_orig(mat)
  return mat

# Print the tableau 
def printTableauForm(mat):
  print '********************************'
  for row in mat:
    print row
  print '********************************'
  return

# Reading inputs from a file
num_lines = sum(1 for line in open('input1.txt'))
#print num_lines

count = num_lines-1

c = []

eq = [[] for x in xrange(count)]

for i in xrange(1, count):
  eq[i] = []

b = []

with open('input1.txt', 'r') as f:
    array = [[float(x) for x in line.split()] for line in f]

c.extend(array[0])


for i in xrange(1, count):
  eq[i].extend(array[i])

b.extend(array[count])

# for i in xrange(1, count):
#   print array[i]

eqs = []

# for i in xrange(1, count):
#   print eq[i]

for i in xrange(1, count):
  eqs.append(eq[i])


# To get the initial tableau by adding slack variables
# adding artificial variables
# Here the assumption is that b vector is positive, if not we multiply the whole equation by -1
mat = []
artificialVar = []
slackVar = [0.0]

m = len(eqs)
n = len(c)
c.insert(0, 0.0)

i = 0
while (i < n):
  slackVar.append(0.0)
  i = i + 1

i = 0
while (i < m):
  artificialVar.append(0.0)
  slackVar.append(1.0)
  i = i + 1

c.extend(artificialVar)
mat.append(c)
mat.append(slackVar)

i = 0
for eq in eqs:
  eq.insert(0, b[i])
  eq.extend(artificialVar)
  eq[n + i + 1] = 1.0
  mat.append(eq)
  i = i + 1

i = 0 
for ei in mat:
  if (i > 1):
    j = 0
    for eij in ei:
      mat[1][j] = mat[1][j] - eij
      j = j + 1
  i = i + 1

printTableauForm(mat)
mat = simplex_two_phase(mat)
printTableauForm(mat)
print "Optimal Cost = %s" % -mat[0][0]