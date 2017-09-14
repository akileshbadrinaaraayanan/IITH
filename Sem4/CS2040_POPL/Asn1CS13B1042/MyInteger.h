#ifndef _MYINTEGER_H
#define _MYINTEGER_H
#include<iostream>
#include<cstring>

using namespace std;

class MyInteger
{
    public:

        string var;
        bool isPos();

        MyInteger();

        MyInteger(string s);

        MyInteger(const MyInteger& newMyInteger);

        ~MyInteger();

        static MyInteger parse(string s);

        MyInteger pow(MyInteger& newMyInteger);

        MyInteger abs();

        MyInteger removestartzeroes();

         MyInteger operator++();

        MyInteger operator--();


        MyInteger operator+(MyInteger& newMyInteger);

        MyInteger operator-(MyInteger& newMyInteger);

        bool operator!=(MyInteger& newMyInteger);

        MyInteger operator*(MyInteger& newMyInteger);

        MyInteger operator/(MyInteger& newMyInteger);

        MyInteger operator%(MyInteger& newMyInteger);


        bool operator<(MyInteger& newMyInteger);

        bool operator>(MyInteger& newMyInteger);

        bool operator==(MyInteger& newMyInteger);


        bool operator<=(MyInteger& newMyInteger);

        bool operator>=(MyInteger& newMyInteger);

        MyInteger operator<<(MyInteger& newMyInteger);

        MyInteger operator>>(MyInteger& newMyInteger);

        friend ostream &operator<<(ostream &output, MyInteger& newMyInteger);

};

#endif // MYINTEGER_H
