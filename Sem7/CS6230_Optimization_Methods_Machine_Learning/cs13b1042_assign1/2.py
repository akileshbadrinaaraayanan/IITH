# Basic idea in this problem is to write expressions in DCP form so that it passes through the analysis and gives 
# that it is convex.

# The convexity of an expression can be checked by using exp.curvature (where exp is the expression). 
# The valid constraints under the DCP rule is: convex <= concave.
# So we convert everything into less than or equal to form and check the convexity of the expression.

from cvxpy import *

# Define 3 variables - x,y,z
x = Variable()
y = Variable()
z = Variable()

# These are the expressions for the corresponding sub-parts : from a till f.
a = square(norm(vstack(x, y, z), 2)) - 1
b = norm(vstack(x,1), 2) - (3*x+y)
c = inv_pos(x) + 2*inv_pos(y) - 5
d = quad_over_lin(x+y, sqrt(y)) - x + y - 5
e = norm(vstack(x+2*y, x-y), 2)
f = log_sum_exp(vstack(y-1, x/2)) + exp(x)

# As mentioned above, the convexity of an expression can be checked by exp.curvature (where exp can be between a,b,c,d,e,f)
print(a.curvature)