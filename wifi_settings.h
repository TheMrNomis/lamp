#ifndef WIFI_SETTINGS_H
#define WIFI_SETTINGS_H

class WifiSettings
{
    public:
        class Network
        {
            friend class WifiSettings;

            protected:
                Network(char const* name, Network* next);

            public:
                ~Network();

                inline bool has_all_infos() const
                {
                    return m_ssid != nullptr && m_pass != nullptr;
                }

                inline char const* ssid() const {return m_ssid;}
                inline char const* pass() const {return m_pass;}

                inline Network const* next() const {return m_next;}

            private:
                char const*const m_name;
                char const* m_ssid;
                char const* m_pass;

                Network *const m_next;
        };

        WifiSettings();
        ~WifiSettings();

        void add_info(char const* network_name, char const* key, char const* val);

        Network const* networks_cbegin() const;
        Network const* networks_cend() const;

    protected:
        Network* find_network(char const* network_name);

    protected:
        Network* m_first_network;
};

#endif //WIFI_SETTINGS_H
