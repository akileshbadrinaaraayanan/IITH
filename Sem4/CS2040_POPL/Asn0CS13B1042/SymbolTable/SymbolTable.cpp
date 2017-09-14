/*
implementation of interface files
*/
/*
Implemented by Akilesh B
CS13B1042
*/
#include<iostream>
#include "SymbolTable.h"
#include<map>
#include<vector>
#include<algorithm>

using namespace std;

/*
Constructor
current scope is assigned 0 when an object is created.
initialize a new empty symbol table.
*/
SymbolTable::SymbolTable()
{
scope = 0;
map<string, void*> firstMap;
Vtable.push_back(firstMap);
}

/*
destructor
free memory occupied by symbol table
*/

SymbolTable::~SymbolTable()
{

}

/*
Enter a new scope
When we enter a new scope, create a temp map and push it to the vector

*/

void SymbolTable::enterscope()
{
map<string, void*> temp; //temp map
Vtable.push_back(temp);
scope++;
}

/*
Exit current scope and go up one scope. Stop at global scope
When we exit a scope pop the last.

*/

void SymbolTable::exitscope()
{
if(scope > 0) //when scope > 0
{
Vtable.pop_back();//pop it
scope--; //decrement scope
}
}

/*

Insert(key,value)pair in symbol table at current scope.
It will overwrite previous binding(if any) of current scope.

*/
void SymbolTable::insert(char *key, void *value)
{

string s(key); //converting char* to string
Vtable[scope][s] = value;
}

/*

look up for value associated with key only in the current scope. if binding exists return pointer to value
else return NULL

*/

void* SymbolTable::probe(char *key)
{
string s(key);//converting char* to string

if(Vtable[scope].find(s) != Vtable[scope].end()) //if found return pointer to value
return Vtable[scope][s];

else
    return NULL;
}

/*

Looks up for value associated with key in the current scope.
If binding exists, returns pointer to value else
goes up one scope and keeps going until it finds binding.

*/
void* SymbolTable::lookup(char *key)
{
string s(key); //converting type char* to string

int newScope = scope;

while(newScope >= 0)
{
    if(Vtable[newScope].find(s) != Vtable[newScope].end())
        return Vtable[newScope][s];
    else
    newScope--;
}
return NULL; //returns NULL if not found.

}

