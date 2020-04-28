#include<iostream>
#include<fstream>
#include<string>

using namespace std;

//scanner================================================================
//tokentype types
enum tokentype { 
	WORD1, WORD2, PERIOD, ERROR, VERB, VERBNEG, VERBPAST, 
	VERBPASTNEG, IS, WAS, OBJECT, SUBJECT, DESTINATION, PRONOUN, 
	CONNECTOR, EOFM 
};
//word1, word2, in above enum                                                                                                                                                  
string tokenName[30] = { 
	"WORD1", "WORD2", "PERIOD", "ERROR", "VERB", "VERBNEG",
	"VERBPAST","VERBPASTNEG","IS","WAS", "OBJECT", "SUBJECT",
	"DESTINATION", "PRONOUN", "CONNECTOR", "EOFM"
};
//reserverd struct definition
struct reserved {
	const char* string;
	tokentype tokenT;
};
//reserved table including string and tokentypes
reserved reservedWords[] = {
	{"masu",VERB}, {"masen",VERBNEG}, {"mashita",VERBPAST},
	{"masendeshita",VERBPASTNEG}, {"desu",IS}, {"deshita",WAS},
	{"o", OBJECT}, {"wa", SUBJECT}, {"ni", DESTINATION},
	{"watashi",PRONOUN}, {"anata",PRONOUN}, {"kare",PRONOUN},
	{"kanojo", PRONOUN}, {"sore", PRONOUN},
	{"mata",CONNECTOR}, {"soshite", CONNECTOR},
	{"shikashi", CONNECTOR}, { "dakara", CONNECTOR}, {"eofm", EOFM}
};
//stream for reading from the input file
ifstream fin;  
//wordDFA/periodDFA/scanner function prototypes
bool word(string s);
bool period(string s);
int scanner(tokentype& a, string& w);
//scanner================================================================


//parser=================================================================
/* INSTRUCTION:  Complete all ** parts.
   You may use any method to connect this file to scanner.cpp
   that you had written.  
  e.g. You can copy scanner.cpp here by:
          cp ../ScannerFiles/scanner.cpp .  
       and then append the two files into one: 
          cat scanner.cpp parser.cpp > myparser.cpp
*/

//=================================================
// File parser.cpp written by Group Number: 22
//=================================================

// ----- Four Utility Functions and Globals -----------------------------------

// Purpose: *Updated Next Token w/ Global Variables*
// Done by: *Ian Altoveros*
// global VARIABLES
tokentype saved_token; // saved token global variable
string saved_lexeme; //saved_lexme global variable
bool token_available = false; //set flag to false at first 

// ** Need syntaxerror1 and syntaxerror2 functions (each takes 2 args)
//    to display syntax error messages as specified by me.  

// Type of error: *If the Match Fails*
// Done by: *Ian Altoveros* 
void syntaxerror1(tokentype token, string lexeme)
{
	/*
	 match fails => call syntax_error1 and generate
     "SYNTAX ERROR: expected token_type but found saved_lexeme   and halt. ** exit(1)
	e.g. SYNTAX ERROR: expected WORD2 but found asa
	*/
        saved_token = ERROR;
	saved_lexeme = "ERROR";
	cout << "SYNTAX ERROR: Expected " << token << " but found " << lexeme << endl;

}
// Type of error: *Switch Default *
// Done by: *Ian Altoveros* 
void syntaxerror2(string token, string lexeme) 
{
	/*
	switch default in a parser function => call syntax_error2 and generate
      "SYNTAX ERROR: unexpected saved_lexeme found in parser function" and halt. ** exit(1)
	e.g. SYNTAX ERROR: unexpected nai found in tense 
	*/
        saved_lexeme = "ERROR";
  	saved_token = ERROR;
	cout << "SYNTAX Error: unexpected " << lexeme << " found in " << token << endl;

}

// ** Need the updated match and next_token with 2 global vars
// saved_token and saved_lexeme

// Purpose: *next_token function*
// Done by: *Ian Altoveros*
tokentype next_token()
{
	if (!token_available) // if there is no saved token yet
	{
		scanner(saved_token, saved_lexeme); // call scanner to grab a new token
		cout<<"Scanner called using word: "<< saved_lexeme <<endl;
		token_available = true; // mark that fact that you have saved it
	}
	return saved_token;// return the saved token
}

