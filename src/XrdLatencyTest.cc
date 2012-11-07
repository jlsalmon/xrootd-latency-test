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

#include "XrdLatencyTest.hh"
#include "AsyncStatResponse.hh"
#include "SyncStatResponse.hh"
#include "XrdCl/XrdClFileSystem.hh"
#include "XrdCl/XrdClURL.hh"
#include "XrdSys/XrdSysPthread.hh"
#include <ctime>
#include <sys/time.h>
#include <iomanip>

XrdLatencyTest::XrdLatencyTest() {
    currenthost = "";
    statpath = "/tmp";
    proto = "root://";
    async = true;
    flood = false;
    loop = false;
    statinterval = 1;
    floodinterval = 10;
}

XrdLatencyTest::XrdLatencyTest(std::string statpath,
        bool flood,
        bool loop,
        size_t statinterval,
        size_t floodinterval) {

    setStatPath(statpath);
    setFlood(flood);
    setLoop(loop);
    setStatInterval(statinterval);
    setFloodInterval(floodinterval);
}

XrdLatencyTest::~XrdLatencyTest() {
}

bool XrdLatencyTest::Start() {

    using namespace XrdCl;

    std::map<std::string, StatResponse*>::iterator it;
    int n = 1;
    if (flood) n = 1000;

    do {
        for (it = hosts.begin(); it != hosts.end(); ++it) {

            currenthost = it->first;
            URL url(proto + currenthost);
            FileSystem fs(url);

            // Probe each host to make sure it's alive.
            if (!Probe(fs)) continue;

            for (int i = 0; i < n; i++) {
                Stat(fs);
            }
        }

        // Wait until we have received a response from all hosts
        bool wait = true;
        int readycount = 0;

        while (wait) {
            for (it = hosts.begin(); it != hosts.end(); ++it) {
                if (it->second->IsDone()) {
                    readycount++;
                }
            }

            if (hosts.size() == readycount) {
                wait = false;
            }
        }

        PrintOut();

        sleep(statinterval);
    } while (loop);
}

bool XrdLatencyTest::Probe(XrdCl::FileSystem &fs) {
    StatResponse *sr = new SyncStatResponse();
    sr->DoStat(fs, statpath);
    return (sr->GetXrootdStatus().IsOK());
}

void XrdLatencyTest::Stat(XrdCl::FileSystem &fs) {
    StatResponse *sr;

    if (this->async) {
        sr = new AsyncStatResponse();
    } else {
        sr = new SyncStatResponse();
    }

    sr->DoStat(fs, statpath);
    hosts[currenthost] = sr;
}

bool XrdLatencyTest::Stop() {

}

std::map<std::string, StatResponse*> XrdLatencyTest::GetLatencies() {
    return hosts;
}

void XrdLatencyTest::PrintOut() {
    std::map<std::string, StatResponse*>::iterator i;
    double total = 0;
    
    for (i = hosts.begin(); i != hosts.end(); ++i) {
//        std::cout << i->second->GetXrootdStatus().ToString();
//        std::cout << "\t";
//        std::cout << i->second->GetLatencyAsString() << std::endl;
        
        total += i->second->GetLatency();
    }
    
    std::cout << "hosts: " << hosts.size();
    std::cout << " avg: " << total / hosts.size() << "ms" << std::endl;
}

void XrdLatencyTest::addHostsFromFile(std::string path) {
    std::ifstream in(path.c_str(), std::ios_base::in);
    
    if (in.good()) {
        std::string line;
        while (std::getline(in, line)) {
            addHost(line);
        }    
    }
    
    in.close();
}

int XrdLatencyTest::addHost(std::string host, int port) {
    std::stringstream ss;
    ss << port;
    addHost(host += ":" + ss.str());
}

int XrdLatencyTest::addHost(std::string host) {
    StatResponse *sr = 0;
    hosts.insert(std::make_pair(host, sr));
}

void XrdLatencyTest::printHosts() {
    std::map<std::string, StatResponse*>::iterator i;

    for (i = hosts.begin(); i != hosts.end(); ++i)
        std::cout << i->first << std::endl;
}

int XrdLatencyTest::removeHost(std::string host, int port) {
    std::stringstream ss;
    ss << port;
    removeHost(host += ":" + ss.str());
}

int XrdLatencyTest::removeHost(std::string host) {
    hosts.erase(host);
}

void XrdLatencyTest::setFlood(bool flood) {
    this->flood = flood;
}

void XrdLatencyTest::setLoop(bool loop) {
    this->loop = loop;
}

void XrdLatencyTest::setStatPath(std::string path) {
    statpath = path;
}

void XrdLatencyTest::setStatInterval(int seconds) {
    statinterval = seconds;
}

void XrdLatencyTest::setFloodInterval(int seconds) {
    floodinterval = seconds;
}

void XrdLatencyTest::setAsync(bool async) {
    this->async = async;
}

void XrdLatencyTest::setVerbose(bool verbose) {
    this->verbose = verbose;
}

