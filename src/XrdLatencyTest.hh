/* 
 * File:   XrdLatencyTest.h
 * Author: jsalmon
 *
 * Created on November 5, 2012, 10:19 AM
 */

#ifndef XRDLATENCYTEST_H
#define	XRDLATENCYTEST_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

class XrdLatencyTest {
public:
    XrdLatencyTest();
    virtual ~XrdLatencyTest();

    std::vector<std::string> hosts;
    std::string statpath;
    
    int statinterval;
    int floodinterval;

    int addHost(std::string host, int port);
    int addHost(std::string host);
    void printHosts();
    int removeHost(std::string host, int port);
    int removeHost(std::string host);
    int setStatPath(std::string path = "/");
    int setStatInterval(size_t seconds);
    int setFloodInterval(size_t seconds);

    void PrintOut(std::string &out, std::string &err);
    bool Start(bool do_stat, bool do_flood);
    bool Stop();

    bool GetLatencies(std::map<std::string, double> &measurement);
    bool GetFloodRate(std::map<std::string, double> &measurements);

};

#endif	/* XRDLATENCYTEST_H */

