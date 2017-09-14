lexer grammar CoolLexer;

tokens{
	ERROR,
	TYPEID,
	OBJECTID,
	BOOL_CONST,
	INT_CONST,
	STR_CONST,
	LPAREN,
	RPAREN,
	COLON,
	ATSYM,
	SEMICOLON,
	COMMA,
	PLUS,
	MINUS,
	STAR,
	SLASH,
	TILDE,
	LT,
	EQUALS,
	LBRACE,
	RBRACE,
	DOT,
	DARROW,
	LE,
	ASSIGN,
	CLASS,
	ELSE,
	FI,
	IF,
	IN,
	INHERITS,
	LET,
	LOOP,
	POOL,
	THEN,
	WHILE,
	CASE,
	ESAC,
	OF,
	NEW,
	ISVOID,
	NOT
}

/*
  DO NOT EDIT CODE ABOVE THIS LINE
*/

@members{

	/*
		YOU CAN ADD YOUR MEMBER VARIABLES AND METHODS HERE
	*/

	/**
	* Function to report errors.
	* Use this function whenever your lexer encounters any erroneous input
	* DO NOT EDIT THIS FUNCTION
	*/
	public void reportError(String errorString){
		setText(errorString);
		setType(ERROR);
	}
	// This function takes care of invalid token. If there is any invalid token, report error with same text
	public void invalidToken() {
		Token t = _factory.create(_tokenFactorySourcePair, _type, _text, _channel, _tokenStartCharIndex, getCharIndex()-1, _tokenStartLine, _tokenStartCharPositionInLine);
                String text = t.getText();
		reportError(text);
	}
	// processString() function 
	public int processString() {
		Token t = _factory.create(_tokenFactorySourcePair, _type, _text, _channel, _tokenStartCharIndex, getCharIndex()-1, _tokenStartLine, _tokenStartCharPositionInLine);
		String text = t.getText();

		//write your code to test strings here

		text = text.substring(1,text.length()-1);
		
		for(int i=0; i<text.length(); i++)
		{
			if(text.charAt(i) == '\\')		//Check if it is a slash
			{
				if(text.charAt(i+1) == 'n' || text.charAt(i+1) == '\n')		//If it a n after slash combine it as a single character
				{
					text = (text.substring(0,i)+'\n').concat(text.substring(i+2, text.length()));
					  
				}	
				else if(text.charAt(i+1) == 't' || text.charAt(i+1) == '\t')	//If it is a t after slash combine it as a single character
				{
					 text = (text.substring(0,i)+'\t').concat(text.substring(i+2, text.length()));

				}
				else if(text.charAt(i+1) == 'b' || text.charAt(i+1) == '\b')	//If it is b after slash combine it as a single character
                                {
                                         text = (text.substring(0,i)+'\b').concat(text.substring(i+2, text.length()));

                                }	
				else if(text.charAt(i+1) == 'f' || text.charAt(i+1) == '\f')	// If it is f after slash combine it as a single character
                                {
                                         text = (text.substring(0,i)+'\f').concat(text.substring(i+2, text.length()));
                                }
				else
				{
					text = text.substring(0,i).concat(text.substring(i+1, text.length()));	//for all others except above
				}
		
			}

		//	System.out.println("text is "+text);
		}
			 
			 setText(text);		//call set text with processed string
			
			if(text.length() > 1025) {		//Report string constant long error
				reportError("String constant too long");
				return 1;			// Return value helpful in distinguishing
			} 
			
		return 0;
	}

}
/*
	WRITE ALL LEXER RULES BELOW
*/

SEMICOLON   : ';';
DARROW      : '=>';
COMMA	    : ',';
PLUS	    : '+';
MINUS	    : '-';
STAR	    : '*';
TILDE	    : '~';
LT	    : '<';
LE	    : '<=';
DOT	    : '.';
EQUALS	    : '=';
COLON	    : ':';
IF	    : [iI][Ff];
FI	    : [fF][iI];
BOOL_CONST  : [t][rR][uU][eE] | [f][aA][lL][sS][eE];
INT_CONST   : [0-9]+;
STR_CONST   :  '"' (ALLOWEDA | ALLOWEDB)* ['"'] {processString();};
STR_UNTERM  :  '"' (ALLOWEDA | ALLOWEDB)* [\n] {
	int a = processString();
        if(a == 0) {	//Check the return value
	reportError("Unterminated string constant");
        }
};

STR_NULL    :  '"' (ALLOWEDA | ALLOWEDB)* [\u0000] (ALLOWEDA | ALLOWEDB)* '"'  {reportError("String contains null character");};
STR_EOF     :  '"' (ALLOWEDA | ALLOWEDB)*(.EOF){
	int a = processString();
	if(a == 0) {		// Check the return value 
	reportError("EOF in string");
	}
};

LBRACE	    : '{';
RBRACE	    : '}';
LPAREN	    : '(';
RPAREN	    : ')';
IN	    : [iI][nN];
ATSYM	    : '@';
CLASS	    : [cC][lL][aA][sS][sS];
INHERITS    : [iI][nN][hH][eE][rR][iI][tT][sS];
LET	    : [lL][eE][tT];
LOOP	    : [lL][oO][oO][pP];
POOL	    : [pP][oO][Oo][lL];
THEN	    : [tT][hH][eE][nN];
WHILE       : [wW][hH][iI][lL][eE];
CASE	    : [cC][aA][sS][eE];	
ESAC	    : [eE][sS][aA][cC];
WS 	    : [ \t\n\b\r\f]+ -> skip;
OF	    : [oO][fF];
NEW	    : [nN][eE][wW];
ISVOID	    : [iI][sS][vV][oO][iI][dD];
NOT	    : [nN][oO][tT];
LINECOMMENT : [-][-](~[\n])*[\n]? -> skip;	

OBJECTID        : LOWER(LETTER | DIGIT |'_')*;
TYPEID          : UPPER(LETTER | DIGIT | '_')*;
STR_EOF_OTHER     :  '"'(ALLOWEDA | ALLOWEDB)*{reportError("EOF in string");};

// Necessary fragments 
fragment DIGIT          : ('0'..'9');
fragment UPPER          : [A-Z];
fragment LOWER          : [a-z];
fragment LETTER         : (UPPER | LOWER);
fragment ALLOWEDA       : ~['\n' | '\u0000' | '\"' | '\\'];	// Anything apart from these is allowed in string constant
fragment ALLOWEDB       : ('\\'~[\u0000]);			// single slash is allowed iff it is followed by something other than null character
INVALIDTOKEN            : (.) {invalidToken();};                // If none of the above matches it is an invalid token                                                                                                                          

MUL_COM	    : '(*' -> pushMode(fir_level), skip;		//push it
UNHANDLED_COM : '*)' {reportError("Unmatched *)");}; 

mode fir_level;							//first level

EOF_HERE   :   .(.EOF) {reportError("EOF in comment");};	//report EOF
LEVEL_INCREMENT	: '(*' -> pushMode(sec_level), skip;		//Go to next level
END_CURRENT  : '*)' -> popMode, skip;				//Pop it
OTHER        : (.) -> skip;					//ANy other character consume

mode sec_level;							//Second level

SEC_EOF_HERE   :  .(.EOF) {reportError("EOF in comment");};	//report EOF
LEVEL_RECURSE : '(*' -> pushMode(sec_level), skip;		//recurse
EOF_UNHANDLED : '*)'.(.EOF) {reportError("EOF in comment");};	//since it is in level 2 we have to handle this
SEC_END_CURRENT  : '*)' -> popMode, skip;			// pop it
SEC_OTHER        : (.) -> skip;					//any other then consume it

