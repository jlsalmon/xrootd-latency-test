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

XrdLatencyTest::XrdLatencyTest() {
    currenthost = "";
    statpath = "/tmp";
    proto = "root://";
    async = true;
    flood = false;
    loop = false;
    verbose = false;
    statinterval = 1;
    floodinterval = 2;
    floodcount = 100;
    cv = XrdSysCondVar(0);
}

XrdLatencyTest::XrdLatencyTest(std::string statpath,
        bool flood,
        bool loop,
        bool verbose,
        size_t statinterval,
        size_t floodinterval) {

    setStatPath(statpath);
    setFlood(flood);
    setLoop(loop);
    setStatInterval(statinterval);
    setFloodInterval(floodinterval);
    setVerbose(verbose);
}

XrdLatencyTest::~XrdLatencyTest() {
    XrdSysThread::Join(thread, NULL);
}

bool XrdLatencyTest::Start() {
    XrdSysThread::Run(&thread, XrdLatencyTest::StaticRun,
            static_cast<void *> (this), XRDSYSTHREAD_HOLD, "Latency Test Thread");
    return true;
}

void* XrdLatencyTest::StaticRun(void* args) {
    reinterpret_cast<XrdLatencyTest*> (args)->Run();
    return 0;
}

void XrdLatencyTest::Run() {
    std::map<std::string, Host*>::iterator it;
    Stat *stat = 0;
    int n;
    (flood) ? n = floodcount : n = 1;

    if (!hosts.size()) {
        std::cout << "no hosts defined." << std::endl;
        exit(0);
    }
    
    std::cout << "creating stat objects" << std::endl;
    for (it = hosts.begin(); it != hosts.end(); ++it) {
        for (int i = 0; i < n; i++) {
            (async) ? stat = new AsyncStat(&cv)
                    : stat = new SyncStat(&cv);
            it->second->stats.push_back(stat);
        }
    }
    
    
    do {
        cv.Lock();
        
        std::cout << "running stats" << std::endl;
        for (it = hosts.begin(); it != hosts.end(); ++it) {

            if (it->second->IsDisabled()) continue;

            currenthost = it->first;
            XrdCl::URL *url = new XrdCl::URL(proto + currenthost);

            hosts.at(currenthost)->DoStat(url, &statpath, n);

            delete url;
        }

        std::cout << "waiting" << std::endl;
        if (async) while (WaitHosts()) cv.Wait(5);

        PrintOut();

        cv.UnLock();

        std::cout << "sleeping" << std::endl;
        (flood) ? sleep(floodinterval) : sleep(statinterval);
    } while (loop);
}

bool XrdLatencyTest::WaitHosts() {
    std::map<std::string, Host*>::iterator it;
    unsigned int readycount = 0;

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

bool XrdLatencyTest::Stop() {
    XrdSysThread::Cancel(thread);
    loop = false;
    return true;
}

double XrdLatencyTest::GetTotalTime() {
    return GetLastResponse() - GetFirstRequest();
}

double XrdLatencyTest::GetFirstRequest() {
    double first = 0;

    std::map<std::string, Host*>::iterator i;
    for (i = hosts.begin(); i != hosts.end(); i++) {
        if (i->second->IsDisabled()) continue;

        if (first == 0 || i->second->GetFirstRequest() < first) {
            first = i->second->GetFirstRequest();
        }
    }
    return first;
}

double XrdLatencyTest::GetLastResponse() {
    double last = 0;

    std::map<std::string, Host*>::iterator i;
    for (i = hosts.begin(); i != hosts.end(); i++) {
        if (i->second->IsDisabled()) continue;

        if (last == 0 || i->second->GetLastResponse() > last) {
            last = i->second->GetLastResponse();
        }
    }
    return last;
}

std::map<std::string, Host*> XrdLatencyTest::GetLatencies() {
    return hosts;
}

void XrdLatencyTest::PrintOut() {
    // Flood mode? Show flood rate
    std::map<std::string, Host*>::iterator i;
    Host* host = 0;
    std::string maxhost, minhost;
    double curr = 0, total = 0, min = 0, max = 0;
    double rate = 0;
    unsigned int errors = 0, disabled = 0, pending = 0;

    for (i = hosts.begin(); i != hosts.end(); ++i) {
        host = i->second;

        if (host->IsDisabled()) {
            disabled++;
            if (disabled == hosts.size()) {
                std::cout << "all hosts are disabled." << std::endl;
                exit(-1);
            }
            continue;
        }

        if (!host->IsDone()) {
            pending++;
            continue;
        }

        (flood) ? curr = host->GetLatency()
                : curr = host->GetAverageLatency();
        total += curr;

        // Find maximum latency
        if (max == 0 || curr > max) {
            max = curr;
            maxhost = host->GetHostname();
        }

        // Find minimum latency
        if (min == 0 || curr < min) {
            min = curr;
            minhost = host->GetHostname();
        }

        // Calculate flood rate (in seconds)
        rate = (hosts.size() * floodcount) / (GetTotalTime() / 1000);

        std::cout << std::setprecision(3) << std::fixed;
        std::cout << "hostname: " << host->GetHostname();
        std::cout << ((flood) ? "\tavg latency: " : "\tlatency: ");
        std::cout << curr << "ms ";
        std::cout << std::endl;

    }

    std::cout << "this is: " << XrdSysThread::ID() << std::endl;
    //if (verbose) {

    std::cout << "  good: " << hosts.size() - errors - disabled;
    std::cout << "  bad: " << errors;
    std::cout << "  disabled: " << disabled;
    std::cout << "  pending: " << pending;
    std::cout << std::setprecision(3) << std::fixed;
    std::cout << "  avg: " << total / (hosts.size() - errors - disabled) << "ms";
    std::cout << "  min: " << min << "ms";
    std::cout << "  max: " << max << "ms" << std::endl;

    std::cout << "  total time for " << hosts.size() - errors - disabled;
    std::cout << " host(s) with " << ((flood) ? floodcount : 1);
    std::cout << " stat(s): " << GetTotalTime() << "ms";
    std::cout << "  rate: " << rate << std::endl;
    //}
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

void XrdLatencyTest::addHost(std::string host, int port) {
    std::stringstream ss;
    ss << port;
    addHost(host += ":" + ss.str());
}

void XrdLatencyTest::addHost(std::string hostname) {
    hosts.insert(std::make_pair(hostname, new Host(hostname, async, &cv)));
}

void XrdLatencyTest::removeHost(std::string host, int port) {
    std::stringstream ss;
    ss << port;
    removeHost(host += ":" + ss.str());
}

void XrdLatencyTest::removeHost(std::string host) {
    hosts.erase(host);
}
