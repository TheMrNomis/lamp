#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

/**
 * @brief Object used to create string by appending chars without re-allocations
 */
class StringBuilder
{
    public:
        StringBuilder();
        ~StringBuilder();

        /**
         * @brief empties everything and go back to an empty string
         */
        void clear();

        /**
         * @return true if this is empty
         */
        bool empty() const;

        /**
         * @brief appends a character
         */
        void append(char const c);
        inline void operator += (char const c) {this-> append(c);}

        /**
         * @brief get the string
         *
         * @return a dynamically allocated string (be sure to delete it afterwards)
         */
        char* to_string() const;

    protected:
        /**
         * @brief Class used to simulate a std::forward_list
         */
        class Node
        {
            public:
                Node(Node* next, char c);
                ~Node();

                inline char getChar() const {return m_c;}
                inline Node const* next() const {return m_next;}

            protected:
                Node* m_next;
                char  m_c;
        };

    protected:
        unsigned int m_size;
        Node * m_front;
};

#endif //STRING_BUILDER_H
