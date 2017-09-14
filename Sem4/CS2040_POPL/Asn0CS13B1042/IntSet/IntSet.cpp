//IMPLEMENTING ALL THE INTERFACE FUNCTIONS
/*
Implemented by Akilesh B
CS13B1042
*/

#include<iostream>
#include "IntSet.h"
#include <vector>
#include<algorithm>
#include<string>

using namespace std;

/*
Constructor for IntSet class
*/
IntSet::IntSet()
{

}

/*
Parametrized constructor which initializes the set with the elements
given in the array
*/

IntSet::IntSet(int a[],int size)
{

}
/* destructor */
IntSet::~IntSet()
{

}

/*
Inserts element into the set
parameter i: element to be inserted
return : True if element was not previously present, False otherwise
*/

bool IntSet::Insert(int i)
{
    for(int k=0;k<inputset.size();k++)
    {
        if(inputset[k] == i)
        {
        cout<<i<<" ELEMENT ALREADY PRESENT"<<endl;  //prints if already present.
        return false;
        }
    }

inputset.push_back(i);
return true;

}
/*
Deletes an element from the set
parameter i: element to be deleted
return : False if element was not previously present, True otherwise
*/

bool IntSet::Delete(int i)
{
    for(int k=0;k<inputset.size();k++)
    {
    if(inputset[k] == i)
        {
        inputset.erase(inputset.begin()+k);
        return true;
        }

    }
cout<<i<<" ELEMENT NOT PRESENT"<<endl; //if trying to delete an element not present.
return false;
}

/*
Checks for the presence of element in the set
param i: query element
return : True if element is present, False otherwise
*/

bool IntSet::Present(int i)
{
    for(int k=0;k<inputset.size();k++)
    {
        if(inputset[k] == i)
	{
	cout<<"True"<<endl;
        return true;
	}
    }
cout<<"False"<<endl;
return false;
}

//Prints the elements present in the set in ascending order

void IntSet::PrintSet()
{
std::sort(inputset.begin(), inputset.end());

    for(int i=0;i<inputset.size();i++)
    cout<<inputset[i];

cout<<endl;
}

/*Unions this set with the given set
parameter otherSet: The set with which this set is to be unioned
     */
void IntSet::Union(const IntSet& otherSet)
{
int j;
    for(int i=0;i<otherSet.inputset.size();i++)
    {
        for(j=0;j<inputset.size();j++)
            {
                if(otherSet.inputset[i] == inputset[j])
                break;
            }
        if(j == inputset.size())  //unions to the end of first set to find AUB
        {
        inputset.push_back(otherSet.inputset[i]);
        }
    }
}

/*Intersect this set with the given set
parameter otherSet: The set with which this set's intersection is to be calculated
*/

void IntSet::Intersection(const IntSet& otherSet)
{
std::vector<int> common;
    for(int i=0;i<otherSet.inputset.size();i++)
    {
        for(int j=0;j<inputset.size();j++)
        {
            if(otherSet.inputset[i] == inputset[j]) //pushing only the common elements to find intersection
            {
            common.push_back(inputset[j]);
            break;
            }
        }
    }

        inputset = common; //making it the original set

}

/*
Compute this-otherSet (set difference)
parameter otherSet: The set with which this set's difference is to be calculated
*/

void IntSet::Difference(const IntSet& otherSet)
{
    for(int i=0;i<otherSet.inputset.size();i++)
    {
        for(int j=0;j<inputset.size();j++)
        {
            if(otherSet.inputset[i] == inputset[j]) //remove the common elements to find A-B
            {
            inputset.erase(inputset.begin()+j);
            break;
            }
        }
    }
}

