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
#include "AsyncHost.hh"
#include "SyncHost.hh"

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
    XrdSysThread::Join(thread, NULL);
}

bool XrdLatencyTest::Start() {
    XrdSysThread::Run(&thread, XrdLatencyTest::StaticRun,
            static_cast<void *> (this), XRDSYSTHREAD_HOLD, "Latency Test Thread");
}

void* XrdLatencyTest::StaticRun(void* args) {
    reinterpret_cast<XrdLatencyTest*> (args)->Run();
}

void XrdLatencyTest::Run() {
    using namespace XrdCl;

    if (!hosts.size()) {
        std::cout << "no hosts defined." << std::endl;
        exit(0);
    }

    if (verbose) {
        std::cout << "host count:           " << hosts.size() << std::endl;
        std::cout << "stat path:            " << statpath << std::endl;
        std::cout << "asynchronous:         " << std::boolalpha << async << std::endl;
        std::cout << "flood mode:           " << std::boolalpha << flood << std::endl;
        std::cout << "loop mode:            " << std::boolalpha << loop << std::endl;
        std::cout << "stat interval:        " << statinterval << std::endl;
        std::cout << "flood interval:       " << floodinterval << std::endl;
    }

    std::map<std::string, Host*>::iterator it;
    int n = 1;
    if (flood) n = 100;

    do {
        for (it = hosts.begin(); it != hosts.end(); ++it) {

            if (it->second->IsDisabled()) continue;

            currenthost = it->first;
            URL *url = new URL(proto + currenthost);

            cv.Lock();
            for (int i = 0; i < n; i++) {

                hosts.at(currenthost)->DoStat(url, &statpath);
                if (async) cv.WaitMS(100);

            }
            cv.UnLock();

            delete url;
        }

        PrintOut();
        sleep(statinterval);
    } while (loop);
}

bool XrdLatencyTest::Stop() {
    XrdSysThread::Cancel(thread);
    loop = false;
}

std::map<std::string, Host*> XrdLatencyTest::GetLatencies() {
    return hosts;
}

void XrdLatencyTest::PrintOut() {
    // Flood mode? Show flood rate
    std::map<std::string, Host*>::iterator i;
    std::string maxhost, minhost;
    double curr = 0, total = 0, min = 0, max = 0;
    int errors = 0, disabled = 0, pending = 0;

    for (i = hosts.begin(); i != hosts.end(); ++i) {

        if (i->second->IsDisabled()) {
            disabled++;
            continue;
        }

        if (!i->second->IsDone()) {
            pending++;
            continue;
        }

        if (curr > max) {
            max = curr;
            maxhost = i->first;
        }

        if (min == 0 || curr < min) {
            min = curr;
            minhost = i->first;
        }

        if (i->second->GetXrootdStatus().IsOK()) {
            total += curr;
        } else {
            errors++;
        }

        curr = i->second->GetLatency();
        std::cout << "hostname: " << i->first << "\tlatency: ";
        std::cout << curr << "ms" << std::endl;
    }

    if (verbose) {
        std::cout << "   good: " << hosts.size() - errors - disabled;
        std::cout << "   bad: " << errors;
        std::cout << "   disabled: " << disabled;
        std::cout << "   pending: " << pending;
        std::cout << std::setprecision(3) << std::fixed;
        std::cout << "   avg: " << total / hosts.size() << "ms";
        std::cout << "   min: " << min << "ms";
        std::cout << "   max: " << max << "ms" << std::endl;
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

int XrdLatencyTest::addHost(std::string hostname) {
    Host *host = 0;

    if (this->async) {
        host = new AsyncHost(&cv);
    } else {
        host = new SyncHost(&cv);
    }

    hosts.insert(std::make_pair(hostname, host));
}

int XrdLatencyTest::removeHost(std::string host, int port) {
    std::stringstream ss;
    ss << port;
    removeHost(host += ":" + ss.str());
}

int XrdLatencyTest::removeHost(std::string host) {
    hosts.erase(host);
}
