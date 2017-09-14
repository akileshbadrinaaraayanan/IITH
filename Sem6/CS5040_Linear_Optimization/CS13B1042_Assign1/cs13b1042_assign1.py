'''
Linear Optimization assignment 1. Finding solution to a system of linear equation by Gaussian elimination method
'''
import numpy as np 
import math 

#To find column ith of matrix
#Used in printing the solution when the system is consistent 
def column(matrix, i):
    return [row[i] for row in matrix]

# Helper function
def myfunction(x):
    if x < math.pow(10, -8):
        return 0

'''
Function which implements main algorithm of Gaussian Elimination to find the reduced row echelon form. 
'''
def reducedRowEchelonForm(M):
    # Base case 
    if not M: 
        return

    # Find rows, columns
    no_rows = len(M)
    no_columns = len(M[0])
    front = 0
    
    #iterate through the rows
    for r in range(no_rows):
        if no_columns <= front:
            return
        i = r
        while M[i][front] == 0:
            i = i + 1
            if no_rows == i:
                i = r
                front = front + 1
                if no_columns == front:
                    return
        
        M[i],M[r] = M[r],M[i] #Swap rows i and r
        temp = M[r][front]
        
        if temp != 0:
            M[r] = [ mrtemp / float(temp) for mrtemp in M[r]]
        
        for i in range(no_rows):
            if i != r:
                temp = M[i][front] #subtract temp multiplied by row r from row i
                M[i] = [ itemp - temp*rtemp for itemp,rtemp in zip(M[i],M[r])]
        
        front = front + 1

#read input from file
f = open('input.txt', 'r')
mtx = [ map(int,line.split(',')) for line in f ]

#function call
reducedRowEchelonForm(mtx)

#print np.apply_along_axis(myfunction, axis=1, arr=mtx)
a = np.array(mtx);
b = a[:, :-1]
bound = 0.000000001

#print the reduced row echelon matrix
print "The RREF is:"
for rw in mtx:
  print ', '.join( (str(rv) for rv in rw) )

ans = 0
ans1 = 0
rows = (a != 0).sum(1)
rows1 = (b != 0).sum(1)

#print rows
#print rows1

#Loop to find rank of augmented matrix
for r in rows:
    if r == 0:
        ans = ans + 1

#Loop to find rank of original matrix
for r in rows1:
    if r == 0:
        ans1 = ans1 + 1

#print ans
#print ans1

ans = len(mtx) - ans
ans1 = len(mtx) - ans1

print "The rank of augmented matrix is", ans
print "The rank of original matrix is", ans1

if ans != ans1:
    print "The system has no solution"

check = len(mtx[0]) - 1
diff = check - ans

solution = column(a, check)

if ans < check:
    print "The system has infinite solutions, described by an affine subspace of dimension ",diff 
'''Print solution
If the system is consistent then the last column of the RREF is the solution
'''
if ans == check:
    print "The system has a unique solution: "
    for s in solution:
        if abs(s) < bound:
            s = 0
        print s
