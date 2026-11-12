#include "DNSManager.h"

DNSManager::DNSManager()
    : running(false) {}

void DNSManager::start(String name)
{
    currentName = name;
    if (!running)
    {
        dnsServer.setTTL(300);
        dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
    }
    else
    {
        dnsServer.stop();
    }
    running = dnsServer.start(DNS_PORT, currentName, INNERIP);
}

void DNSManager::processNextRequest()
{
    dnsServer.processNextRequest();
}

String DNSManager::getName()
{
    return currentName;
}

DNSManager &theDNSManager()
{
    static DNSManager dm;
    return dm;
}