#include "./string.h"

bool string_starts_by(char const* hay, char const* needle)
{
    if(needle == nullptr)
        return true;
    if(hay == nullptr)
        return false;

    for(unsigned int i = 0; needle[i] != '\0'; ++i)
    {
        if(hay[i] == '\0')
            return false;
        if(hay[i] != needle[i])
            return false;
    }

    return true;
}

bool string_equals(char const* a, char const* b)
{
    if(a == nullptr) return b == nullptr;
    if(b == nullptr) return false;

    unsigned int i = 0;
    for(; a[i] != '\0' && b[i] != '\0'; ++i)
        if(a[i] != b[i]) return false;

    //either a or b is finished, so we check that both are
    return a[i] == b[i];
}

char* string_copy(char const* s)
{
    unsigned int const len = strlen(s)+1;

    char * ret = new char[len];
    for(unsigned int i = 0; i < len; ++i)
        ret[i] = s[i];

    return ret;
}
