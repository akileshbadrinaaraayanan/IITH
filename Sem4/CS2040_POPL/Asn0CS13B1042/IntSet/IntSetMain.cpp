/*
Implemented by Akilesh B
CS13B1042
*/
#include <iostream>
#include<fstream>
#include<cstdlib>
#include<string>
#include "IntSet.h"
#include "IntSet.cpp"

using namespace std;

int main()
{
    int N; //No of elements to be inserted or deleted.

    int choice;

    int numberToInsert, numberToDelete, numberToCheck;

    ifstream inputFile; //input file object.
    ofstream outputFile; //output file object.

    IntSet firstSet;
    IntSet otherSet;

    cout<<"enter a choice \n 1. Insert elements \n 2. Delete elements \n 3. Check elements \n 4. Union \n 5. Intersection \n 6. Difference \n 7. Exit \n";
    cin>>choice;

while(1)
{

    if(choice == 1)
    {
        cout<<" Enter the number of elements"<<endl;
        cin>>N;

        cout<<" Enter the elements"<<endl;

            for(int i=0;i<N;i++)
            {
            cin>>numberToInsert; //number to be inserted.
            firstSet.Insert(numberToInsert);
            }
    }

else if(choice == 2)
{
    cout<<" Enter the number of elements"<<endl;
    cin>>N;
    cout<<" Enter the elements"<<endl;

        for(int i=0;i<N;i++)
        {
        cin>>numberToDelete; //number to be deleted.
        firstSet.Delete(numberToDelete);
        }
}

else if(choice == 3)
{
    cout<<" Enter the number of elements"<<endl;
    cin>>N;
    cout<<" Enter the elements"<<endl;

        for(int i=0;i<N;i++)
        {
        cin>>numberToCheck; //number to be checked.
        firstSet.Present(numberToCheck);
        }
}

else if(choice == 4)
{

int item;
string filename1,filename2;

otherSet.inputset.resize(0);

cout<<"enter the input file name"<<endl;
cin>>filename1;
cout<<"enter the output file name"<<endl;
cin>>filename2;

inputFile.open(filename1.c_str());
outputFile.open(filename2.c_str());

    if(inputFile.fail())
    {
    cerr<<"Error opening file "<<endl;
    exit(1);
    }

    while(true)
    {
    inputFile >> item;
    if(inputFile.eof())
        break;
    otherSet.Insert(item);
    }

    inputFile.close();

firstSet.Union(otherSet);

std::sort(firstSet.inputset.begin(), firstSet.inputset.end()); //for sorting the elements before writing onto the file.

for(int i = 0;i < firstSet.inputset.size(); i++ )
{
outputFile << firstSet.inputset[i];
outputFile << endl;
}
outputFile.close();

}

else if(choice == 5)
{
int item;

string filename1,filename2;

otherSet.inputset.resize(0);

cout<<"enter the input file name"<<endl;
cin>>filename1;
cout<<"enter the output file name"<<endl;
cin>>filename2;

inputFile.open(filename1.c_str());
outputFile.open(filename2.c_str());


    if(inputFile.fail())
    {
    cerr<<"Error opening file "<<endl;//if there is an error in opening the file
    exit(1);
    }

    while(true)
    {
    inputFile >> item;
    if(inputFile.eof())
        break;
    otherSet.Insert(item);
    }

inputFile.close();

firstSet.Intersection(otherSet);

std::sort(firstSet.inputset.begin(), firstSet.inputset.end()); //for sorting the elements before writing onto the file

for(int i = 0;i < firstSet.inputset.size(); i++ ) //writing on to the output file
{
outputFile << firstSet.inputset[i];
outputFile << endl;
}
outputFile.close();

}

else if(choice == 6)
{
otherSet.inputset.resize(0);

string filename1,filename2;

int item;

cout<<"enter the input file name"<<endl;
cin>>filename1;
cout<<"enter the output file name"<<endl;
cin>>filename2;

inputFile.open(filename1.c_str()); //open input file
outputFile.open(filename2.c_str()); //open output file

    if(inputFile.fail())
    {
    cerr<<"Error opening file "<<endl;
    exit(1);
    }

while(true)
    {
    inputFile >> item;
    if(inputFile.eof())
        break;
    otherSet.Insert(item);
    }

inputFile.close();


firstSet.Difference(otherSet);


std::sort(firstSet.inputset.begin(), firstSet.inputset.end()); //for sorting the elements

for(int i = 0;i < firstSet.inputset.size(); i++ )
{
outputFile << firstSet.inputset[i]; //write it to the output file
outputFile << endl;
}
outputFile.close();

}

    else if(choice == 7) //if 7 then break
        break;

    cout<<"enter a choice \n 1. Insert elements \n 2. Delete elements \n 3. Check elements \n 4. Union \n 5. Intersection \n 6. Difference \n 7. Exit \n";
    cin>>choice;

    }
}
