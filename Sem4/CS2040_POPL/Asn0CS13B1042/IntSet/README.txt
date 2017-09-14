Written by Akilesh B
cs13b1042
20-01-2015

STL vector was used to dynamically add elements to each set.

Functions used:
Insert function checks if element already exists in the set. If not, it adds it to the set.

Delete function deletes given element from set if present, else gives appropriate message. erase() function is used for this purpose.

Present function checks if element is present in set or not.

Union function unions two sets A, B. If an element is present in both sets, care is taken so that it is included only once in the final output. 
push_back() function of vector was used for this purpose.

Intersect function checks for common elements between the two sets and outputs them. Common elements were pushed to a temp vector and later copied to firstSet.
Difference function removes all the common elements between original set and other set. erase() function was used to remove common elements.

In all these cases, firstSet (whose input we get at run time) gets modified (due to union, intersection, difference).



It consists of three files IntSet.h which is the interface, IntSet.cpp which contains actual implementation of these functions, and IntSetMain which provides the users with options and calls relevant functions.
