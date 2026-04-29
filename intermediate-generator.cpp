#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "C:\Users\Daniel\Downloads\Compiler-Project-main\Compiler-Project-main\lexical-analyzer.cpp"
// replace directory with location of lexical-analyzer document when executing 

//input is going to be the output of the lexical analyzer, which should just be
vector<string> keywords = {"def", "return", "if", "else", "elif", "while", "for", "class", "True", "False", "None"};
vector<string> punctuations = {"(", ")", "{", "}", "[", "]", ",", ":", ";", "."};
vector<string> operators = {"+", "-", "*", "/", "=", "==", "!=", "<", ">", "<=", ">=", "**", "//", "%", "and", "or", "not"};
vector<string> print_statements = {"print"};
//replaced vector definitions for key stuff, cause for some reason i couldn't get it to find the definitions in previous files . Same for token class definition despite referring to previious files 

class Token {
public:
    string type;
    string value;
    Token(const string& type, const string& value) : type(type), value(value) {}
};
vector<Token> token_convert(vector<Token> line_tokens) // input is the same type as the output from lexical-analyzer.cpp. 
//value is the ACTUAL word, while type is just what kind of token it is 
  { 
    vector<Token> tokens; //  new token dynamic array to hold converted values, then return at the end of the function 
    int i = 0; //counter 
    int tcounter = 0; // for  t registers in assembly (likel identifiers )
    //KEYWORDS BLOCK 
    while(is_valid_identifier(line_tokens[i].type)) // roundabout way to check if there is a value stored at the token in the vector located at index i 
    {
    
        if(line_tokens[i].type == "keyword")// if it is a keyword
        // check which one and change to corresponding intermediate assembly, or close equivalent 
            {
                tokens[i].type = "keyword";  //carry over type to new token incase 
                if(line_tokens[i].value == "return") // would the equivalent just be jmp back? for now, just translate to JMP command 
                    {
                        tokens[i].value = "JMP ";
                    }
                else if(line_tokens[i].value == "def")
                    {
                        tokens[i].value = " ";
                        //adding a space just so it doesn't count as NULL and skip the rest of the token due to printing later falsely claiming this is end
                        //possibly add flag to have token empty, but still count as part of translation? 
                    }
                else if(line_tokens[i].value == "if" || line_tokens[i].value == "elif" || line_tokens[i].value == "while")
                //when it's an if statement, scan ahead until you get to the operator. Save the I value of those identifiers, and assign accordingly
                //could potentially recursively call a function that assigns variables to assembly positons (t1,a2, etc etc)? 
                //can make this if, elif, for potentially as well? just change the assignment of the identifier token based on the token following (if it is equal, not equal, greater than, etc)
                //just have it look ahead and print just the if,elif, for equivalent for now 
                    {
                        //if statements in python can either just go straight to identifier and condition, or be enclosed in brackets. have to cover both cases 
                        if(line_tokens[i+1].type == "punctuation" && line_tokens[i+2].type == "identifier" && line_tokens[i+3].type == "equals" || line_tokens[i+1].type == "identifier" && line_tokens[i+2].type == "equals") // 1st is case with parentheses, 2nd is without, so just identifier and equals 
                        // 1st case is in the format:  if(a=)
                            {
                                tokens[i].value = "BEQ "; // branch if not equal, then later will scan for comparison variables 
                            }
                        else if(line_tokens[i+1].type == "punctuation" && line_tokens[i+2].type == "identifier" && line_tokens[i+3].type == "not_equals" || line_tokens[i+1].type == "identifier" && line_tokens[i+2].type == "not_equals") // if the 
                            {
                                 tokens[i].value = "BNE ";
                            }
                    }
                else if(line_tokens[i].value == "for") 
                //for loops in python defined as such 
                // for i in range(number_here) 
                //usually just BNE then? 
                    {
                        tokens[i].value = "BNE";
                    }

                else if(line_tokens[i].value == "class")
                    {
                        tokens[i].value = " ";
                            //add space as well, since assembly doesn't specifically have a class. just the identifier name and colon: 
                    }
                else if(line_tokens[i].value == "true")
                //unsure what the direct conversion for this would be? 
                //beq 1? just compare to 1?
                    {
                        tokens[i].value = "BEQ 1, ";
                    }
                else if(line_tokens[i].value == "false")
                    {
                        tokens[i].value = "BEQ 0, ";
                    }
                  else if(line_tokens[i].value == "None")
                    {
                        tokens[i].value = "BEQ 0, ";
                    }
                    else
                {
                    cout<<"Error: Token identified as KEYWORD, but no applicable value"<<endl;
                    return;
                }
            }
        //specifically for print command, since we have its identifier separate from  keyword token class 
        else if(line_tokens[i].type == "print_statement")
            {
                //printing in assembly usually involves moving it into a register, but since this is intermediate, i think we can just type print for now? 
                tokens[i].value = "print ";
                tokens[i].type == "print_statement";
            }
        else if(is_valid_identifier(line_tokens[i].type) ) 
            {
                //just print token if it is just an identifier, then add colon after,new line,and tab for space     
                tokens[i].type = tokens[i].type + ": \n     ";
                //in theory, a token that looks like the following in python:
                //def function_name(param1,param2)
                //the def should just be translated to a blank space, while the function name should be translated to  the name + colon, then new line and tab for ease of reading 
                //function_name: 
                //      continue_here 
            }
        //PUNCUTATION BLOCK 
        else if(line_tokens[i].type == "punctuation") // if punctuation 
        {
            tokens[i].type = "punctuation";
            if(line_tokens[i].value ==";")
            {
                tokens[i].value = " ";
                    //assembly doesn't have semicolon ending, so just skip to printing and new line printing below 
            }
            else if(line_tokens[i].value ==".")
            {
                tokens[i].value = ".";
            }
            else if(line_tokens[i].value ==":")
            {
                tokens[i].value = ":";
            }
            else if(line_tokens[i].value ==",")
            {
                tokens[i].value = ",";
            }
            else if(line_tokens[i].value =="]")
            {
                tokens[i].value = "]";
            }
             else if(line_tokens[i].value =="[]")
            {
                tokens[i].value = "]";
            }
             else if(line_tokens[i].value =="}")
            {
                tokens[i].value = "}";
            }
             else if(line_tokens[i].value =="{")
            {
                tokens[i].value = "{";
            }
             else if(line_tokens[i].value =="(")
            {
                tokens[i].value = "(";
            }
             else if(line_tokens[i].value ==")")
            {
                tokens[i].value = ")";
            }
            else
                {
                    cout<<"Error: Token identified as PUNCTUATION, but no applicable value"<<endl;
                    return;
                }
            
        }
        //OPERATOR BLOCK 
         else if(line_tokens[i].type == "operator") 
         {
            //because of the way assembly prints out operations, will likely assign the value, then have to look behind and ahead of the operators to deduce which is the source and target 
            //ex - y = x + 5 would have to be ADD ty, tx, 5, so by the time it scans th eplus, you have to look back at the previous 3 operators in this case. 
            //this means more conditions have to be checked for each of these 
            //to avoid writing the same checks everytime, likely will run through if else statements, then exit if else statement and then run checks for operators 
             if(line_tokens[i].value == "+") 
                {
                    tokens[i].value = "ADD ";//no new line; identifier registers should follow, only after 2nd operator is added should 
                    cout<<tokens[i].value;
                }
                else if(line_tokens[i].value == "-") 
                    {
                        tokens[i].value = "SUB ";
                    }
                else if(line_tokens[i].value == "*") 
                    {
                        tokens[i].value = "MUlT ";
                    }
            //should have token assigned now, so you can check behind and infront 
            //y = x + 5 
            if ((line_tokens[i-1].type == "identifier" || line_tokens[i-1].type == "number") && line_tokens[i-2].value == "=" && (line_tokens[i-3].type == "identifier" ||line_tokens[i-3].type == "number") &&(line_tokens[i+1].type == "identifier" ||line_tokens[i+1].type == "number") )
            {
                //if of the form y = x + 5 
                //y = x + x 
                //y = 5 + x
                cout<< tokens[i].value, " ", tokens[i-3].value, tokens[i-2].value, tokens[i+1].value,"\n"; 
                //should nowbe in form ADD ttarget, tsource1, tsource2
            }
            else
            {
                cout<< tokens[i].value, " ", tokens[i-1].value, tokens[i+1].value,"\n"; 
                //for mult, since that only has two operands 
            }

         }

        //IDENTIFIER BLOCK 
        else if(line_tokens[i].type == "identifier") 
         {
             tokens[i].type = "identifier";
           tokens[i].value = "t" + tcounter; 
           cout<<tokens[i].value;
           tcounter++;
         }

        
        //print converted intermediate token before moving onto next token translation 
        i++; 
    }   
    return tokens; 
}