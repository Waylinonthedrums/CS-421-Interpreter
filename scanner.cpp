#include<iostream>
#include<fstream>
#include<string>
using namespace std;

/* Look for all **'s and complete them */

//=====================================================
// File scanner.cpp written by: Group Number: 22 
//=====================================================

// --------- Two DFAs ---------------------------------

// WORD DFA 
// Done by: **
// RE:   **
bool word(string s)
{
	/*
	int state = 0;
	int charpos = 0;
	// replace the following to do the word dfa  **
	
	while (s[charpos] != '\0') 
	{
		if (state == 0 && s[charpos] == 'a')
			state = 1;
		else
		if (state == 1 && s[charpos] == 'b')
			state = 2;
		else
		if (state == 2 && s[charpos] == 'b')
			state = 2;
		else
			return(false);
		charpos++;
	}//end of while
	// where did I end up????
	if (state == 2) return(true);  // end in a final state
	else return(false);
	*/
	
	return true; // just to test scanner function
}

// PERIOD DFA 
// Done by: **
bool period(string s)
{  // complete this **

	// state?
	if (s == ".")
		return true;
	else 
		return false;
}

// ------ Three  Tables -------------------------------------

// TABLES Done by: **

// ** Update the tokentype to be WORD1, WORD2, PERIOD, ERROR, EOFM, etc.
enum tokentype {
	WORD1, WORD2, PERIOD, ERROR, EOFM, // standard tokens
	
	VERB,        VERBNEG, VERBPAST, VERBPASTNEG, // reserved tokens
	IS,          WAS,     OBJECT,   SUBJECT, 
	DESTINATION, PRONOUN, CONNECTOR
};

// ** For the display names of tokens - must be in the same order as the tokentype.
string tokenName[30] = {
	"WORD1", "WORD2", "PERIOD", "ERROR", "EOFM", // standard tokens
	
	"VERB",        "VERBNEG", "VERBPAST", "VERBPASTNEG", // reserved tokens
	"IS",          "WAS",     "OBJECT",   "SUBJECT", 
	"DESTINATION", "PRONOUN", "CONNECTOR"
}; 

// ** Need the reservedwords table to be set up here. 
// ** Do not require any file input for this. Hard code the table.
// ** a.out should work without any additional files.
const unsigned int total_res_words = 18; // global number of reserved words
struct reserved{ // associate each reserved word with their respected token type 
	string st;
	tokentype tt;
};
reserved reservedWords[total_res_words] = { // list of reserved words with token type
	{"masu", VERB},                {"masen", VERBNEG},      {"mashita", VERBPAST}, 
	{"masendeshita", VERBPASTNEG}, {"desu", IS},            {"deshita", WAS}, 
	{"o", OBJECT},                 {"wa", SUBJECT},         {"ni", DESTINATION},      
	{"watashi", PRONOUN},          {"anata", PRONOUN},      {"kare", PRONOUN}, 
	{"kanojo", PRONOUN},           {"sore", PRONOUN},       {"mata", CONNECTOR},   
	{"soshite", CONNECTOR},        {"shikashi", CONNECTOR}, {"dakara", CONNECTOR}
};


// ------------ Scanner and Driver ----------------------- 

ifstream fin;  // global stream for reading from the input file

// Scanner processes only one word each time it is called
// Gives back the token type and the word itself
// ** Done by: 
int scanner(tokentype& tt, string& w)
{
	// ** Grab the next word from the file via fin
	string str;
	fin >> str;
	// 1. If it is eofm, return right now.   
	if (str == "eofm")
	{
		tt = EOFM;
		w = str;
		return 0; //4. Return the token type & string  (pass by reference)
	}
	//2. Call the token functions (word and period) 
	//one after another (if-then-else).
	else if(word(str))
	{
		//3. If it was a word,
		//check against the reservedwords list.
		for (unsigned int i = 0; i < total_res_words; i++)
		{
			if(str == reservedWords[i].st)
			{
				tt = reservedWords[i].tt;
				w = str;
				return 0; //4. Return the token type & string  (pass by reference)
			}
		}
		//If not reserved, tokentype is WORD1 or WORD2
		//decided based on the last character.
		if (
		str[str.size()-1] == 'a' || str[str.size()-1] == 'i' ||
		str[str.size()-1] == 'u' || str[str.size()-1] == 'e' ||
		str[str.size()-1] == 'o' || str[str.size()-1] == 'n' )
		{
			tt = WORD1;
			w = str;
			return 0; //4. Return the token type & string  (pass by reference)
		}
		else if (str[str.size()-1] == 'I' || str[str.size()-1] == 'E')
		{
			tt = WORD2;
			w = str;
			return 0; //4. Return the token type & string  (pass by reference)
		}
	}
	else if (period(str))
	{
		tt = PERIOD;
		w = str;
		return 0; //4. Return the token type & string  (pass by reference)
	}
	else 
	{
		//Generate a lexical error message if both DFAs failed.
		cout << "Lexical error: " << str << " is not a valid token" << endl;
		tt = ERROR; //Let the tokentype be ERROR in that case.
		w = str;
		return -1; //4. Return the token type & string  (pass by reference)
	}
}//the end of scanner


// The temporary test driver to just call the scanner repeatedly  
// This will go away after this assignment
// DO NOT CHANGE THIS!!!!!! 
// Done by:  Louis
int main()
{
  tokentype thetype;
  string theword; 
  string filename;

  cout << "Enter the input file name: ";
  cin >> filename;

  fin.open(filename.c_str());

  // the loop continues until eofm is returned.
   while (true)
    {
       scanner(thetype, theword);  // call the scanner which sets
                                   // the arguments  
       if (theword == "eofm") break;  // stop now

       cout << "Type is:" << tokenName[thetype] << endl;
       cout << "Word is:" << theword << endl;   
    }

   cout << "End of file is encountered." << endl;
   fin.close();

	return 0;
}// end

