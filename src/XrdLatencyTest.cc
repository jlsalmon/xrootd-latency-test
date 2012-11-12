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
    cv = XrdSysCondVar(0);
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

    if (!hosts.size()) {
        std::cout << "no hosts defined." << std::endl;
        exit(0);
    }

    std::cout << "host count:           " << hosts.size() << std::endl;
    std::cout << "stat path:            " << statpath << std::endl;
    std::cout << "synchronous:          " << std::boolalpha << !async << std::endl;
    std::cout << "flood mode:           " << std::boolalpha << flood << std::endl;
    std::cout << "loop mode:            " << std::boolalpha << loop << std::endl;
    std::cout << "stat interval:        " << statinterval << std::endl;
    std::cout << "flood interval:       " << floodinterval << std::endl;

    std::map<std::string, StatResponse*>::iterator it;
    int n = 1;
    if (flood) n = 1000;

    do {
        for (it = hosts.begin(); it != hosts.end(); ++it) {

            currenthost = it->first;
            URL url(proto + currenthost);
            FileSystem fs(url);

            // Probe each host to make sure it's alive.
            if (!Probe(fs)) {
                removeHost(currenthost);
                continue;
            }

            for (int i = 0; i < n; i++) {
                Stat(fs);
            }
        }

        // Wait until we have received a response from all hosts
        cv.Lock();
        while (WaitHosts()) {
            cv.Wait(10);
        }

        PrintOut();
        cv.UnLock();

        sleep(statinterval);
    } while (loop);
}

bool XrdLatencyTest::Probe(XrdCl::FileSystem &fs) {
    StatResponse *sr = new SyncStatResponse();
    sr->DoStat(fs, statpath);
    hosts[currenthost] = sr;

    if (!sr->GetXrootdStatus().IsOK()) {
        std::cout << currenthost << statpath << " not responding: ";
        std::cout << sr->GetXrootdStatus().ToStr() << std::endl;
    }
    return (sr->GetXrootdStatus().IsOK());
}

void XrdLatencyTest::Stat(XrdCl::FileSystem &fs) {
    StatResponse *sr;

    if (this->async) {
        sr = new AsyncStatResponse(cv);
    } else {
        sr = new SyncStatResponse();
    }

    sr->DoStat(fs, statpath);
    hosts[currenthost] = sr;
}

bool XrdLatencyTest::Stop() {
    loop = false;
}

bool XrdLatencyTest::WaitHosts() {
    std::map<std::string, StatResponse*>::iterator it;
    int readycount = 0;

    for (it = hosts.begin(); it != hosts.end(); it++) {
        if (it->second->IsDone()) {
            readycount++;
        }
    }

    if (hosts.size() == readycount) {
        return false;
    }
    return true;

}

std::map<std::string, StatResponse*> XrdLatencyTest::GetLatencies() {
    return hosts;
}

void XrdLatencyTest::PrintOut() {
    std::map<std::string, StatResponse*>::iterator i;
    std::string maxhost, minhost;
    double curr = 0, total = 0, min = 0, max = 0;
    int errors = 0;

    for (i = hosts.begin(); i != hosts.end(); ++i) {
        curr = i->second->GetLatency();

        if (curr > max) {
            max = curr;
            maxhost = i->first;
        }
        if (min == 0 || curr < min) {
            min = curr;
            minhost = i->first;
        }

        std::cout << minhost << " >> " << maxhost << std::endl;

        if (i->second->GetXrootdStatus().IsOK()) {
            total += curr;
        } else {
            errors++;
        }

        delete i->second;
    }

    if (!verbose) {
        std::cout << "good: " << hosts.size() - errors;
        std::cout << "    bad: " << errors;
        std::cout << "    min: " << std::setprecision(3) << std::fixed << min << "ms";
        std::cout << " (" << minhost << ")";
        std::cout << "    max: " << max << "ms";
        std::cout << " (" << maxhost << ")";
        std::cout << "    avg: ";
        std::cout << total / hosts.size() << "ms" << std::endl;
    }
}

void XrdLatencyTest::addHostsFromFile(std::string path) {
    std::ifstream in(path.c_str(), std::ios_base::in);

    if (in.good()) {
        std::string line;
        while (std::getline(in, line)) {
            addHost(line);
        }
    } else {
        std::cout << "error opening host file: " << path << std::endl;
        exit(1);
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

int XrdLatencyTest::removeHost(std::string host, int port) {
    std::stringstream ss;
    ss << port;
    removeHost(host += ":" + ss.str());
}

int XrdLatencyTest::removeHost(std::string host) {
    hosts.erase(host);
}
