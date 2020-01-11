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
        inline char const*const api_key() const {return m_api_key;}
        inline char const*const time_zone() const {return m_time_zone;}

    protected:
        char const* m_server;
        unsigned int m_cooldown;
        char const* m_api_key;
        char const* m_time_zone;
};

#endif //CLOCK_SETTINGS_H
