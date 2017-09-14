Written by Akilesh B
cs13b1042
20-01-2015 

Use a vector of maps.

Functions used:

Basic constructor which initialises with an empty set
Parametereized constructor which initialises the set with the elements given in the array
Destructor.
enterscope(): A new map is created. It is pushed to vector.
exitscope(): pop_back() function to pop out of current scope. 
Insert(): Inserts(key,value)pair in symbol table at current scope. It overwrites previous binding(if any) of current scope.
Probe() :Looks up for value associated with key only in the current scope. if match exists return pointer to value, otherwise returns NULL. It uses find() function to check if key is present in map.
Lookup(): Looks up for value associated with key in the current scope. If binding exists, returns pointer to value else goes up one scope and keeps going until it finds binding. Basically, repeated probes till scope becomes 0.

It consists of three files SymbolTable.h which is the interface, SymbolTable.cpp which contains actual implementation of these functions, and SymbolTableMain which provides the users with options and calls relevant functions.
