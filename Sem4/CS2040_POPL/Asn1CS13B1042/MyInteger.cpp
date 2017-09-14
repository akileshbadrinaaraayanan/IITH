/*
Implemented by Akilesh B
CS13B1042
*/

#include "MyInteger.h"
#include<iostream>
using namespace std;
#include<string>


        MyInteger::MyInteger()    // default constructor
        {
            this->var = "0";
        }

        MyInteger MyInteger::removestartzeroes() // function to remove start zeroes- leading zeros
        {
            if(this->isPos() == true)
            {
                int l = (this->var).length();
            int counter = 0;

            for(int i = 0; i<l-1; i++)
            {
                if(this->var.at(i)=='0')
                {
                    counter++;
                }
                else
                    break;
            }
            this->var.erase((this->var).begin(),(this->var).begin()+counter);
            return (*this);
            }
            else
            {
                this->var.erase((this->var).begin()+0);

                int l = (this->var).length();
            int counter = 0;

            for(int i = 0; i<l-1; i++)
            {
                if(this->var.at(i)=='0')
                {
                    counter++;
                }
                else
                    break;
            }
            this->var.erase((this->var).begin(),(this->var).begin()+counter);

            this->var = "-" + this->var;

            return (*this);
            }
        }

        MyInteger::MyInteger(string s) // parametrised constructor which creates an instance of myinteger
        {
            if(s.empty())
            {
                this->var = "0";
            }
            else
            {
                this->var = s;
            }

            removestartzeroes();
        }

        MyInteger::MyInteger(const MyInteger& newMyInteger) //copy constructor
        {
            this->var = newMyInteger.var;
        }

        MyInteger::~MyInteger() // destructor
        {

        }

        MyInteger MyInteger::parse(string s) // returns an instance of myinteger with given string
        {
            MyInteger obj(s);

            return obj;
        }

        MyInteger MyInteger::operator++() // to increment the value by 1
        {
            MyInteger temp("1");

            return((*this) + temp);

        }


        MyInteger MyInteger::operator--() // to decrement the value by 1
        {

            MyInteger temp("1");

            return ((*this) - temp);
        }

        MyInteger MyInteger::abs() // returns absolute value of given string
        {
        if(this->isPos() == true)
            return (*this);
        else
        {
            this->var.erase((this->var).begin()+0);
            return (*this);
        }

        }

        bool MyInteger::isPos() // function to check if given number is positive or negative
        {
            if(((this->var).at(0))=='-')
                return false;
            else
                return true;
        }


        MyInteger MyInteger::operator+(MyInteger& newMyInteger) // adds two myinteger objects
        {

            if(this->isPos() == true && newMyInteger.isPos() == true)
            {
                MyInteger temp;
            int l;

            int l1 = (this->var).length();
            int l2 = (newMyInteger.var).length();

            if(l1 > l2)
                {
                    l = l1- l2;
                    string s(l, '0');
                    newMyInteger.var = s + newMyInteger.var;
                }
            else
                if(l1 < l2)
            {
                l = l2-l1;
                string s(l, '0');
                this->var = s + this->var;
            }

            int l3 = max(l1,l2);
            string s1(l3+1, '0');


            int d = 0;

            for(int i = l3-1; i >=0 ; i--)
            {
                int p = (((this->var).at(i))-'0') + (((newMyInteger.var).at(i))-'0') + d;

                if(p > 9)
                {
                    p = p-10;
                    s1.at(i+1) = p +'0';
                    d = 1;
                }
                else
                {
                    s1.at(i+1) = p + '0';
                    d = 0;
                }

            }
            s1.at(0) = d +'0';

            temp.var = s1;
            temp.removestartzeroes();
            return temp;
            }
            else
                if(this->isPos() == true && newMyInteger.isPos() == false)
                 {

                     newMyInteger.var.erase(newMyInteger.var.begin()+0);
                    return((*this) - newMyInteger);
                 }
            else
                if(this->isPos() == false && newMyInteger.isPos() == true)
                 {

                    this->var.erase((this->var).begin()+0);
                    return(newMyInteger - (*this));
                 }
            else
                if(this->isPos() == false && newMyInteger.isPos() == false)
                {
                    MyInteger temp1;

                    this->var.erase((this->var).begin()+0);
                    newMyInteger.var.erase(newMyInteger.var.begin()+0);
                    temp1 = ((*this) + newMyInteger);

                temp1.var = "-" + temp1.var;

                return temp1;

                }

        }

        MyInteger MyInteger::operator-(MyInteger& newMyInteger) //OPERATOR - for subtracting two myinteger objects
        {
            MyInteger temp;
            int l1 = (this->var).length();
            int l2 = (newMyInteger.var).length();
            int l;

            if(l1 > l2)
                {
                    l = l1- l2;
                    string s(l, '0');
                    newMyInteger.var = s + newMyInteger.var;
                }
            else
                if(l1 < l2)
            {
                l = l2-l1;
                string s(l, '0');
                this->var = s + this->var;
            }


            int l3 = (this->var).length();
            int d = 0;
            int p;
            string s1(l3, '0');



            if(this->isPos() == true && newMyInteger.isPos() == true)
           {

            if((*this) >= newMyInteger)
            {
                for(int i = l3-1; i>=0; i--)
                {
                    if(((this->var).at(i)) > ((newMyInteger.var).at(i)))
                    {
                        p = ((((this->var).at(i))-'0') - (((newMyInteger.var).at(i))-'0')-d);
                        s1.at(i) = p +'0';
                        d = 0;
                    }
                    else
                        if(((this->var).at(i)) < ((newMyInteger.var).at(i)))
                    {
                        p = ((((this->var).at(i))-'0') - (((newMyInteger.var).at(i))-'0') -d + 10);
                        s1.at(i) = p + '0';
                        d = 1;
                    }

                    else
                        if(((this->var).at(i)) == ((newMyInteger.var).at(i)))
                    {
                        if(d == 1)
                            p = ((((this->var).at(i))-'0') - (((newMyInteger.var).at(i))-'0') -d + 10);
                        else
                            if(d == 0)
                            p = ((((this->var).at(i))-'0') - (((newMyInteger.var).at(i))-'0')-d);
                        s1.at(i) = p + '0';
                    }
                }

                temp.var = s1;
                temp.removestartzeroes();
                return temp;
            }
            else if((*this) < newMyInteger)
            {
                MyInteger temp1;
                temp1.var = this->var;
                this->var = newMyInteger.var;
                newMyInteger.var = temp1.var;

                for(int i = l3-1; i>=0; i--)
                {
                    if(((this->var).at(i)) > ((newMyInteger.var).at(i)))
                    {
                        p = ((((this->var).at(i))-'0') - (((newMyInteger.var).at(i))-'0')-d);
                        s1.at(i) = p +'0';
                        d = 0;
                    }
                    else
                        if(((this->var).at(i)) < ((newMyInteger.var).at(i)))
                    {
                        p = ((((this->var).at(i))-'0') - (((newMyInteger.var).at(i))-'0') -d + 10);
                        s1.at(i) = p + '0';
                        d = 1;
                    }
                    else
                        if(((this->var).at(i)) == ((newMyInteger.var).at(i)))
                    {
                        if(d == 1)
                            p = ((((this->var).at(i))-'0') - (((newMyInteger.var).at(i))-'0') -d + 10);
                        else
                            if(d == 0)
                            p = ((((this->var).at(i))-'0') - (((newMyInteger.var).at(i))-'0')-d);
                        s1.at(i) = p + '0';
                    }
                }

                temp.var = s1;
                temp.removestartzeroes();
                temp.var = "-" + temp.var;
                return temp;
            }


         }
        else
            if(this->isPos() == true && newMyInteger.isPos() == false)
        {
            newMyInteger.var.erase(newMyInteger.var.begin()+0);
            return((*this)+ newMyInteger);
        }
        else
            if(this->isPos() == false && newMyInteger.isPos() == true)
         {
                MyInteger temp1;

                    this->var.erase(this->var.begin()+0);

                    temp1 = ((*this) + newMyInteger);

                    temp1.var = "-" + temp1.var ;
                    return temp1;
         }

         else
            if(this->isPos() == false && newMyInteger.isPos() == false)
            {
                newMyInteger.var.erase(newMyInteger.var.begin()+0);
                this->var.erase(this->var.begin()+0);
                return(newMyInteger - (*this));
            }

    }



