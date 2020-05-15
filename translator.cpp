#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;

/* INSTRUCTION:  copy your parser.cpp here
      cp ../ParserFiles/parser.cpp .
   Then, insert or append its contents into this file and edit.
   Complete all ** parts.
*/

//Scanner================================================================
//Words: (vowel | vowel n | consonant vowel | consonant vowel n | consonant-pair vowel | consonant-pair vowel n)^+              
//If it ends in a vowel lower case or n :	    WORD1
//If it ends in I | E :               		 	WORD2 
//Consonants: 		b, d, g, h, j, k, m, n, p, r, s, t, w, y, z   	(note that d,j,w,y,z do not start consonant pairs)
//Consonant pairs: 	by, gy, hy, ky, my, ny, py, ry, ch, sh, ts 		(note that c is not listed as a consonant)
//Vowels: 			a, i, u, e, o  									(but I and E uppercase are allowed)

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
//Scanner================================================================

//Parser=================================================================
//1  <story>         ::= <s> {<s>}  
//2  <s> 			 ::= [CONNECTOR #getEword# #gen(“CONNECTOR”)#] <noun> #getEword# SUBJECT #gen(“ACTOR”)# <after subject>
//3  <after subject> ::= <verb> #getEword# #gen(“ACTION”)# <tense> #gen(“TENSE”)# PEROD | <noun> #getEword# <after noun>
//4  <after_noun>    ::= <be> #gen(DESCRIPTION)# #gen(“TENSE”)# PERIOD | DESTINATION #gen(“TO”)# <verb> #getEword# #gen(“ACTION”)# <tense> #gen(“TENSE”)# PERIOD | OBJECT #gen(“OBJECT”)# <after_object>
//5  <after_object>  ::= <verb> #getEword# #gen(“ACTION”)# <tense> #gen(“TENSE”)# PERIOD | <noun> #getEword# DESTINATION #gen(“TO”)# <verb> #getEword# #gen(“ACTION”)# <tense> #gen(“TENSE”)# PERIOD
//6  <noun>          ::= WORD1 | PRONOUN 
//7  <verb>          ::= WORD2
//8  <be>            ::= IS | WAS
//9  <tense>         ::= VERBPAST | VERBPASTNEG | VERB | VERBNEG

//All non-terminals are in <>   				(use switch statement)
//Optional parts are in []   					(use if statement)
//Repeatable (zero or more) parts are in {}		(use while loop)

// ----- parser variables -----
tokentype saved_token; // saved token global variable
string saved_lexeme; //saved_lexme global variable
bool token_available = false; //set flag to false at first 
// ----- Parser functions -----
void syntaxerror1(tokentype token, string lexeme); // match fails
void syntaxerror2(tokentype token, string lexeme); // switch default in a parser function
tokentype next_token(); // next_token function
bool match(tokentype expected); // Match Function to compare tokens
// ----- RDP functions -----
void tense();//			::= VERBPAST | VERBPASTNEG | VERB | VERBNEG
void be();//			::= IS | WAS
void verb();//			::= WORD2
void noun();//			::= WORD1 | PRONOUN 
void after_object();//	::= <verb> #getEword# #gen(“ACTION”)# <tense> #gen(“TENSE”)# PERIOD | <noun> #getEword# DESTINATION #gen(“TO”)# <verb> #getEword# #gen(“ACTION”)# <tense> #gen(“TENSE”)# PERIOD
void after_noun();//	::= <be> #gen(DESCRIPTION)# #gen(“TENSE”)# PERIOD | DESTINATION #gen(“TO”)# <verb> #getEword# #gen(“ACTION”)# <tense> #gen(“TENSE”)# PERIOD | OBJECT #gen(“OBJECT”)# <after_object>
void after_subject();// ::= <verb> #getEword# #gen(“ACTION”)# <tense> #gen(“TENSE”)# PEROD | <noun> #getEword# <after noun>
void s();//             ::= [CONNECTOR #getEword# #gen(“CONNECTOR”)#] <noun> #getEword# SUBJECT #gen(“ACTOR”)# <after subject>
void story();//        	::= <s> {<s>}  
//Parser=================================================================


//=================================================
// File translator.cpp written by Group Number: 22
//=================================================

// ----- Additions to the parser.cpp ---------------------

// ** Declare Lexicon (i.e. dictionary) that will hold the content of lexicon.txt
// Make sure it is easy and fast to look up the translation.
// Do not change the format or content of lexicon.txt 
//  Done by: ** 
struct DictionaryEntry {
	string japanese;
	string english;
};
vector<DictionaryEntry> Dictionary;
string saved_E_word;
ofstream outFile;

// ** Additions to parser.cpp here:
//    getEword() - using the current saved_lexeme, look up the English word
//                 in Lexicon if it is there -- save the result   
//                 in saved_E_word
//  Done by: Vinh Pham 
void getEword()
{
	bool found = false;
	for(int count = 0; count < Dictionary.size(); count++)
	{
		if(Dictionary[count].japanese == saved_lexeme)
		{
			saved_E_word = Dictionary[count].english;
			found = true;
		}
    }
	if(found == false)
		{saved_E_word = saved_lexeme;}
}//end of getEword function

