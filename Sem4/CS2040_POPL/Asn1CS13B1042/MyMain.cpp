#include<iostream>
#include<string>
#include "MyRational.cpp"

using namespace std;

int main()
{
int counter, counter1,counter2;
MyInteger temp1, temp2, temp5;
MyRational temp3, temp4, temp6;

string s,s1,s2;

cin>>s;

A:

if(s.at(0) != '$')
{
cin>>s1;
if(s1.at(0) != '$')
{
cin>>s2;

if(s.find(".") == -1)
{
    temp1.var = s;
    counter = 0;
}
else
{
temp3 = temp3.parse(s);
    //cout<<"hi"<<endl;
    counter = 1;
cout<<"this is temp3"<<temp3.toString(20)<<endl;
}

if(s2.find(".") == -1)
{
        temp2.var = s2;
    counter1 = 0;
    //cout<<s2<<endl;

}
else
{
cout<<"this is s2"<<s2<<endl;
temp4 = temp4.parse(s2);
    counter1 = 1;
    //cout<<"hi"<<endl;

//cout<<"this is temp4"<<temp4.toString(20)<<endl;
//cout<<"this is temp4 hi"<<temp4.toString(20)<<endl;
}

if(s1 == "+")
{
    if(counter == 0 && counter1 == 0)
    {
        temp5 = temp1 + temp2;
        counter2 = 1;
    }
    else
        if(counter == 1 && counter1 == 0)
    {
        MyRational temp7;
        (temp7.p) = temp2;
        (temp7.q).var = "1";

                temp6 = temp3 + temp7;
        counter2 = 0;
    }
    else
        if(counter == 0 && counter1 == 1)
    {
        MyRational temp7;
        (temp7.p) = temp1;
        (temp7.q).var = "1";

            temp6 = temp4 + temp7;

            counter2 = 0;
    }
    else
        if(counter == 1 && counter1 == 1)
    {
       // cout<<"inside temp3"<<temp3.toString(20)<<endl;
        //cout<<"inside temp4"<<temp4.toString(20)<<endl;
        temp6 = temp3 + temp4;
        counter2 = 0;
        //cout<<"inside temp6"<<temp6.toString(20)<<endl;
    }


if(counter2 == 1)
    s = temp5.var<<endl;
else if(counter2 == 0)
    s = temp6.toString(20)<<endl;

cout<<s<<endl;


goto A;
}
else if(s1 == "-")
{
    if(counter == 0 && counter1 == 0)
    {
        temp5 = temp1 - temp2;
        counter2 = 1;
    }
    else
        if(counter == 1 && counter1 == 0)
    {
        MyRational temp7;
        (temp7.p) = temp2;
        (temp7.q).var = "1";

                temp6 = temp3 - temp7;

                counter2 = 0;
    }
    else
        if(counter == 0 && counter1 == 1)
    {
        MyRational temp7;
        (temp7.p) = temp1;
        (temp7.q).var = "1";

            temp6 = temp7 - temp4;

            counter2 = 0;
    }
    else
        if(counter == 1 && counter1 == 1)
    {
        temp6 = temp3 - temp4;
        counter2 = 0;
    }


if(counter2 == 1)
    s = temp5.var<<endl;
else if(counter2 == 0)
    s = temp6.toString(20)<<endl;

cout<<s<<endl;


goto A;

}

else if(s1 == "*")
{
    if(counter == 0 && counter1 == 0)
    {
        //cout<<"inside "<<temp1.var<<endl;
        //cout<<"inside "<<temp2.var<<endl;
        temp5 = temp1 * temp2;
        //cout<<"inside "<<temp5.var<<endl;
        counter2 = 1;
        //cout<<"inside counter"<<counter2<<endl;
    }
    else
        if(counter == 1 && counter1 == 0)
    {
        MyRational temp7;
        (temp7.p) = temp2;
        (temp7.q).var = "1";

                temp6 = temp3 * temp7;
                counter2 = 0;
    }
    else
        if(counter == 0 && counter1 == 1)
    {
        MyRational temp7;
        (temp7.p) = temp1;
        (temp7.q).var = "1";

            temp6 = temp7 * temp4;
            counter2 = 0;
    }
    else
        if(counter == 1 && counter1 == 1)
    {
        temp6 = temp3 * temp4;
        counter2 = 0;
    }

//cout<<s<<endl;
//cout<<"outside counter"<<counter2<<endl;


if(counter2 == 1)
    s = temp5.var<<endl;
else if(counter2 == 0)
    s = temp6.toString(20)<<endl;

cout<<s<<endl;

goto A;

}

else if(s1 == "/")
{
    if(counter == 0 && counter1 == 0)
    {
        temp5 = temp1 / temp2;
        counter2 = 1;
    }
    else
        if(counter == 1 && counter1 == 0)
    {
        MyRational temp7;
        (temp7.p) = temp2;
        (temp7.q).var = "1";

                temp6 = temp3 / temp7;

        counter2 = 0;
    }
    else
        if(counter == 0 && counter1 == 1)
    {
        MyRational temp7;
        (temp7.p) = temp1;
        (temp7.q).var = "1";

            temp6 = temp7 / temp4;

                    counter2 = 0;
    }
    else
        if(counter == 1 && counter1 == 1)
    {
        temp6 = temp3 / temp4;
                counter2 = 0;
    }


if(counter2 == 1)
    s = temp5.var<<endl;
else if(counter2 == 0)
    s = temp6.toString(20)<<endl;

cout<<s<<endl;


goto A;

}

else if(s1 == "pow")
{
    if(counter == 0 && counter1 == 0)
    {
        temp5 = temp1.pow(temp2);
            counter2 = 1;
    }
    else
        if(counter == 1 && counter1 == 0)
    {

        temp6 = temp3.pow(temp2);
        counter2 = 0;
    }


if(counter2 == 1)
    s = temp5.var<<endl;
else if(counter2 == 0)
    s = temp6.toString(20)<<endl;

cout<<s<<endl;
goto A;

}

}
else
{
    cout<<"$ encountered "<<endl;
}

}
else if(s.at(0) == '$')
{
cout<<"no number encountered"<<endl;
}

}
