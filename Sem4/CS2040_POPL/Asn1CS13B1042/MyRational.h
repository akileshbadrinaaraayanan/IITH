#ifndef MYRATIONAL_H
#define MYRATIONAL_H
#include "MyInteger.h"
#include<iostream>
#include<string>

using namespace std;

class MyRational
{
private:
   // MyInteger p;
   // MyInteger q;

    public:
        MyInteger p;
    MyInteger q;

        void print();

        string roundOff();

        MyRational assign(MyInteger& newMyInteger);

        string var1;

        MyRational();

        MyRational(string s);

 static MyRational parse(string s);

        string toString(int p1);

        string floatVal();

        MyRational pow(MyInteger& newMyInteger);

        MyRational abs();

        ~MyRational();

        MyInteger intVal();

        MyRational operator++();

        MyRational operator--();

        MyRational operator+(MyRational& newMyRational);

        MyRational operator-(MyRational& newMyRational);

        MyRational operator*(MyRational& newMyRational);

        MyRational operator/(MyRational& newMyRational);

        bool operator!=(MyRational& newMyRational);

        bool operator<(MyRational& newMyRational);

        bool operator>(MyRational& newMyRational);

        bool operator==(MyRational& newMyRational);

        bool operator<=(MyRational& newMyRational);

        bool operator>=(MyRational& newMyRational);

        friend ostream &operator<<(ostream &output, MyRational& newMyRational);

};

#endif // MYRATIONAL_H
