#include "settings.h"

#include <SD.h>
#include "string_helper.h"

Settings::Settings()
{
}

Settings::~Settings()
{
}

void Settings::add_line(char const* section, char const* key, char const* val)
{
    if(string_starts_by(section, "wifi"))
    {
        char const* network_name = "0";
        if(section[4] == ':')
            network_name = section + 5;

        wifi.add_info(network_name, key, val);
    }
    else if(string_equals(section, "clock"))
        clock.add_info(key, val);
    else
    {
        delete[] key;
        delete[] val;
    }

    delete[] section;
}

Settings const* Settings::load(char const* settings_filename)
{
    if(!SD.begin(D8))
        return NULL;

    File file = SD.open("settings.ini", FILE_READ);
    if(!file)
        return NULL;

    Settings* settings = new Settings();

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

            //discard whitespace-only lines
            if(c == '\n') continue;

            first_char_of_line = false;

            //is it a comment line?
            if(c == ';')
            {
                is_comment_line = true;
                continue;
            }
            //is it a section line?
            else if (c == '[')
            {
                is_section_line = true;
                current_section.clear();
                continue;
            }
        }

        if(c == '\n')
        {
            if(!is_comment_line && !is_section_line)
                settings->add_line(current_section.to_string(),
                                   current_key.to_string(),
                                   current_val.to_string());

            first_char_of_line = true;
            is_comment_line = false;
            is_section_line = false;

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
            else
            {
                StringBuilder& writer = write_to_key? current_key : current_val;

                for(unsigned int i = 0; i < nb_whitespace; ++i)
                    writer += ' ';
                nb_whitespace = 0;

                if(write_to_key && c == '=')
                    write_to_key = false;
                else
                    writer += c;
            }
        }
    }
    if(!first_char_of_line && !is_comment_line && !is_section_line)
        settings->add_line(current_section.to_string(),
                           current_key.to_string(),
                           current_val.to_string());

    file.close();

    return settings;
}