// Purpose: *Match Function to compare tokens*
// Done by: *Ian Altoveros*
bool match(tokentype expected) 
{
	if(next_token() != expected)//mismatch has occurred with the next token
	{
		syntaxerror1(saved_token, saved_lexeme);//calls a syntax error function here to generate a syntax error message here and do recovery
		return false;
	}
	else// match has occurred
	{
		cout << "Found a Match! " << tokenName[expected] << endl;
		token_available = false;// match has occurred
		return true;// say there was a match
	}
}


// ----- RDP functions - one per non-term --------------------------------
// ** Make each non-terminal into a function here
// ** Be sure to put the corresponding grammar rule above each function
// ** Be sure to put the name of the programmer above each function

//1  <story>         ::= <s> {<s>}  
//2  <s>             ::= [CONNECTOR] <noun> SUBJECT <after_subject>
//3  <after_subject> ::= <verb> <tense> PEROD | <noun> <after_noun>
//4  <after_noun>    ::= <be> PERIOD | DESTINATION <verb> <tense> PERIOD | OBJECT <after_object>
//5  <after_object>  ::= <verb> <tense> PERIOD | <noun> DESTINATION <verb> tense> PERIOD
//6  <noun>          ::= WORD1 | PRONOUN 
//7  <verb>          ::= WORD2
//8  <be>            ::= IS | WAS
//9  <tense>         ::= VERBPAST | VERBPASTNEG | VERB | VERBNEG

//All non-terminals are in <>   (use switch statement)
//Optional parts are in []   (use if statement)
//Repeatable (zero or more) parts are in {}   (use while loop)

 // <tense> ::= VERBPAST | VERBPASTNEG | VERB | VERBNEG
 // Done by: Ian Altoveros

void tense()
{
  cout<<"Processing <tense>"<<endl;

	switch(next_token())
	{
		case VERBPAST: //if case is VERBPAST
			match(VERBPAST); //matches VERBPAST
			break;
			
		case VERBPASTNEG: //if case is VERBPASTNEG
			match(VERBPASTNEG); //matches VERBPASTNEG
			break;
			
		case VERB: //if case is VERB
			match(VERB); //matches VERB
			break;
			
		case VERBNEG: //if case is VERBNEG
			match(VERBNEG); //matches VERBNEG
			break;
			
		default:
			syntaxerror2(saved_lexeme,"tense");
  }
}

// Grammar: <be> ::= IS | WAS
// Done by: Ian Altoveros
void be()
{
  cout<<"Processing <be>"<<endl;

	switch(next_token())
	{
		case IS: //if case is "IS"
			match(IS); //matches "IS"
			break;
			
		case WAS: //if case is WAS
			match(WAS); //matches "WAS"
			break;
			
		default:
			syntaxerror2(saved_lexeme,"be");
} 
}

// Grammar: <verb> ::= WORD2
// Done by:Vinh Pham 
void verb() 
{
  cout<<"Processing <verb>"<<endl;
  //match(WORD2); //matches VERB
  switch(next_token())                                                              
    {
    case  WORD2: //condition case
      match(WORD2); //match WORD2
      break;
    default: 
      syntaxerror2(saved_lexeme, "verb");  
    }
} //end of verb function


// Grammar: <noun> ::= WORD1 | PRONOUN 
// Done by: Ian Altoveros
void noun()
{ 
  cout<<"Processing <noun>"<<endl;

	switch(next_token())
	{
		case WORD1: //if case is WORD1
			match(WORD1); //matches WORD1
			break;
      
    		case PRONOUN: //if case is PRONOUN
			match(PRONOUN); //matches PRONOUN
			break;
      
		default:
			syntaxerror2(saved_lexeme,"noun");
	}	
} // end of noun function



// Grammar: <after_object> ::= <verb> <tense> PERIOD | <noun> DESTINATION <verb> tense> PERIOD
// Done by: Ian Altoveros
void after_object() 
{
  cout<<"Processing <afterObject>"<<endl;

	switch(next_token())
	{
		case WORD2: //if case is WORD2
			verb(); 
			tense(); 
			match(PERIOD); //matches PERIOD
			break;
			
		case WORD1: //if case is WORD1 OR PRONOUN
			
		case PRONOUN:
			noun();
			match(DESTINATION); //matches DESTINATION
			verb();
			tense();
			match(PERIOD); //matches PERIOD
			break;
			
		default:
			syntaxerror2(saved_lexeme,"after_object");
	}
}

