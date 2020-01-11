#ifndef CLOCK_SETTINGS_H
#define CLOCK_SETTINGS_H

class ClockSettings
{
    public:
        ClockSettings();
        ~ClockSettings();

        void add_info(char const* key, char const* val);

        inline char const*const server() const {return m_server;}
        inline unsigned int cooldown() const {return m_cooldown;}

    protected:
        char const* m_server;
        unsigned int m_cooldown;
};

#endif //CLOCK_SETTINGS_H
