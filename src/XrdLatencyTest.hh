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

#include "Host.hh"

#include "XrdCl/XrdClURL.hh"
#include "XrdSys/XrdSysPthread.hh"
#include "XrdCl/XrdClFileSystem.hh"

#include <fstream>

class XrdLatencyTest {
public:

    std::map<std::string, Host*> hosts;
    std::string currenthost;
    std::string statpath;
    std::string proto;
    bool async;
    bool flood;
    bool loop;
    int statinterval;
    int floodinterval;
    int floodcount;
    bool verbose;
    pthread_t thread;
    XrdSysCondVar cv;

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
            bool verbose,
            size_t statinterval,
            size_t floodinterval);

    /**
     * Destructor
     */
    virtual ~XrdLatencyTest();

    /**
     * Start the thread doing the measurements.
     * 
     * @return true if the thread was started, false otherwise
     */
    bool Start();

    /**
     * Static wrapper to Run()
     */
    static void* StaticRun(void* args);

    /**
     * Start making the actual measurements.
     */
    void Run();

    /**
     * Stop the thread doing the measurements.
     *
     * @return true if the thread was stopped, false otherwise
     */
    bool Stop();

    /**
     * 
     * @param host
     * @return 
     */
    double GetLatency(Host* host);


    /**
     * Get the latest results.
     * 
     * @return a map containing "hostname/Host object" key/values 
     */
    std::map<std::string, Host*> GetLatencies();

    /**
     * Print the latest results.
     */
    void PrintOut();

    /**
     * Add a host to this test.
     * 
     * @param host: the hostname of the host to be added
     * @param port: the xrootd port of the host
     */
    void addHost(std::string host, int port);

    /**
     * Add a host to this test.
     * 
     * @param host: the hostname/port of the host to be added in the
     *              form: "<hostname>[:port]"
     */
    void addHost(std::string host);

    /**
     * Insert a bunch of hosts into the test from a file. The file must 
     * contain tokens of the format <hostname>:port and be separated
     * by a newline.
     * 
     * @param path: the path to the host file
     */
    void addHostsFromFile(std::string path);

    /**
     * Remove a host from this test.
     * 
     * @param host: the hostname of the host to be removed
     * @param port: the xrootd port that this host uses
     */
    void removeHost(std::string host, int port);

    /**
     * Remove a host from this test.
     * 
     * @param host: the hostname/port of the host to be removed in the
     *              form: "<hostname>[:port]"
     */
    void removeHost(std::string host);

    /**
     * Set the path to stat on the remote box. Defaults to "/".
     * 
     * @param path: the local path on the remote box.
     */
    void setStatPath(std::string path) {
        statpath = path;
    }

    /**
     * Whether to use flood mode or not, i.e. perform a bunch of stats
     * each time instead of just a single one. Defaults to false.
     * 
     * @param flood: true/false flood or not
     */
    void setFlood(bool flood) {
        this->flood = flood;
    }

    /**
     * Whether to run in loop mode or not, i.e. do a one-shot stat or
     * keep shooting forever. Defaults to false.
     * 
     * @param loop: true/false loop or not
     */
    void setLoop(bool loop) {
        this->loop = loop;
    }

    /**
     * Set the interval between single stat requests during operation.
     * Defaults to 1.
     * 
     * @param seconds: the number of seconds between stats
     */
    void setStatInterval(int seconds) {
        statinterval = seconds;
    }

    /**
     * Set the interval between stat floods during operation. Defaults
     * to 2.
     * 
     * @param seconds: the number of seconds between floods
     */
    void setFloodInterval(int seconds) {
        floodinterval = seconds;
    }

    /**
     * 
     * @param count
     */
    void setFloodCount(int count) {
        floodcount = count;
    }

    /**
     * Whether to run in asynchronous mode or not, i.e. whether to make
     * asynchronous requests via XrdCl. Defaults to true.
     * 
     * @param async: true/false async mode or not
     */
    void setAsync(bool async) {
        this->async = async;
    }

    /**
     * Whether to run in verbose mode or not. Defaults to false.
     * 
     * @param verbose: true/false verbose or not
     */
    void setVerbose(bool verbose) {
        this->verbose = verbose;
    }

private:

    /**
     * Convert tv_sec & tv_usec to milliseconds.
     * 
     * @param tv: timeval to convert
     * @return timeval in milliseconds
     */
    double mstime(struct timeval tv);

    /**
     * Calculate the time difference between two times.
     * 
     * @param req: request (start) time
     * @param resp: response (end) time
     * @return time difference in milliseconds
     */
    double timediff(struct timeval req, struct timeval resp);
};

#endif	/* XRDLATENCYTEST_H */