// Grammar: <after_noun> ::= <be> PERIOD | DESTINATION <verb> <tense> PERIOD | OBJECT <after_object>
// Done by: Ian Altoveros
void after_noun() 
{
  cout<<"Processing <afterNoun>"<<endl;

  switch(next_token())
  {
  
	case WAS: //if case is "WAS" OR "IS"
	case IS:
		be();
		match(PERIOD); //matches PERIOD
		break;
	
	case DESTINATION: //if case is DESTINATION
		match(DESTINATION); //matches DESTINATION
		verb();
		tense();
		match(PERIOD); //matches PERIOD
		break;
	
	case OBJECT: //if case is OBJECT
		match(OBJECT); //matches OBJECT
		after_object();
		break;
	
	default:
		syntaxerror2(saved_lexeme,"after_noun");
	
}
}



// Grammar: <after_subject> ::= <verb> <tense> PEROID | <noun> <after_noun>
// Done by: Vinh Pham
void after_subject() 
{
  cout<<"Processing <afterSubject>"<<endl;

    switch(next_token())                                            
      {
        case WORD2: //if case is WORD2
        verb();
        tense();
        match(PERIOD); //matches PERIOD
        break;
        
        case WORD1: //if case is WORD1
        noun();
        after_noun();
        break;
      
        case PRONOUN: //if case is PRONOUN
        noun();
        after_noun();
        break;
        
        default:
            syntaxerror2(saved_lexeme,"after_subject");
      }
}

// Grammar: <s> ::= [CONNECTOR] <noun> SUBJECT <after_subject>
// Done by: Waylin
void s()
{ 
  /*
	cout << "Processing <s> " << saved_lexeme << endl;
	if(next_token() == CONNECTOR)
		match(CONNECTOR);
	noun();
	match(SUBJECT);
	after_subject();
  */

  //cout<<"Processing <s>"<<endl;
	next_token();
	if(saved_lexeme != "eofm") //condition if saved_lexeme not match
	  {
	    cout<<"Processing <s>"<<endl; //processing s
	    if(next_token() == CONNECTOR) // if token match with the CONNECTOR
	      {
		match(CONNECTOR); //match connector
	      }
  
	    noun(); //call noun
	    match(SUBJECT); //match subject
	    after_subject(); 
	  }
	
	//token_available = false;// delete this, used just to stop loop for testing
	
}

// Grammar: <story> ::= <s> {<s>} 
// Done by: Waylin
void story() 
{
	cout << "Processing <story>" << endl << endl;
	bool loop = true;
	/*
	while (loop)
	{
		switch(saved_token)
		{ 
		        case ERROR:
			    return;
			case EOFM:
				loop = false;
				break;
			default:
				s(); 
		}
	}
	*/	

	s(); //calling s() function
	while(loop && (saved_lexeme != "eofm"))
	  {
	    s();
	  }
	cout << endl << "Successfully parsed <story> " << endl;
}


//----------- Driver ---------------------------
// The new test driver to start the parser
// Done by:  Waylin
int main()
{
	string filename;//variable to get file from user
	cout << "Enter the input file name: ";//ask user for file name
	cin >> filename;//get user input filename
	fin.open(filename.c_str());//fstream input file data

	story();//** calls the <story> to start parsing

	fin.close();//** closes the input file 
	return 0;//returned successfully
}// end
//** require no other input files!
//** syntax error EC requires producing errors.txt of error messages
//** tracing On/Off EC requires sending a flag to trace message output functions


