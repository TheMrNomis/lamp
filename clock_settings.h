#ifndef CLOCK_SETTINGS_H
#define CLOCK_SETTINGS_H

class ClockSettings
{
    public:
        ClockSettings();
        ~ClockSettings();

        void add_info(char const* key, char const* val);

    protected:
        char const* m_server;
        unsigned int m_cooldown;
};

#endif //CLOCK_SETTINGS_H
