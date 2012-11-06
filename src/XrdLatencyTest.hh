//------------------------------------------------------------------------------
// Copyright (c) 2011-2012 by European Organization for Nuclear Research (CERN)
// Author: Justin Salmon <jsalmon@cern.ch>
//------------------------------------------------------------------------------
// XRootD is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// XRootD is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with XRootD.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

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
    
    std::vector<std::string> hosts;
    std::string statpath;
    bool flood;
    bool loop;
    int statinterval;
    int floodinterval;
    bool verbose;

    /**
     * Default constructor
     */
    XrdLatencyTest();

    /**
     * Optional constructor
     * 
     * @param statpath: the path on the remote box to stat
     * @param flood: perform flood or not
     * @param loop: loop mode or not
     * @param statinterval: time between stats (if looping)
     * @param floodinterval: time between floods (if looping)
     */
    XrdLatencyTest(std::string statpath, 
                   bool flood, 
                   bool loop, 
                   size_t statinterval, 
                   size_t floodinterval);

    /**
     * Destructor
     */
    virtual ~XrdLatencyTest();


    /**
     * Add a host to this test.
     * 
     * @param host: the hostname of the host to be added
     * @param port: the xrootd port of the host
     * @return
     */
    int addHost(std::string host, int port);

    /**
     * Add a host to this test.
     * 
     * @param host: the hostname/port of the host to be added in the
     *              form: "<hostname>[:port]"
     * @return 
     */
    int addHost(std::string host);

    /**
     * Print out the current list of hosts.
     */
    void printHosts();

    /**
     * Remove a host from this test.
     * 
     * @param host: the hostname of the host to be removed
     * @param port: the xrootd port that this host uses
     * @return 
     */
    int removeHost(std::string host, int port);

    /**
     * Remove a host from this test.
     * 
     * @param host: the hostname/port of the host to be removed in the
     *              form: "<hostname>[:port]"
     * @return 
     */
    int removeHost(std::string host);

    /**
     * Set the path to stat on the remote box. Defaults to "/".
     * 
     * @param path: the local path on the remote box.
     */
    void setStatPath(std::string path);

    /**
     * 
     * @param flood
     */
    void setFlood(bool flood);

    /**
     * 
     * @param loop
     */
    void setLoop(bool loop);

    /**
     * Set the interval between single stat requests during operation.
     * 
     * @param seconds: the number of seconds between stats
     */
    void setStatInterval(int seconds);

    /**
     * Set the interval between stat floods during operation.
     * 
     * @param seconds: the number of seconds between floods
     */
    void setFloodInterval(int seconds);

    /**
     * Print the latest results to out and errors to err.
     * 
     * @param out: successful results
     * @param err: error results
     */
    void PrintOut(std::string &out, std::string &err);

    /**
     * Start the thread doing the measurements
     * 
     * @return 
     */
    bool Start();

    /**
     * Stop the thread doing the measurements.
     *
     * @return 
     */
    bool Stop();

    /**
     * 
     * @param measurement
     * @return true if measured, false if nothing measured
     */
    bool GetLatencies(std::map<std::string, double> &measurement);

    /**
     * 
     * @param measurements
     * @return true if measured, false if nothing measured
     */
    bool GetFloodRate(std::map<std::string, double> &measurements);

    /**
     * 
     * @param verbose
     */
    void setVerbose(bool verbose);
};

#endif	/* XRDLATENCYTEST_H */

