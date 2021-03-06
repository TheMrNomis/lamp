#include "string_builder.h"

StringBuilder::StringBuilder():
    m_size(0),
    m_front(nullptr)
{
}

StringBuilder::~StringBuilder()
{
    if(m_front != nullptr)
        delete m_front;
}

void StringBuilder::clear()
{
    if(m_front != nullptr)
        delete m_front;
    m_front = nullptr;

    m_size = 0;
}

bool StringBuilder::empty() const
{
    return m_size == 0;
}

void StringBuilder::append(char const c)
{
    ++m_size;
    Node* new_front = new Node(m_front, c);
    m_front = new_front;
}

void StringBuilder::append_number(int x)
{
    if(x < 0)
    {
        this->append('-');
        x = -x;
    }

    int max_divider = 10;
    while(x/max_divider > 10)
        max_divider*=10;

    while(max_divider > 0)
    {
        char c = x/max_divider;
        x = x%max_divider;
        max_divider /= 10;

        this->append_digit(c);
    }
}

void StringBuilder::append_digit(char x)
{
    x = (x % 10) + '0';
    this->append(x);
}

char* StringBuilder::to_string() const
{
    char* ret = new char[m_size + 1];
    if(ret == nullptr)
        return ret;

    //populating the string in reverse
    //because m_front is the last char of the string
    ret[m_size] = '\0';
    unsigned int i = m_size - 1;
    for(Node const* it = m_front; it != nullptr; it = it->next())
        ret[i--] = it->getChar();

    return ret;
}

StringBuilder::Node::Node(Node* next, char c):
    m_next(next),
    m_c(c)
{
}

StringBuilder::Node::~Node()
{
    if(m_next != nullptr)
        delete m_next;
}
