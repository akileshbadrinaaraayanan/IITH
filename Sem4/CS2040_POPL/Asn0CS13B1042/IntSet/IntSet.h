#ifndef __INTSET_H
#define __INTSET_H
#include <vector>
#include<algorithm>

class IntSet{
  /* Add here the attributes required for the IntSet */



  public:
      /* a vector by name inputset */
    std::vector<int> inputset;
    /* Constructors for the IntSet class */
    /*
     * Basic constructor which initialises with an empty set
     */
    IntSet();
    /*
     * Parametereized constructor which initialises the set with the elements
     * given in the array
     */
    IntSet(int a[],int size);

    /* Destructor for the class */
    ~IntSet();

    /* Compulsory methods to be implemented */

    /*
     * Inserts element into the set
     * param i: element to be inserted
     * return : True if element was not previously present, False otherwise
     */
    bool Insert(int i);

    /*
     * Deletes an element from the set
     * param i: element to be deleted
     * return : False if element was not previously present, True otherwise
     */
    bool Delete(int i);

    /*
     * Checks for the presence of element in the set
     * param i: query element
     * return : True if element is present, False otherwise
     */
    bool Present(int i);

    /*
     * Prints the elements present in the set in ascending order
     */
    void PrintSet();

    /*
     * Unions this set with the given set
     * param otherSet: The set with which this set is to be unioned
     */
    void Union(const IntSet& otherSet);

    /*
     * Intersect this set with the given set
     * param otherSet: The set with which this set's intersection is to
     *                 be calculated
     */
    void Intersection(const IntSet& otherSet);

    /*
     * Compute this-otherSet (set difference)
     * param otherSet: The set with which this set's difference is to
     *                 be calculated
     */
    void Difference(const IntSet& otherSet);

    /* Add any other interface methods that you may require */
};

#endif
