/**
 * @file settings.cpp
 * @author n0m1s
 */
#include "settings.h"

#include <SD.h>

bool is_whitespace(char const c)
{
    return c == ' ' || c == '\t';
}

bool is_printable(char const c)
{
    //cf. ASCII table
    return c >= 0x20 && c <= 0x7E;
}

Settings::Settings()
{
}

Settings::~Settings()
{
}

void Settings::add_line(char const* section, char const* key, char const* val)
{
    //TODO: do something with the line
    Serial.print("Section: \"");
    Serial.print(section);
    Serial.print("\", Key: \"");
    Serial.print(key);
    Serial.print("\", Val: \"");
    Serial.print(val);
    Serial.println("\"");

    delete[] section;
    delete[] key;
    delete[] val;
}

Settings const* Settings::load(char const* settings_filename)
{
    Serial.println(""); //TODO: remove debug message
    if(!SD.begin(D8))
    {
        Serial.println("cannot open SD card");
        return NULL;
    }

    File file = SD.open("settings.ini", FILE_READ);
    if(!file)
    {
        Serial.println("cannot open settings file");
        return NULL;
    }

    Settings* settings = new Settings();

    Serial.println("----Settings----"); //TODO: remove debug message

    bool first_char_of_line = true;
    bool is_comment_line = false;
    bool is_section_line = false;
    unsigned int nb_whitespace = 0; //used for trimming the end of lines

    StringBuilder current_section;
    StringBuilder current_key;
    StringBuilder current_val;

    bool write_to_key = true;

    while(file.available())
    {
        char const c = file.read();
        if(!is_printable(c)) continue;

        if(first_char_of_line)
        {
            //trim whitespace at beginning of line
            if(is_whitespace(c)) continue;

            first_char_of_line = false;

            //check if comment, section or normal line
            if(c == ';')
            {
                is_comment_line = true;
                continue;
            }
            else if (c == '[')
            {
                is_section_line = true;
                current_section.clear();
                continue;
            }
        }

        if(c == '\n')
        {
            first_char_of_line = true;
            is_comment_line = false;
            is_section_line = false;

            settings->add_line(current_section.to_string(),
                               current_key.to_string(),
                               current_val.to_string());

            current_key.clear();
            current_val.clear();
            write_to_key = true;
        }
        else if(is_comment_line)
        {
            //discarding comment lines
            continue;
        }
        else if(is_section_line)
        {
            if(c == ']')
            {
                //discard end of line
                is_section_line = false;
                is_comment_line = true;
            }
            else
            {
                current_section += c;
            }
        }
        else
        {
            if(is_whitespace(c))
                ++nb_whitespace;
            else if(write_to_key && c == '=')
            {
                write_to_key = false;
            }
            else
            {
                if(write_to_key)
                    current_key += c;
                else
                    current_val += c;
            }
        }
    }
    if(!first_char_of_line && !is_comment_line && !is_section_line)
        settings->add_line(current_section.to_string(),
                           current_key.to_string(),
                           current_val.to_string());

    file.close();

    Serial.println("----End settings----"); //TODO: remove debug log

    return settings;
}
