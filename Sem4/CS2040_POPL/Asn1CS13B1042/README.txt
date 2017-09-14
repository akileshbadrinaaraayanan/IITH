Two classes are implemented in the given program.
MyInteger and MyRational

MyInteger class

This class supports the following constructors/destructors:

1)Default constructor MyInteger() that initializes the instance with value 0.

2)Constructor MyInteger(string s) that initializes the instance by the number whose string representation is given by ’s’. Eg: MyInteger(“-34534536454”);        

3)Copy constructor that creates an instance of MyRational

4)Destructor that explicitly frees the memory.


This class supports the following member functions

1)parse(string s) - static function that returns an instance of MyInteger corresponding to the string representation ‘s’.

2) pow(MyInteger n) - returns this power n

3)abs() - returns the absolute value.

4)The following integer arithmetic operators are overloaded (+, -, *, /, %, ++, - -). 

5)The following comparator operators are overloaded (<, >, ==, ! =, <=, >=).

6)Shift operators are overloaded (<<, >>). 

7)Overload the cout << operator. It prints the number.


MyRational class

This class supports the following constructors/destructors:

1)Default constructor MyRational() that initializes the instance with value 0.

2)Constructor MyRational(String s) that initializes the instance by the number whose string representation is given by ’s’.

This class supports the following member functions

1) parse(String s) - static function that returns an instance of MyRational corresponding to the string representation ‘s’.

2) pow(MyInteger n) - returns this power n.

3) abs() - returns the absolute value.

4) intVal() - returns MyInteger that contains the integer part.

5) floatVal() - returns the floating point value.

6) toString(int p) - returns the string representation where p denotes the number of digits after the decimal floating point.

7) The real arithmetic operators are overloaded: (+, -, *, /, ++, - -).

8) The comparator operators are overloaded (<, >, ==, ! =, <=, >=)

9) The cout << operator to print the number rounded off to a maximum of 20 digits after the decimal by default.