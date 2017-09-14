# COOL Compiler #

------------------------------- Written by Akilesh B, CS13B1042 ----------------------------------------------

The first task in this assignment is to define regular expressions for all the tokens provided in the tokens section of the grammar file. Some of the tokens like semicolon, plus etc. are just simple symbols, some like Bool constants are just text with the first letter being small (ie. in both true and false, the first letter should be small case and others can be anything). 

The tough part was to define an appropriate regular expression for string constants. I accomplished this by defining two fragments. First fragment allows everything except '\n', '\u0000' (NULL character), single quote ('\"') and single slash ('\\'). It is because '\\' is allowed only if it is followed by something which is not NULL. This was the challenging part in the assignment. (Please refer to the grammar for regex).

Error handling for strings was done for unterminated string constant (If '\n' is encountered anywhere within opening quotes and without closing quotes, null character in string (If '\u0000' is located anywhere in between the string) and for EOF.

processString() function is called for all strings, which takes care of cases like '\c' denotes a character 'c' except \b, \n, \t, \f and other cases mentioned in Section 10.2 of cool manual. The task in the cases of \b, \n, is to make \ and n as a single character \n. If ith character is a '\\' and i+1th character is a 'n' then take substring of string from 0 to i and concatenate with substring of string from i+2 to length of the string. 

After processing the string, I check if the length of the string is > 1025 or not. One tricky part here is if a string's length is more than 1024 and is unterminated(unescaped \n), cool reports "String constant too long" instead of "Unterminated string constant". So, in order to acheive this I change the return type of processString() to Integer and return 1, if "String constant too long" error is reported and return 0 otherwise.
The place where I define "EOF in string" and "Unterminated string constant" I check the return value of processString() and print error messages appropriately.

The last part was error handling in case of nested comments. I used ANTLR modes to achieve this which is some kind of stack. When I first encounter (*, I go into another level and recurse. For every occurence of *), I pop it. This way, I handle EOF in comments and Unmatched *). 

Thus, a lexer for COOL language was built using ANTLR. 