MyInteger MyInteger::operator*(MyInteger& newMyInteger) //multiplication operator
{

    MyInteger temp(this->var);

    MyInteger temp2(newMyInteger.var);


    if(temp.isPos() == false && temp2.isPos() == false)
    {

        temp=temp.abs();

        temp2=temp2.abs();

        MyInteger answer = temp * temp2;

        this->var = (answer.var);
        this->removestartzeroes();

        return (*this);

    }

    else if(temp.isPos() == true && temp2.isPos() == false)
        {


        temp2=temp2.abs();

        MyInteger answer=temp*temp2;

        answer.var = "-" + answer.var;

        this->var = (answer.var);
        this->removestartzeroes();
        return (*this);

    }
    else
        if(temp.isPos() == false && temp2.isPos() == true)
    {
        temp=temp.abs();


        MyInteger answer=temp*temp2;

        answer.var = "-" + answer.var;

        this->var = (answer.var);
        this->removestartzeroes();
        return (*this);


    }

    else
    {
        MyInteger myint_array[temp2.var.size()];
        MyInteger answer;



        int carry=0;

        for(int i=temp2.var.size()-1;i>=0;i--)
            {
            myint_array[i].var="";

            carry=0;

            for(int j=temp.var.size()-1;j>=0;j--)
            {
                int x=temp2.var[i]-'0';
                int y=temp.var[j]-'0';

                int res=x*y+carry;
                carry=res/10;

                char v=res%10+'0';

                myint_array[i].var=v+myint_array[i].var;
            }
            string s(temp2.var.size()-1-i,'0');

            myint_array[i].var=myint_array[i].var+s;


            if(carry!=0)
            {
                char l=carry+'0';
                myint_array[i].var=l+myint_array[i].var;
            }
        }

        for(int i=0;i<temp2.var.size();i++)
        {

            answer=answer+myint_array[i];

        }

        this->var = (answer.var);
        this->removestartzeroes();
        return *this;

    }
return (*this);

}


