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
// Done by: Vinh Pham 
// RE:   **
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
}//end of bool word                                                                                                                                                                                           

// ** Done by: Ian Joseph Altoveros
//fucntion period with string s 
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
}
// ------ Three  Tables -------------------------------------
//Done by: Ian Joseph Altoveros
// -----  Tables -------------------------------------  
// ** Update the tokentype to be WORD1, WORD2, PERIOD, ERROR, etc.                                                                                                             
// Feel free to add a tokentype for the end-of-file marker.                                                                                                                    
enum tokentype { WORD1, WORD2, PERIOD, ERROR, VERB, VERBNEG, VERBPAST, VERBPASTNEG, IS, WAS, OBJECT, SUBJECT, DESTINATION, PRONOUN, CONNECTOR, EOFM, };
//word1, word2, in above enum                                                                                                                                                  
string tokenName[30] = { "WORD1", "WORD2", "PERIOD", "ERROR", "VERB", "VERBNEG","VERBPAST","VERBPASTNEG","IS","WAS", "OBJECT", "SUBJECT","DESTINATION", "PRONOUN", "CONNECTOR", "EOFM", };

// ** Need the reservedwords table to be set up here.                                                                                                                          
// ** Do not require any file input for this.                                                                                                                                  
// ** a.out should work without any additional files.                                                                                                                          
struct reserved
{
  const char* string;
  tokentype tokenT;
}

//reserved table including string and tokentypes

reserved[] = {
  { "masu",VERB},
  {"masen",VERBNEG},
  {"mashita",VERBPAST},
  {"masendeshita",VERBPASTNEG},
  {"desu",IS},
  {"deshita",WAS},
  {"o", OBJECT},
  {"wa", SUBJECT},
  {"ni", DESTINATION},
  {"watashi",PRONOUN},
  {"anata",PRONOUN},
  {"kare",PRONOUN},
  {"kanojo", PRONOUN},
  {"sore", PRONOUN},
  {"mata",CONNECTOR},
  {"soshite", CONNECTOR},
  {"shikashi", CONNECTOR},
  { "dakara", CONNECTOR},
  {"eofm", EOFM}
};
// ** Do not require any file input for this. Hard code the table.
// ** a.out should work without any additional files.

// ------------ Scaner and Driver ----------------------- 

ifstream fin;  // global stream for reading from the input file

// Scanner processes only one word each time it is called
// Gives back the token type and the word itself
// ** Done by: Waylin Owen /Vinh Pham / Ian Joseph Altoveros 
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
     


	int rowCount = sizeof reserved/ sizeof reserved[0];
	//cout<<"Row Count"<< rowCount <<endl;

        cout<<"\n";

	if(word(w)) //checking step 2, part 1
	  {
      bool isReserved = false;
	    for (int i = 0; i < rowCount; i++)
		{
		  if (w == reserved[i].string) //checking step 3, part 1
			{
			        //cout << "Word is reserved.\n";
				//cout << "token_type: " << reserved[i][1] << endl;
				a = reserved[i].tokenT;
        isReserved = true;
				return 0;
			}
		}
	    
	if (!isReserved) 
	{
			if (w[w.length() - 1] == 'I' || w[w.length() - 1] == 'E')
				a = WORD2;
			else
				a = WORD1;
		}
	}
	else if(period(w)) {
		a = PERIOD;
	}
	else if (w == "eofm") 
	{
		a = EOFM;
	}
	else {
			// Generated a lexical error
			cout << "ERROR!! " << w << " IS NOT VALID!" << endl;
			a = ERROR;
	}

	return 0;
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
