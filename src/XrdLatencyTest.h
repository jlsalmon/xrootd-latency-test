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
#include <map>
#include <ctype.h>
#include <unistd.h>

class XrdLatencyTest {
public:
    XrdLatencyTest();
    virtual ~XrdLatencyTest();
private:

    int addHost(std::string host, int port);
    int addHost(std::string host); /* "<node>[:port]" */
    int removeHost(std::string host, int port);
    int removeHost(std::string host);
    int setStatPath(std::string path = "/");
    int setStatInterval(size_t seconds);
    int setFloodInterval(size_t seconds);

    // print the latest results to out and errors to err
    void PrintOut(std::string &out, std::string &err);
    // start the thread doing the measurements optionally with stat or flood option
    bool Start(bool do_stat, bool do_flood);
    // stop the thread doing the measurements
    bool Stop();

    // true if measured, false if nothing measured
    bool GetLatencies(std::map<std::string, double> &measurement);
    // true if measured, false if nothing measured
    bool GetFloodRate(std::map<std::string, double> &measurements);

};

#endif	/* XRDLATENCYTEST_H */

