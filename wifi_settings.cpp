#include "wifi_settings.h"
#include "string_helper.h"

WifiSettings::Network::Network(char const* name, Network* next):
    m_name(name),
    m_ssid(nullptr),
    m_pass(nullptr),
    m_next(next)
{
}

WifiSettings::Network::~Network()
{
    if(m_next != nullptr)
        delete m_next;
}

WifiSettings::WifiSettings():
    m_first_network(nullptr)
{
}

WifiSettings::~WifiSettings()
{
    if(m_first_network != nullptr)
        delete m_first_network;
}

void WifiSettings::add_info(char const* network_name, char const* key, char const* val)
{
    Network* net = find_network(network_name);
    if(net == nullptr)
    {
        net = new Network(string_copy(network_name), m_first_network);
        m_first_network = net;
    }

    if(string_equals(key, "ssid"))
    {
        if(net->m_ssid != nullptr)
            delete[] net->m_ssid;

        net->m_ssid = val;
    }
    else if(string_equals(key, "password"))
    {
        if(net->m_pass != nullptr)
            delete[] net->m_pass;

        net->m_pass = val;
    }
    else
        delete[] val;

    delete[] key;
}

WifiSettings::Network const* WifiSettings::networks_cbegin() const
{
    return m_first_network;
}

WifiSettings::Network const* WifiSettings::networks_cend() const
{
    return nullptr;
}

WifiSettings::Network* WifiSettings::find_network(char const* network_name)
{
    for(Network* net = m_first_network; net != nullptr; net = net->m_next)
        if(string_equals(net->m_name, network_name))
            return net;

    return nullptr;
}
