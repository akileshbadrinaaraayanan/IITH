#include "MyRational.h"
#include "MyInteger.h"
#include "MyInteger.cpp"
#include<iostream>
#include<string>

using namespace std;

    MyRational::MyRational()
    {

    }

MyRational::MyRational(string s)
{
    int x = s.find(".");
    int y = s.length();

    int z = y - x - 1;

    s.erase(s.begin()+ x);
    (this->p).var = s;

    string s1(z,'0');
    s1 = "1" + s1;

    (this->q).var = s1;

    //cout<<p.var<<endl;
    //cout<<q.var<<endl;
}

MyRational::~MyRational()
{

}

void MyRational::print()
{
    cout<< (this->p).var<<endl;
    cout<< (this->q).var <<endl;
}

MyRational MyRational::assign(MyInteger& newMyInteger)
{
newMyInteger.removestartzeroes();
(this->p) = newMyInteger;
(this->q).var = "1";

return (*this);
}


MyRational MyRational::parse(string s)
{
    //cout<<"hi"<<endl;
    MyRational obj1(s);

    return obj1;
}

MyInteger MyRational::intVal()
{
    MyInteger temp;

    temp = (this->p) / (this->q);

    return temp;
}

MyRational MyRational::abs()
{

(this->p) = (this->p).abs();
(this->q) = (this->q).abs();

return (*this);
}

MyRational MyRational::operator+(MyRational& newMyRational)
{

    MyInteger temp1(newMyRational.q),temp2(this->q),temp3(newMyRational.p),temp4(this->p);
    MyInteger temp5, temp6, temp7;

    temp5 = ((temp4) * (temp1));
    //cout<<"temp5"<<temp5.var<<endl;

    temp4 = this->p;
    temp1 = newMyRational.q;

    temp6 = ((temp2) * (temp3));
    //cout<<"temp6"<<temp6.var<<endl;

    temp2 = this->q;
    temp3 = newMyRational.p;

    temp7 = temp2 * temp1;
    //cout<<"temp7"<<temp7.var<<endl;

    (this->p) = temp5 + temp6;
    //cout<<this->p.var<<endl;
    cout<<(this->p).var<<endl;


    (this->q) = temp7;
    cout<<this->q.var<<endl;

return (*this);

}

MyRational MyRational::operator-(MyRational& newMyRational)
{

    MyInteger temp1(newMyRational.q),temp2(this->q),temp3(newMyRational.p),temp4(this->p);
    MyInteger temp5, temp6, temp7;

    temp5 = ((temp4) * (temp1));
    cout<<"temp5"<<temp5.var<<endl;

    temp4 = this->p;
    temp1 = newMyRational.q;

    temp6 = ((temp2) * (temp3));
    cout<<"temp6"<<temp6.var<<endl;

    temp2 = this->q;
    temp3 = newMyRational.p;

    temp7 = temp2 * temp1;
    cout<<"temp7"<<temp7.var<<endl;

    (this->p) = temp5 - temp6;
    //cout<<this->p.var<<endl;
    cout<<(this->p).var<<endl;


    (this->q) = temp7;
    cout<<this->q.var<<endl;

return (*this);


}


MyRational MyRational::operator*(MyRational& newMyRational)
{
MyInteger temp1(newMyRational.q),temp2(this->q),temp3(newMyRational.p),temp4(this->p);
    MyInteger temp5, temp6;

    temp5 = ((temp4) * (temp3));


    temp6 = ((temp2) * (temp1));


    (this->p) = temp5;
    //cout<<(this->p).var<<endl;


    (this->q) = temp6;

return (*this);

}

MyRational MyRational::operator/(MyRational& newMyRational)
{

MyInteger temp1(newMyRational.q),temp2(this->q),temp3(newMyRational.p),temp4(this->p);
    MyInteger temp5, temp6;

    temp5 = ((temp4) * (temp1));


    temp6 = ((temp3) * (temp2));


    (this->p) = temp5;
    //cout<<(this->p).var<<endl;


    (this->q) = temp6;

return (*this);
}

MyRational MyRational::operator++()
{
(this->p) = (this->p) + (this->q);

return (*this);
}

MyRational MyRational::operator--()
{
(this->p) = (this->p) - (this->q);

return (*this);
}

bool MyRational::operator>(MyRational& newMyRational)
{

MyInteger temp1(newMyRational.q),temp2(this->q),temp3(newMyRational.p),temp4(this->p);
    MyInteger temp5, temp6, temp7,temp8("0");

    temp5 = ((temp4) * (temp1));

    temp6 = ((temp2) * (temp3));


    temp7 = temp5 - temp6;
    //cout<<(this->p).var<<endl;
if(temp7 > temp8)
{
    return true;
}
else
    return false;

}

