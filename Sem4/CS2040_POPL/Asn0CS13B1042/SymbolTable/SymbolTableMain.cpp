/* This file contains code for testing the Symbol Table */
/********************DO NOT MODIFY***********************/

#include <iostream>
#include"SymbolTable.cpp"
#include "SymbolTable.h"
#include<vector>
#include<map>

using namespace std;

int main(int argc, char* argv[]){
	int choice = -1;
	SymbolTable table;
	int temp,*temp_ptr;
	char *string = new char[100];
	while(choice != 6){
		std::cout<<"1.Enter scope\n2.Exit scope\n3.Insert\n4.Probe\n5.Lookup\n6.Exit\n";
		std::cin>>choice;
		switch(choice){
			case 1:	table.enterscope();
				break;
			case 2: table.exitscope();
				break;
			case 3: std::cout<<"Enter symbol name : ";
				std::cin>>string;
				std::cout<<"Enter number : ";
				std::cin>>temp;
				temp_ptr = new int;
				*temp_ptr = temp;
				table.insert(string, temp_ptr);
				break;
			case 4:	std::cout<<"Enter symbol name : ";
				std::cin>>string;
				temp_ptr = (int*)table.probe(string);
				if(temp_ptr == NULL){
					std::cout<<"Not found\n";
				}else{
					std::cout<<string<<" : "<<*temp_ptr<<"\n";
				}
				break;
			case 5:	std::cout<<"Enter symbol name : ";
				std::cin>>string;
				temp_ptr = (int*)table.lookup(string);
				if(temp_ptr == NULL){
					std::cout<<"Not found\n";
				}else{
					std::cout<<string<<" : "<<*temp_ptr<<"\n";
				}
				break;
			case 6: break;
			default:
				std::cout<<"Invalid input\n";
				break;
		}
	}
	return 0;
}
