#include<iostream>
#include<fstream>
#include<string>
#include "scanner.cpp"
#include <stdlib.h>
#include <vector>
using namespace std;

/* INSTRUCTION:  copy your parser.cpp here
      cp ../ParserFiles/parser.cpp .
   Then, insert or append its contents into this file and edit.
   Complete all ** parts.
*/

//=================================================
// File translator.cpp written by Group Number: 22
//=================================================

// ----- Additions to the parser.cpp ---------------------

// ** Declare Lexicon (i.e. dictionary) that will hold the content of lexicon.txt
// Make sure it is easy and fast to look up the translation.
// Do not change the format or content of lexicon.txt 
//  Done by: ** 


// ** Additions to parser.cpp here:
//    getEword() - using the current saved_lexeme, look up the English word
//                 in Lexicon if it is there -- save the result   
//                 in saved_E_word
void getEword()
{
  bool found = false;
  for(int count =0;count < wordJapan.size(); count++)
    {
      if(wordJapan[count]==saved_lexeme)
	{
	  savedEnglishWord = wordEnglish[count];
	  found = true;
	}
    }
  if(found == false)
    {savedEnglishword = saved_lexeme;}
}//end of getEword function



//  Done by: Vinh Pham 
//    gen(line_type) - using the line type,
//                     sends a line of an IR to translated.txt
//                     (saved_E_word or saved_token is used)
void gen(string line_type)
{
  if(line_type == "TENSE")
    {
      outFile<< line_type<< "  " << tokenName[saved_token] << endl;
    }
  else
    {
      outFile<< line_type<< "  "<< savedEword <<endl;
    }
}//end gen function






//  Done by: ** 

// ----- Changes to the parser.cpp content ---------------------

// ** Comment update: Be sure to put the corresponding grammar 
//    rule with semantic routine calls
//    above each non-terminal function 

// ** Each non-terminal function should be calling
//    getEword and/or gen now.

// Type of error: *If the Match Fails*
// Done by: *Ian Altoveros* 
void syntaxerror1(tokentype token, string lexeme)
{
  /*
     match fails => call syntax_error1 and generate
     "SYNTAX ERROR: expected token_type but found saved_lexeme   and halt. ** exit(1)
     e.g. SYNTAX ERROR: expected WORD2 but found asa
  */
  
  cout << "SYNTAX ERROR: Expected " << tokenName[token] << " but found " << lexeme << endl;
  saved_token = ERROR;
  saved_lexeme = "ERROR";
}
// Type of error: *Switch Default *
// Done by: *Ian Altoveros* 
void syntaxerror2(tokentype token, string lexeme) 
{
  /*
    switch default in a parser function => call syntax_error2 and generate
      "SYNTAX ERROR: unexpected saved_lexeme found in parser function" and halt. ** exit(1)
      e.g. SYNTAX ERROR: unexpected nai found in tense 
  */
  
  cout << "SYNTAX ERROR: Unexpected " << saved_lexeme << " found in " << lexeme << endl;
  saved_token = ERROR;
  saved_lexeme = "ERROR";
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
      
    case ERROR: //if case is ERROR
      break;
      
    default: //throw an error
      syntaxerror2(saved_token, "tense");
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
      
    case ERROR: //if case is ERROR
      break;
      
    default: //throw an error
      syntaxerror2(saved_token, "be");
    } 
}

// Grammar: <verb> ::= WORD2
// Done by:Ian Altoveros 
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
      return;
      
    default: //throw an error
      syntaxerror2(saved_token, "verb");  
    }
} //end of verb function


// Grammar: <noun> ::= WORD1 | PRONOUN 
// Done by: Ian Altoveros
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
      return;
        
    default: //throw an error
      syntaxerror2(saved_token, "noun");
    }
} 

// Grammar: <after_object> ::= <verb> <tense> PERIOD | <noun> DESTINATION <verb> <tense> PERIOD
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
      
    case WORD1: //if case is WORD1
      noun();
      match(DESTINATION); //matches DESTINATION
      verb();
      tense();
      match(PERIOD); //matches PERIOD
      break;
      
    case PRONOUN:
      noun();
      match(DESTINATION); //matches DESTINATION
      verb();
      tense();
      match(PERIOD); //matches PERIOD
      break;
      
    case ERROR: //if case is ERROR
      return;
        
    default: //throw an error
      syntaxerror2(saved_token, "after_object");
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
      be();
      match(PERIOD); //matches PERIOD
      break;
      
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
      
    case ERROR: //if case is ERROR
      return;

    default: //throw an error
      syntaxerror2(saved_token, "after_noun");
    }
}

// Grammar: <after_subject> ::= <verb> <tense> PEROID | <noun> <after_noun>
// Done by: Vinh Pham
void after_subject() 
{
  switch(next_token())  // look ahead at next token                                          
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
      
    case ERROR: //if case is ERROR
      return;

    default: //throw an error
      syntaxerror2(saved_token, "after_subject");  
    }
}

// Grammar: <s> ::= [CONNECTOR] <noun> SUBJECT <after_subject>
// Done by: Waylin
void s()
{ 
  cout << "Processing <s> " << saved_lexeme << endl;
  
  if(saved_token == ERROR) // if first word is ERROR, return
    {
      syntaxerror2(saved_token, "s");  // throw an error message
      return;
    }
  
  if(saved_token == CONNECTOR) // optional CONNECTOR
    match(CONNECTOR); //matches CONNECTOR
  noun();
  if(saved_token == ERROR) // checks if we got ERROR in noun() before matching subject
    return;
  match(SUBJECT); //matches SUBJECT
  after_subject();
}

// Grammar: <story> ::= <s> {<s>} 
// Done by: Waylin
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


// ---------------- Driver ---------------------------

// The final test driver to start the translator
// Done by:  **
int main()
{
  //** opens the lexicon.txt file and reads it into Lexicon
  //** closes lexicon.txt 


  //** opens the output file translated.txt

  cout << "Enter the input file name: ";
  cin >> filename;
  fin.open(filename.c_str());

  //** calls the <story> to start parsing

  //** closes the input file 
  //** closes traslated.txt
 
}// end
//** require no other input files!
//** syntax error EC requires producing errors.txt of error messages
//** tracing On/Off EC requires sending a flag to trace message output functions

