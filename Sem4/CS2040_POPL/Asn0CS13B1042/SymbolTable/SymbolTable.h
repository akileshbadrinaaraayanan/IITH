#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include<map>
#include<vector>
#include<cstring>

using namespace std;

class SymbolTable
{
	/* Add here the attributes required for the SymbolTable class*/

public:
   vector< map < string, void*> > Vtable; //vector of maps named Vtable
    int scope; //keeps track of current scope

	/* Declarations of the constructor and Destructors for the SymbolTable */
	SymbolTable();
	~SymbolTable();

	/* Declare the compulsory interfaces for the class */
	void enterscope(void);
	void exitscope(void);
	void insert(char *key, void *value);
	void* probe(char *key);
	void* lookup(char *key);

	/*Add any more interfaces if required */

};

#endif