MyInteger MyInteger::operator/(MyInteger& newMyInteger)
{
    MyInteger temp10("0");
    MyInteger temp(this->var);
    MyInteger temp2(newMyInteger.var);

if(temp2 == temp10 && temp == temp10)
{
cout<<"Undefined value"<<endl;
}
if(temp != temp10 && temp2 == temp10)
{
    cout<<"division by zero"<<endl;
}

    if(temp.isPos() == false && temp2.isPos() == false)
    {
        temp=temp.abs();
        temp2=temp2.abs();
        MyInteger answer=temp/temp2;

        this->var = answer.var;
        this->removestartzeroes();

        return *this;
    }
    else if(temp.isPos() == false && temp2.isPos() == true)
    {
        temp=temp.abs();
        temp2=temp2.abs();

        MyInteger answer=temp/temp2;
        answer.var="-"+answer.var;

        this->var = answer.var;
        this->removestartzeroes();
        return *this;
    }
    else if(temp.isPos() == true && temp2.isPos() == false)
    {
        temp=temp.abs();
        temp2=temp2.abs();

        MyInteger answer=temp/temp2;
        answer.var="-"+answer.var;

        this->var = answer.var;
        this->removestartzeroes();
        return *this;
    }
    else
        {
        MyInteger answer("");
        MyInteger res("");
        int number=0;
        while(1)
        {
            if(res<temp2)
            {
                res.var=res.var+temp.var[number];
                number++;
                answer.var=answer.var+"0";
            }
            if(res>=temp2)
            {
                MyInteger temp3("0");
                MyInteger count("1");
                MyInteger temp4("1");

                while(temp3<=res)
                {
                MyInteger z=temp2*count;
                temp2.var = newMyInteger.var;

                temp3 = temp2 * count;

                temp2.var = newMyInteger.var;

                count = count + temp4;

                }
                count = count - temp4;

                count = count - temp4;

                answer=answer+count;
                temp3=temp2*count;
                temp2.var = newMyInteger.var;
                res=res-temp3;
            }
            if(number==temp.var.size())
            {
                break;
            }
        }
        this->var = answer.var;
        this->removestartzeroes();
        return (*this);
    }
}


        MyInteger MyInteger::operator%(MyInteger& newMyInteger)
        {
            MyInteger temp(this->var);
            MyInteger temp2(newMyInteger.var);

            if(temp.isPos() == true && temp2.isPos() == true)
            {
            MyInteger temp3,temp4,temp5;

            temp3 = temp / temp2;

            temp2.var = newMyInteger.var;
            temp.var = this->var;

            temp4 = temp3 * temp2;
            temp5 = temp - temp4;

            return temp5;
            }
            else if(temp.isPos()== false && temp2.isPos() == true)
            {
                MyInteger temp6;
                MyInteger temp7(newMyInteger.var);
                MyInteger temp8("0");

                temp = temp.abs();
                temp2 = temp2.abs();

                temp6 = temp % temp2;

                if(temp6 == temp8)
                {
                    return temp8;
                }
                else
                {
                    return(temp7 - temp6);
                }

            }

        }

         MyInteger MyInteger::pow(MyInteger& newMyInteger)
        {
            MyInteger temp1(this->var);
            MyInteger temp2(this->var);
            MyInteger temp3("1");
            MyInteger temp4("0");

            if(this->isPos() == true || this->isPos() == false && newMyInteger.isPos() == true)
            {
                while(newMyInteger != temp3)
                {

                    temp1 = temp2;

                   (*this) = (*this) * (temp1);

                    newMyInteger=newMyInteger-temp3;

                }
                return (*this);
            }
            else
                if(this->isPos() == true || this->isPos() == false && newMyInteger.isPos() == false)
            {
                return temp4;
            }

        }

        bool MyInteger::operator>(MyInteger& newMyInteger)
        {
            if(this->var == "")
                return false;

            if(newMyInteger.var == "")
                return true;

            int l1 = (this->var).length();
            int l2 = (newMyInteger.var).length();
            int l;

            if(l1 > l2)
                {
                    l = l1- l2;
                    string s(l, '0');
                    newMyInteger.var = s + newMyInteger.var;
                }
            else
                if(l1 < l2)
            {
                l = l2-l1;
                string s(l, '0');
                this->var = s + this->var;
            }


            int l3 = (this->var).length();


            if(this->isPos() == true && newMyInteger.isPos() == true)
            {
                for(int i = 0;i<l3;i++)
                {
                    if(this->var.at(i) > newMyInteger.var.at(i))
                {
                    return true;
                }
                else
                    if(this->var.at(i) < newMyInteger.var.at(i))
                {
                    return false;
                }

                }
                return false;
              }

            else
                if(this->isPos() == true && newMyInteger.isPos() == false)
            {
                return true;
            }
            else
                if(this->isPos() == false && newMyInteger.isPos() == true)
            {
                return false;
            }
            else
                if(this->isPos() == false && newMyInteger.isPos() == false)
            {
                    this->var.erase((this->var).begin()+0);
                    newMyInteger.var.erase(newMyInteger.var.begin()+0);


                     for(int i = 0;i<l3;i++)
                {
                        if(this->var.at(i) > newMyInteger.var.at(i))
                    {
                        return false;
                    }
                    else
                        if(this->var.at(i) < newMyInteger.var.at(i))
                    {
                        return true;
                    }

                }
                return false;
            }


        }

        bool MyInteger::operator<(MyInteger& newMyInteger)
        {
            if(this->var == "")
                return true;

            if(newMyInteger.var == "")
                return false;

            int l1 = (this->var).length();
            int l2 = (newMyInteger.var).length();
            int l, counter = 0;

            if(l1 > l2)
                {
                    l = l1- l2;
                    string s(l, '0');
                    newMyInteger.var = s + newMyInteger.var;
                }
            else
                if(l1 < l2)
            {
                l = l2-l1;
                string s(l, '0');
                this->var = s + this->var;
            }


            int l3 = (this->var).length();


            if(this->isPos() == true && newMyInteger.isPos() == true)
            {

                     for(int i = 0;i<l3;i++)
                {
                        if(this->var.at(i) > newMyInteger.var.at(i))
                    {
                        return false;
                    }
                    else
                        if(this->var.at(i) < newMyInteger.var.at(i))
                    {
                        return true;
                    }

                }

            return false;
            }
            else
                if(this->isPos() == true && newMyInteger.isPos() == false)
            {
                return false;
            }
            else
                if(this->isPos() == false && newMyInteger.isPos() == true)
            {
                return true;
            }
            else
                if(this->isPos() == false && newMyInteger.isPos() == false)
            {
                    this->var.erase(this->var.begin()+0);
                    newMyInteger.var.erase(newMyInteger.var.begin()+0);

                    for(int i = 0;i<l3;i++)
                {
                        if(this->var.at(i) > newMyInteger.var.at(i))
                    {
                        return true;
                    }
                    else
                        if(this->var.at(i) < newMyInteger.var.at(i))
                    {
                        return false;
                    }

                }
                return false;
              }


            }

        bool MyInteger::operator==(MyInteger& newMyInteger)
        {

            int l1 = (this->var).length();
            int l2 = (newMyInteger.var).length();
            int l;

             if(l1 > l2)
                {
                    l = l1- l2;
                    string s(l, '0');
                    newMyInteger.var = s + newMyInteger.var;
                }
            else
                if(l1 < l2)
            {
                l = l2-l1;
                string s(l, '0');
                this->var = s + this->var;
            }


            int l3 = (this->var).length();

            if(this->isPos() == true && newMyInteger.isPos() == true)
            {
                int counter1 = 0;

            for(int i = 0; i <= l3-1; i++)
            {
                if(((this->var).at(i))==((newMyInteger.var).at(i)))
                {
                    counter1++;
                }
            }
            if(counter1 == l3)
                return true;
            else
                return false;

            }
            else
                if(this->isPos() == true && newMyInteger.isPos() == false)
            {
                return false;
            }
            else
                if(this->isPos() == false && newMyInteger.isPos() == true)
            {
                return false;
            }
            else
                if(this->isPos() == false && newMyInteger.isPos() == false)
            {
                this->var.erase(this->var.begin()+0);
                newMyInteger.var.erase(newMyInteger.var.begin()+0);

                int counter1 = 0;

        for(int i = 0; i <= l3-1; i++)
        {
            if(((this->var).at(i))==((newMyInteger.var).at(i)))
            {
                counter1++;
            }
        }
        if(counter1 == l3)
            return true;
        else
            return false;

            }

        }

        bool MyInteger::operator!=(MyInteger& newMyInteger)
        {

            bool temp = ((*this) == newMyInteger);

            if(temp == true)
            {
                return false;
            }
            else
                if(temp == false)
                {
                    return true;
                }
        }

        bool MyInteger::operator>=(MyInteger& newMyInteger)
        {
            bool temp1 = ((*this)> newMyInteger);
            bool temp2 = ((*this)== newMyInteger);

            if(temp1 == true || temp2 == true)
            {
                return true;
            }
            else
                {
                    return false;
                }
        }

        bool MyInteger::operator<=(MyInteger& newMyInteger)
        {
            bool temp1 = ((*this) < newMyInteger);
            bool temp2 = ((*this)== newMyInteger);

            if(temp1 == true || temp2 == true)
            {
                return true;
            }
            else
                {
                    return false;
                }
        }

        MyInteger MyInteger::operator<<(MyInteger& newMyInteger)
        {
            MyInteger temp("2");
            MyInteger temp1, temp2,temp3(this->var),temp4;

            temp2.var = newMyInteger.var;

            temp1 = temp.pow(temp2);

            temp4 = temp3 * temp1;

            this->var = temp4.var;
            return (*this);
        }

        MyInteger MyInteger::operator>>(MyInteger& newMyInteger)
        {
                    MyInteger temp("2");
            MyInteger temp1, temp2,temp3(this->var),temp4;

            temp2.var = newMyInteger.var;

            temp1 = temp.pow(temp2);

            temp4 = temp3 / temp1;

            this->var = temp4.var;
            return (*this);

        }

        ostream &operator<<(ostream &output, MyInteger& newMyInteger)
        {
            output << newMyInteger.var;
            return output;
        }

/*    int main()
        {
            MyInteger test,test2,test4,test5;
            //test.var ="20000000000000000000000000000000000000";

            //int x = test.var.length();
            //cout<<"test length is "<<x<<endl;

            MyInteger test1("100");

            //MyInteger test3("-240");

             test2.var = "52";
             //test4.var="-2342334892743975398453452342341";
             //test2.var = "4987453453345345345";
                //test5 = test4 - test2  ;
           test5 = test2 << test1;

           // test.removestartzeroes();

            //int y = test5.var.length();
            //cout<<"test 5 length "<<y<<endl;
            cout<<test5<<endl;
        }

*/
/*int main()
{
 MyInteger a("17100");
MyInteger b("10000");

MyInteger c,d;
//d.var = "1456";
c = a %(b);
//c = c + d;
cout<<c<<endl;

}*/