//    gen(line_type) - using the line type,
//                     sends a line of an IR to translated.txt
//                     (saved_E_word or saved_token is used)
//  Done by: Vinh Pham 
void gen(string line_type)
{
	if(line_type == "TENSE")
	{
		outFile << line_type << "  " << tokenName[saved_token] << endl;
	}
	else
	{
		outFile << line_type << "  " << saved_E_word << endl;
	}
}//end gen function

// ----- Changes to the parser.cpp content ---------------------

// ** Comment update: Be sure to put the corresponding grammar 
//    rule with semantic routine calls
//    above each non-terminal function 

// ** Each non-terminal function should be calling
//    getEword and/or gen now.


// ---------------- Driver ---------------------------

// The final test driver to start the translator
// Done by:  Waylin
int main()
{
	ifstream finLex;//stream for reading from lexicon.txt
	finLex.open("lexicon.txt");//** opens the lexicon.txt file and reads it into Lexicon
	while (true) 
	{
		string str;
		DictionaryEntry E;
		finLex >> str;
		E.japanese = str;
		finLex >> str;
		E.english = str;
		if(finLex.eof()) break;
		Dictionary.push_back(E);
	}
	finLex.close();//** closes lexicon.txt 
	
	//for (int i = 0; i < Dictionary.size(); i++)// USAGE DEMO, DELETE THIS
	//	cout << "japanese " << Dictionary[i].japanese << " english " << Dictionary[i].english << endl;// USAGE DEMO, DELETE THIS

	outFile.open ("translated.txt");//** opens the output file translated.txt

	cout << "Enter the input file name: ";//ask user for file name
	string filename;//variable to get file from user
	cin >> filename;//get user input filename
	fin.open(filename.c_str());//fstream input file data

	story();//** calls the <story> to start parsing

	fin.close(); //** closes the input file 
	outFile.close();//** closes traslated.txt
	return 0;//program completed successfully
}//  end
//** require no other input files!
//** syntax error EC requires producing errors.txt of error messages
//** tracing On/Off EC requires sending a flag to trace message output functions


//Parser=================================================================

// ----- Parser functions -----

// match fails
void syntaxerror1(tokentype token, string lexeme) 
{
	cout << "SYNTAX ERROR: Expected " << tokenName[token] << " but found " << lexeme << endl;
	saved_token = ERROR;
	saved_lexeme = "ERROR";
}

// switch default in a parser function
void syntaxerror2(tokentype token, string lexeme) 
{
	cout << "SYNTAX ERROR: Unexpected " << saved_lexeme << " found in " << lexeme << endl;
	saved_token = ERROR;
	saved_lexeme = "ERROR";
}

// next_token function
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

// Match Function to compare tokens
bool match(tokentype expected) 
{
	if(next_token() != expected)//mismatch has occurred with the next token
	{
		syntaxerror1(expected, saved_lexeme);//calls a syntax error function here to generate a syntax error message here and do recovery
		return false;
	}
	else// match has occurred
	{
		cout << "Found a Match! " << tokenName[expected] << endl;
		token_available = false;// match has occurred
		return true;// say there was a match
	}
}

// ----- RDP functions -----

// <tense> ::= VERBPAST | VERBPASTNEG | VERB | VERBNEG
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
			
		case ERROR: //if case is ERROR
			break;
			
		default: //throw an error
			syntaxerror2(saved_token, "tense");	
    }
}

// <be> ::= IS | WAS
void be()
{
	cout<<"Processing <be>"<<endl;
	
	gen("DESCRIPTION"); //
	
	switch(next_token())
	{
		case IS: //if case is "IS"
			match(IS); //matches "IS"
			break;
			
		case WAS: //if case is WAS
			match(WAS); //matches "WAS"
			break;
			
		case ERROR: //if case is ERROR
			break;
			
		default: //throw an error
			syntaxerror2(saved_token, "be");
	} 
}

// <verb> ::= WORD2
void verb() 
{
	cout<<"Processing <verb>"<<endl;
	//match(WORD2); //matches VERB
	switch(next_token())                            
	{
		case WORD2: //if case is WORD2
			match(WORD2); //match WORD2
			break;
	
		case ERROR: //if case is ERROR
			break;
			
		default: //throw an error
			syntaxerror2(saved_token, "verb");  
    }
} //end of verb function

// <noun> ::= WORD1 | PRONOUN 
void noun()
{ 
	switch(next_token())
	{
		case WORD1: //if case is WORD1
			match(WORD1); //matches WORD1
			break;
      
    	case PRONOUN: //if case is PRONOUN
			match(PRONOUN); //matches PRONOUN
			break;
				
		case ERROR: //if case is ERROR
			break;		
		  
		default: //throw an error
			syntaxerror2(saved_token, "noun");
	}
} 