//scanner================================================================
//wordDFA/periodDFA/Scanner function definitions  
bool word (string s)
{
  //initialize state variables & char posistion    
  int state =0;
  int charpos = 0;
    
  //    DFA TABLE
  //    STATE #     |     state
  //      0         |       q0 starting state
  //      1         |       q0q1
  //      2         |       vowels
  //      3         |       constants
  //      4         |       's'
  //      5         |       't'
  //      6         |       'c'

   while (s[charpos] != '\0')
    {
      //State 0 when Q0 is going toward qy pair of (b|m|k|h|p|r|g)  --> constants                                                                                                                                                                                   
      if (state == 0 && (s[charpos] == 'b' || s[charpos] == 'g' || s[charpos] == 'h' 
                                            || s[charpos] == 'k' || s[charpos] == 'm' 
                                            || s[charpos] == 'n' || s[charpos] == 'p' 
                                            || s[charpos] == 'r') )
        {
            state = 1; //set state equal to 1
        }
      //state 0 when q0 is going toward q0q1 vowel --> q0q1
      else if(state == 0 && (s[charpos] == 'a' || s[charpos] == 'i' || s[charpos] == 'u' 
                                                || s[charpos] == 'e' || s[charpos] == 'o' 
                                                || s[charpos] == 'I' || s[charpos] == 'E'))
        { 
            state = 2; //set state equal to 2
        }
      //State 0 when q0 is going toward qsa pair of (d|w|z|y|j) --> qsa 
      else if(state == 0 && (s[charpos] == 'd' || s[charpos] == 'j' || s[charpos] == 'w' 
                                                || s[charpos] == 'y' || s[charpos] == 'z'))
        { 
            state = 3; //set state equal to 3
        }
      //State 0 when q0 is going toward qs   
      else if(state == 0 && s[charpos] == 's')
        { 
            state = 4;//set state equal to 4
        }
      //State 0 when q0 is going toward qt   
	  else if(state == 0 && s[charpos] == 't')
		{ 
		    state = 5;//set state equal to 5
		}
	   //State 0 when q0 is going toward qc 	
	  else if(state == 0 && s[charpos] == 'c')
	    { 
	        state = 6;//set state equal to 6
	    }
	  else
	     { 
	      // State 1 when is going toward q0                                                                                                                                                                                     
		  if (state == 1 && (s[charpos] == 'a' || s[charpos] == 'i' || s[charpos] == 'u' 
		                                        || s[charpos] == 'e' || s[charpos] == 'o' 
		                                        || s[charpos] == 'I' || s[charpos] == 'E')) 
		                                        //When in state 2 and the following characters are vowels
		  {
		      state = 2;//  Set state equal to 2 -> vowels
		  }
		  else if(state == 1 && s[charpos] == 'y') //If we're in state 1and the following character is 'y'
		  { 
		      state = 3; // Set state equal to 3 -> constants
		  }
		  else
		    { 
		      //If we're in state 2 and the following character is 'n'                                                                                                                                                                                        
              if(state == 2 && s[charpos] == 'n')
				{ 
				    state = 0; //set state equal to 0
				}
	      else if (state == 2 && (s[charpos] == 'b' || s[charpos] == 'g' || s[charpos] == 'h' 
     		                                        || s[charpos] == 'k' || s[charpos] == 'm' 
				                        || s[charpos] == 'n' || s[charpos] == 'p' 
				                        || s[charpos] == 'r') ) 
     		                                        //When in state 2 and the following characters are constants
				{
				    state = 3; // Set state equal to 3 -> constants
				}
		      else if(state == 2 && (s[charpos] == 'a' || s[charpos] == 'i' || s[charpos] == 'u' 
		                                                || s[charpos] == 'e' || s[charpos] == 'o' 
		                                                || s[charpos] == 'I' || s[charpos] == 'E')) 
		                                                //When in state 2 and the following characters are vowels
				{ 
				    state = 2; //  Set state equal to 2 -> vowels
				}
		      else if(state == 2 && (s[charpos] == 'd' || s[charpos] == 'j' || s[charpos] == 'w' 
		                                                || s[charpos] == 'y' || s[charpos] == 'z')) 
		                                                //When in state 2and the following characters are constants
				{ 
				    state = 3; // Set state equal to 3 -> constants
				}
				
		      else if(state == 2 && s[charpos] == 's') //If we're in state 2 and the following character is 's'
				{ 
				    state = 4; //Set state to 4 -> 's'
				}
				
			  else if(state == 2 && s[charpos] == 't') //If we're in state 2 and the following character is 't'
				{ 
				    state = 5; //Set state to 5 -> 't'
				}
				
			  else if(state == 2 && s[charpos] == 'c') //If we're in state 2 and the following character is 'c'
			    { 
			        state = 6; //Set state to 6 -> 'c'
			    }
			    
			  else
				{ 
				  //State 3 is reached when the character is a constant
				  if(state == 3 && (s[charpos] == 'a' || s[charpos] == 'i' || s[charpos] == 'u' 
				                                        || s[charpos] == 'e' || s[charpos] == 'o' 
				                                        || s[charpos] == 'I' || s[charpos] == 'E')) 
				                                        //When in state 3 and the following characters are vowels
				    { 
				        state = 2; //  Set state equal to 2 -> vowels
				    }
				    
				  else
				    {
				      //State 4 when char is 's'  
				      if(state == 4 && (s[charpos] == 'a' || s[charpos] == 'i' || s[charpos] == 'u' 
				                                        || s[charpos] == 'e' || s[charpos] == 'o' 
				                                        || s[charpos] == 'I' || s[charpos] == 'E')) 
				                                        //When in state 4 and the following characters are vowels 
					  { 
					      state = 2;  //  Set state equal to 2 -> vowels
					  }
					  
					  //State 4 is reached when char is 's'
				      else if(state == 4 && s[charpos] == 'h') 
				      //When in state 4 and the following character is 'h'
					  { 
					      state = 3; // Set state equal to 3 -> constants
					  }
					  
					  else
					  { 
					    //State 5 is reached when the current character is 't'                                                                                                                                                                   
					    if(state == 5 && (s[charpos] == 'a' || s[charpos] == 'i' || s[charpos] == 'u' 
					                                        || s[charpos] == 'e' || s[charpos] == 'o' 
					                                        || s[charpos] == 'I' || s[charpos] == 'E')) 
					                                        //When in state 5 and the following characters are vowels
					      {  
					          state = 2; //  Set state equal to 2 -> vowels
					      }
					    
					    //State 5 is reached when the current character is 't'
					    else if(state == 5 && s[charpos] == 's') // When in state 5 and the following character is 'h'
					      { 
					          state = 3;    // Set state equal to 3 -> constants
					      }
					      
					    else
						  { //State 6 is reached when the current character is 'c'                                                                                                                                                   
							if(state == 6 && s[charpos] == 'h')  //When in state 6 and the following character is 'h'
						      { 
						          state = 3; // Set the state equal to 3 -> constants
						      }
						      
						    else
						        { 
							        return false;
						        } // end else
						  }//end else
					  }
				  }
			    }
		    }
	     }
	charpos++; // update the char posistion
    }//end of while - loop                                                                                                                                                                                           

	// where did I end up????                                                                                                                                                                                   
  if (state == 0 || state == 2) //if state is in '0' or '2' returns true
  {
	  return true;
  }
  return false;
}//end of bool word DFA 