bool MyRational::operator<(MyRational& newMyRational)
{

MyInteger temp1(newMyRational.q),temp2(this->q),temp3(newMyRational.p),temp4(this->p);
    MyInteger temp5, temp6, temp7,temp8("0");

    temp5 = ((temp4) * (temp1));

    temp6 = ((temp2) * (temp3));


    temp7 = temp5 - temp6;
    //cout<<(this->p).var<<endl;
if(temp7 < temp8)
{
    return true;
}
else
    return false;

}

bool MyRational::operator==(MyRational& newMyRational)
{
if((this->p) == (newMyRational.p) && (this->q) == (newMyRational.q))
    return true;
else
    return false;
}

bool MyRational::operator!=(MyRational& newMyRational)
{
    bool temp = ((*this) == (newMyRational));

    if(temp == true)
        return false;
    else
        return true;
}

bool MyRational::operator>=(MyRational& newMyRational)
{
    bool temp = ((*this) == (newMyRational));
    bool temp1 = ((*this) > (newMyRational));


    if(temp == true || temp1 == true)
        return true;
    else
        return false;
}

bool MyRational::operator<=(MyRational& newMyRational)
{
    bool temp = ((*this) == (newMyRational));
    bool temp1 = ((*this) < (newMyRational));


    if(temp == true || temp1 == true)
        return true;
    else
        return false;
}

MyRational MyRational::pow(MyInteger& newMyInteger)
{
  MyInteger temp,temp1,temp2;

  temp = newMyInteger;
  temp1 = this->p;
  temp2 = this->q;

(this->p) = temp1.pow(temp);
temp.var = newMyInteger.var;
(this->q) = temp2.pow(temp);

return (*this);
}

string MyRational::toString(int p1)
{
    string s,s1;
    int counter = 0;


MyInteger temp1(this->p),temp10(this->p);

 MyInteger temp3(this->q), temp11(this->q);


 if((this->p).isPos() == false || (this->q).isPos() == false)
 {
    counter = 1;

    (temp1).abs();
    (temp3).abs();
 }



s = (temp1 / temp3).var;


//cout<<"first "<<s<<endl;

temp1.var = (temp10).var;
//cout<<"outside temp1"<<temp1.var<<endl;

temp3.var = (temp11).var;
//cout<<"outside temp3"<<temp3.var<<endl;

MyInteger temp2,temp4;

temp4 = temp1 % temp3;

//cout<<" outside temp4"<<temp4.var<<endl;

s.append(".");

//cout<<"APEND "<<s<<endl;

for(int i = 0;i<p1;i++)
{
temp2.var = "10";
temp4 * temp2;
MyInteger temp5;
temp5.var = temp4.var;
temp3.var = (this->q).var;
//cout<<"inside1 "<<s<<endl;

//cout<<"inside1 "<<temp4.var<<endl;
//cout<<"inside1 "<<temp3.var<<endl;
//cout<<"inside1 "<<s.length()<<endl;
s.append((temp4 / temp3).var);

//cout<<"inside2 "<<s<<endl;
//cout<<"inside2"<<s.length()<<endl;

temp3.var = (this->q).var;

temp4 = temp5 % temp3;
//cout<<"inside1 "<<temp4.var<<endl;
}
/*int x = s.find(".");

for(int i = x+1; i<s.length();i++)
    s.at(i) = '9'-s.at(i)+'0';
*/
if(counter == 1)
{
    s1 = "-" + s;
}
else
{
    s1 = s;
}
return s1;
}

string MyRational::floatVal()
{
string s1 = toString(20);
string s3;

int pos = s1.find(".");

s3 = s1.substr(pos+1, 20);

return s3;
}

string MyRational::roundOff()
{
    string s1 = toString(21);

int x = s1.find(".");

string s(x,'0');
string s2(19,'0');
string s3,s4;

cout<<s1.at(x+21)<<endl;
cout<<"s1 is "<<s1<<endl;

if(s1.at(x+21) >= '6')
{
s3 = s + "." + s2 + "1";
s1.erase(s1.begin()+x+21);
cout<<"s1 is "<<s1<<endl;
cout<<"s3 is "<< s3<<endl;

 MyRational temp, temp1, temp2;

 temp1 = temp1.parse(s1);
 temp2 = temp2.parse(s3);

 //temp1.var = s1;
 //temp2.var = s2;

 temp = temp1 + temp2;

 cout<<"hi"<<endl;

s4 = temp.toString(20);
 return s4;
}
else
{
s1.erase(s1.begin()+x+21);
return s1;
}

}

ostream &operator<<(ostream &output, MyRational& newMyRational)
        {

            output << newMyRational.roundOff();
            return output;
        }
