
/*
* Program Name: qadon (Quick And Dirty Object Notation)
*
* Purpose: this program allows you to read qadon files into an
* unordered map. The format of a qadon file is almost identical
* to that of a json file. Here is an example
* 
{
    key_1 : value_1
    key_2 : value_2
    key_3 : value_3

}
*
* Author: Christopher Corona
*
* Version: 0.1.0
*
* Edge Case:
* 
*   [1] creating a qadon with non-existent file will throw an error
*   [2] an error in the qadon file format will throw an error
*  
*/

#ifndef QADON_H
#define QADON_H
#include <unordered_map>
#include <fstream>
#include <string>
#include <stdexcept>
#include <exception>
#include <iostream>


class qadon_bracket_error : public std::exception
{
    public:
        const char* what() const throw()
        {
            return "qadon_bracket_error: qadon file is missing bracket";
        }
};

class qadon_syntax_error : public std::exception
{
    public:
        const char* what() const throw()
        {
            return "qadon_syntax_error: qadon file has incorret syntax. missing colon detected";
        }

};

class qadon_file_error : public std::exception
{
    public:
            const char* what() const throw()
            {
                return "qadon_file_error: file does not exist";
            }
};

class qadon{

    public:

        /*Constructor with file_path given*/

        qadon(std::string file_path)
        {
            // initialize vars
            qadon_bracket = 0;
            line = "";
    
            std::ifstream qadon_file(file_path);

            if (qadon_file.is_open())
            {
                // iterate through qadon file
                while(qadon_file)
                {
                    parse_line = true;

                    std::getline(qadon_file,line);


                    // find begin and end of qadon 
                    if (line.find('{') == 0|| line.find('}') == 0)
                    {
                        qadon_bracket++;
                        parse_line = false;
                    }
        
                    // parase line and build key and value pair

                    if (parse_line && qadon_bracket == 1)
                    {
                        int colon_index = line.find(':');
                        if (colon_index == -1) throw qadon_syntax_error();

                        int char_index = 0;
                        std::string key = "";
                        std::string value = "";

                        // assemble key
                        while(char_index < colon_index)
                        {
                            if ( line[char_index] != ' ') key += line[char_index];
                            char_index++;
                        }
                        char_index++; // skip over colon

                        // assemble value
                        while(char_index < line.size())
                        {
                            value += line[char_index];
                            char_index++;
                        }

                        map[key] = value;

                    } 
                }
        
                qadon_file.close();

                if (qadon_bracket == 1)
                {
                    throw qadon_bracket_error();
                }
            } else {throw qadon_file_error();}
        }

        /* Constructor without file_path*/
        qadon()
        {
            // initialize vars
            qadon_bracket = 0;
            line = "";
        }

        std::string& operator[](std::string key)
        {
            return map[key];
        }
        
        /*
        * Function: write()
        *
        * Purpose: this function writes your qadon
        * to a file
        *
        * Arguments: file_path
        * this is argument contains the path and name of the file
        * the qadon will be written to
        *
        */
        void write(std::string file_path)
        {
            std::ofstream new_file(file_path);
            new_file << "{\n";
            for(const auto& pair: map)
            {
            new_file <<  pair.first << " : " << pair.second << "\n";
            }
            new_file << "}\n";
            new_file.close();
        }

        /*
        * Funtion: print()
        *
        * Purpose: This function prints
        * all the key, value pairs inside
        * the qadon
        *
        */
        void print()
        {
            for(const auto& pair : map)
            {
            std:: cout << "( " << pair.first << " , " << pair.second << " ) " << std::endl;
            }
        }

        // iterator variables
        auto begin() { return map.begin(); }
        auto end() { return map.end(); }

    private:
        bool parse_line;
        int qadon_bracket;
        std::string line;
        std::unordered_map<std::string,std::string> map;

};


#endif