bool period (string s)
{  // complete this **
  
  //initialize variables
  int state = 0;
  int charpos = 0;

  while (s[charpos] != '\0') //while not empty
    {
      //if the char is a period and there is no next char
      if (state == 0 && s[charpos] == '.') //if state is 0 and char is  '.'
      {
          state = 1; //then state = 1
      }
        else
      {
          return false;
      }
      ++charpos;//update the char posistion  each time
    }//end of while loop
  
  if(state == 1 ) //if state = 1 then it found a period and returns true
      {
        return true; 
      }
      else 
    {
       return false; // return error/false when nothing is found
     }
}//end of bool period DFA

int scanner(tokentype& a, string& w)
{
	// ** Grab the next word from the file via fin
	fin >> w;

  /* 
  2. Call the token functions one after another (if-then-else)
     And generate a lexical error message if both DFAs failed.
     Let the token_type be ERROR in that case.
  3. Make sure WORDs are checked against the reservedwords list
     If not reserved, token_type is WORD1 or WORD2.
  4. Return the token type & string  (pass by reference)
  */

	int rowCount = sizeof reservedWords/ sizeof reservedWords[0];
	//cout<<"Row Count"<< rowCount <<endl;

	if(word(w)) //checking step 2, part 1
	  {
      bool isReserved = false;
	    for (int i = 0; i < rowCount; i++)
		{
		  if (w == reservedWords[i].string) //checking step 3, part 1
			{
			    //cout << "Word is reservedWords.\n";
				//cout << "token_type: " << reservedWords[i][1] << endl;
				a = reservedWords[i].tokenT;
				isReserved = true;
				return 0;
			}
		}
	//condition if is not reserved    
	if (!isReserved) 
	{		//condition if is "I" or "E"
			if (w[w.length() - 1] == 'I' || w[w.length() - 1] == 'E')
				a = WORD2; //set a to WORD2
			else //else change
				a = WORD1; //set a to WORD1
		}
	}
	else if(period(w)) //else if words is not reserved PERIOD
	{
		a = PERIOD; // set a to PERIOD
	}
	else if (w == "eofm") 
	{
		a = EOFM;
	}
	else //else error
	{
			// Generated a lexical error
			cout << "ERROR!! " << w << " IS NOT VALID!" << endl;
			a = ERROR;
	}
  return 0;
}



//end of int scanner

//scanner================================================================
