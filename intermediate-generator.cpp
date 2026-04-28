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
vector<Token> token_convert(vector<Token> line_tokens) // input is the same type as the output from lexical-analyzer.cpp. Additionally, pass # of lines as well to run while loop  
// this class has 
{
    vector<Token> tokens; //  new token dynamic array to hold converted values, then return at the end of the function 
    int i = 0; 

    //IDENTIFIER BLOCK 
    while(is_valid_identifier(line_tokens[i].type)) // roundabout way to check if there is a value stored at the token in the vector located at index i 
    {
        if(is_keyword(line_tokens[i].type) )// if it is a keyword
        // check which one and change to corresponding intermediate assembly, or close equivalent 
            {
                tokens[i].value = i; 
                if(line_tokens[i].type == "return") // would the equivalent just be jmp back? for now, just translate to JMP command 
                    {
                        tokens[i].type = "JMP ";
                    }
                else if(line_tokens[i].type == "def")
                    {
                        tokens[i].type = " ";
                        //adding a space just so it doesn't count as NULL and skip the rest of the token due to printing later falsely claiming this is end
                        //possibly add flag to have token empty, but still count as part of translation? 
                    }
                else if(line_tokens[i].type == "if" ||)
                //if it is an if statement, scan ahead until you get to the end of the condition. Save the I value of those identifiers, and assign accordingly
                //could potentially recursively call a function that assigns variables to assembly positons (t1,a2, etc etc)? 
                //can make this if, elif, for potentially as well? just change the assignment of the identifier token based on the token following (if it is equal, not equal, greater than, etc)
                for 
                    {
                        tokens[i].type = "BEQ"; // using if = beq for now 
                    }


                else if(line_tokens[i].type == "elif" || line_tokens[i].type == "while") 
                    {
                        tokens[i].type = "BEQ";
                    }
                else if(line_tokens[i].type == "for")
                    {
                        tokens[i].type = "BNE";
                    }
                else if(line_tokens[i].type == "class")
                    {
                        tokens[i].type = "CLASS";
                    }
                else if(line_tokens[i].type == "true")
                    {
                        tokens[i].type = "TRUE";
                    }
            }
        //specifically for print command, since we have its identifier separate from  keyword token class 
        else if(line_tokens[i].type == "print")
            {
                tokens[i].type = "print ";
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
        
        //print converted intermediate token before moving onto next token translation 
        cout<<tokens[i].type<<endl; 
        i++; 
    }   
    return tokens; 
    //return tokens vector so it can be passed through to generate actual assembly 
}
        
        

        
    
    //pass token here 
    // scan what type of token, and assign assembly equivalent based on the token
    //if number, pass it through
    //for different registers, use a counting variable and assign it 
    // print? 



