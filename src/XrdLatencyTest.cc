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

int XrdLatencyTest::addHost(std::string host, int port) {
    std::stringstream ss;
    ss << port;
    addHost(host += ":" + ss.str());
}

int XrdLatencyTest::addHost(std::string host) {
    StatResponse *sr;
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

void XrdLatencyTest::PrintOut(std::string& out, std::string& err) {

}

bool XrdLatencyTest::Start() {

    using namespace XrdCl;

    std::map<std::string, StatResponse*>::iterator it;
    int n = 1;
    if (flood) n = 1000;


    do {
        for (it = hosts.begin(); it != hosts.end(); ++it) {

            // Probe each host to make sure it's alive.

            currenthost = it->first;

            URL url(proto + currenthost);
            FileSystem fs(url);

            for (int i = 0; i < n; i++) {
                Stat(fs);
            }
        }

        // Wait until we have received all responses.
        // Define timeout length (10s).

        sleep(statinterval);
    } while (loop);
}

void XrdLatencyTest::Stat(XrdCl::FileSystem &fs) {
    StatResponse *sr;
    
    if (this->async) {
        sr = new AsyncStatResponse();
    } else {
        sr = new SyncStatResponse();
    }
    
    hosts[currenthost] = sr;
    sr->init();
    sr->DoStat(fs, statpath);
}

//bool XrdLatencyTest::AsyncStat(XrdCl::FileSystem &fs, StatResponse &sr) {
//    using namespace XrdCl;
//
//    hosts.at(currenthost); 
//    sr.->DoStat(fs, statpath);
//}
//
//bool XrdLatencyTest::SyncStat(XrdCl::FileSystem &fs, StatResponse &sr) {
//    using namespace XrdCl;
//
//    StatInfo *si = 0;
//    struct timeval reqtime, resptime;
//
//    gettimeofday(&reqtime, NULL);
//    fs.Stat(statpath, si, 10);
//    gettimeofday(&resptime, NULL);
//
//    double diff = ((resptime.tv_sec * 1000000.0 + resptime.tv_usec)
//            - (reqtime.tv_sec * 1000000.0 + reqtime.tv_usec)) / 1000;
//
//    std::cout << "Latency: " << std::setprecision(3) << std::fixed << diff << " ms" << std::endl;
//}

bool XrdLatencyTest::Stop() {

}

bool XrdLatencyTest::GetLatencies(std::map<std::string, double>& measurement) {

}

bool XrdLatencyTest::GetFloodRate(std::map<std::string, double>& measurements) {

}

void XrdLatencyTest::setVerbose(bool verbose) {
    this->verbose = verbose;
}

