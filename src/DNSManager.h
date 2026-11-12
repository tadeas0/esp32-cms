#ifndef DNS_MANAGER_H
#define DNS_MANAGER_H

#include <DNSServer.h>
#include "constants.h"

class DNSManager
{
public:
    void start(String name);
    String getName();
    friend DNSManager &theDNSManager();
    void processNextRequest();

private:
    DNSManager();
    bool running;
    String currentName;
    DNSServer dnsServer;
};

extern DNSManager &theDNSManager();

#endif