// <after_object>  ::= <verb> #getEword# #gen(“ACTION”)# <tense> #gen(“TENSE”)# PERIOD | <noun> #getEword# DESTINATION #gen(“TO”)# <verb> #getEword# #gen(“ACTION”)# <tense> #gen(“TENSE”)# PERIOD
// Done by: Ian Altoveros
void after_object() 
{
	cout<<"Processing <afterObject>"<<endl;
	  
	switch(next_token())
	{
		case WORD2: //if case is WORD2
			verb();
			getEWord();
			gen("ACTION");
			tense();
			gen("TENSE");
			match(PERIOD); //matches PERIOD
			break;
			
		case WORD1: //if case is WORD1
			noun();
			getEWord();
			match(DESTINATION); //matches DESTINATION
			gen("TO");
			verb();
			getEWord();
			gen("ACTION");
			tense();
			gen("TENSE");
			match(PERIOD); //matches PERIOD
			break;
			
		case PRONOUN:
			noun();
			getEWord();
			match(DESTINATION); //matches DESTINATION
			gen("TO");
			verb();
			getEWord();
			gen("ACTION");
			tense();
			gen("TENSE");
			match(PERIOD); //matches PERIOD
			break;
			
		case ERROR: //if case is ERROR
			break;		
		  
		default: //throw an error
			syntaxerror2(saved_token, "after_object");
	}
}

// <after_noun> ::= <be> #gen(DESCRIPTION)# #gen(“TENSE”)# PERIOD | DESTINATION #gen(“TO”)# <verb> #getEword# #gen(“ACTION”)# <tense> #gen(“TENSE”)# PERIOD | OBJECT #gen(“OBJECT”)# <after_object>
// Done by: Ian Altoveros
void after_noun() 
{
	cout<<"Processing <afterNoun>"<<endl;
	
	switch(next_token())
	{
  
		case WAS: //if case is "WAS" OR "IS"
			be();
			gen("DESCRIPTION");
			gen("TENSE");
			match(PERIOD); //matches PERIOD
			break;
			
		case IS:
			be();
			gen("DESCRIPTION");
			gen("TENSE");
			match(PERIOD); //matches PERIOD
			break;
		
		case DESTINATION: //if case is DESTINATION
			match(DESTINATION); //matches DESTINATION
			gen("TO");
			verb();
			getEWord();
			gen("ACTION");
			tense();
			gen("TENSE");
			match(PERIOD); //matches PERIOD
			break;
		
		case OBJECT: //if case is OBJECT
			match(OBJECT); //matches OBJECT
			gen("OBJECT"); 
			after_object();
			
			break;
			
		case ERROR: //if case is ERROR
			break;		

		default: //throw an error
			syntaxerror2(saved_token, "after_noun");
	}
}

// <after subject> ::= <verb> #getEword# #gen(“ACTION”)# <tense> #gen(“TENSE”)# PEROID | <noun> #getEword# <after noun>
// Done by: Ian Altoveros
void after_subject() 
{
    switch(next_token())  // look ahead at next token                                          
      {
        case WORD2: //if case is WORD2
			verb();
		    	getEword();
		    	gen("ACTION");
			tense();
		    	gen("TENSE");
			match(PERIOD); //matches PERIOD
			break;
        
        case WORD1: //if case is WORD1
			noun();
		    	getEWord();
			after_noun();
			break;
		  
        case PRONOUN: //if case is PRONOUN
			noun();
		    	getEWord();
			after_noun();
			break;
		
		case ERROR: //if case is ERROR
			break;		

		default: //throw an error
			syntaxerror2(saved_token, "after_subject");  
      }
}

// <s> ::= [CONNECTOR #getEword# #gen(“CONNECTOR”)#] <noun> #getEword# SUBJECT #gen(“ACTOR”)# <after subject>
// Done By: Ian Altoveros
void s()
{ 
	cout << "Processing <s> " << saved_lexeme << endl;
	
	if(saved_token == ERROR) // if first word is ERROR, return
		{
			syntaxerror2(saved_token, "s");  // throw an error message
			return;
		}
		
	if(saved_token == CONNECTOR) // optional CONNECTOR
		{
		match(CONNECTOR); //matches CONNECTOR
		getEWord();
		gen("CONNECTOR");
		}
	noun();
	getEWord();
	if(saved_token == ERROR) // checks if we got ERROR in noun() before matching subject
		return;
	match(SUBJECT); //matches SUBJECT
	gen("ACTOR");
	after_subject();
}

// <story> ::= <s> {<s>} 
void story() 
{
	cout << "Processing <story>" << endl << endl;
	
	while (true) 
	{
		if(saved_token == ERROR) // if ERROR was found at last previous iteration, stop
			return; // unsuccessful parsing
		if(next_token() == EOFM) // get first word in sentence, if EOFM was found, stop
			break; // successful parsing
		else
			s();
	}
	
	cout << endl << "Successfully parsed <story> " << endl;
}

//Parser=================================================================


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
		a = PERIOD; // set a to PERIOD

	else if (w == "eofm") 
		a = EOFM;

	else //else error
		a = ERROR; // Generated a lexical error
		
  return 0;
}//end of int scanner

//scanner================================================================
