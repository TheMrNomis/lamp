#ifndef STRING_HELPER_H
#define STRING_HELPER_H

inline bool is_whitespace(char const c)
{
    return c == ' ' || c == '\t';
}

inline bool is_printable(char const c)
{
    //cf. ASCII table
    return c == '\n' || (c >= 0x20 && c <= 0x7E);
}

/**
 * @brief checks that if a string (hay) starts by a prefix (needle)
 */
bool string_starts_by(char const* hay, char const* needle);

/**
 * @brief checks that two strings have the same characters (and no others)
 */
bool string_equals(char const* a, char const* b);

/**
 * @brief copies a string
 */
char* string_copy(char const* s);

#endif //STRING_HELPER_H
