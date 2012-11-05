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
#include "StatResponseHandler.hh"
#include "XrdCl/XrdClFileSystem.hh"
#include "XrdCl/XrdClURL.hh"
#include "XrdSys/XrdSysPthread.hh"

XrdLatencyTest::XrdLatencyTest() {
}

XrdLatencyTest::~XrdLatencyTest() {
}

int XrdLatencyTest::addHost(std::string host, int port) {
    std::stringstream ss;
    ss << port;
    addHost(host += ":" + ss.str());
}

int XrdLatencyTest::addHost(std::string host) {
    hosts.push_back(host);
}

void XrdLatencyTest::printHosts() {
    std::vector<std::string>::const_iterator i;

    for (i = hosts.begin(); i != hosts.end(); ++i)
        std::cout << *i << std::endl;
}

int XrdLatencyTest::removeHost(std::string host, int port) {
    std::stringstream ss;
    ss << port;
    removeHost(host += ":" + ss.str());
}

int XrdLatencyTest::removeHost(std::string host) {
    hosts.erase(std::remove(hosts.begin(), hosts.end(), host), hosts.end());
}

int XrdLatencyTest::setStatPath(std::string path) {
    statpath = path;
}

int XrdLatencyTest::setStatInterval(size_t seconds) {
    statinterval = seconds;
}

int XrdLatencyTest::setFloodInterval(size_t seconds) {
    floodinterval = seconds;
}

void XrdLatencyTest::PrintOut(std::string& out, std::string& err) {

}

bool XrdLatencyTest::Start(bool do_stat, bool do_flood) {

    using namespace XrdCl;

    std::string proto = "root://";
    int n;

    if (do_stat) n = 1;
    else if (do_flood) n = 0;

    for (int i = 0; i < hosts.size(); i++) {
        URL url(proto + hosts[i]);
        FileSystem fs(url);
        ResponseHandler *srh = new StatResponseHandler();
        XRootDStatus xrds;

        for (int i = 0; i < n; i++) {
            xrds = fs.Stat(statpath, srh, 10);
        }
    }
}

bool XrdLatencyTest::Stop() {

}

bool XrdLatencyTest::GetLatencies(std::map<std::string, double>& measurement) {

}

bool XrdLatencyTest::GetFloodRate(std::map<std::string, double>& measurements) {

}

void usage() {
    std::cout << "Usage: xrd-latency-test <options>" << std::endl;
}

int main(int argc, const char* argv[]) {

    std::string path = "/";
    size_t statinterval = 1;
    size_t floodinterval = 10;
    bool flood = false;
    bool loop = false;
    bool verbose = false;

    int c;

    while ((c = getopt(argc, (char**) argv, "p:s:f:lvhF?")) != -1) {
        switch (c) {
            case 'p':
                path = optarg;
                break;
            case 's':
                statinterval = atoi(optarg);
                break;
            case 'f':
                floodinterval = atoi(optarg);
                break;
            case 'F':
                flood = true;
                break;
            case 'l':
                loop = true;
                break;
            case 'v':
                verbose = true;
                break;
            case 'h':
            case '?':
                usage();
                return EXIT_SUCCESS;
            default:
                std::cout << "Unknown option: " << (char) c << std::endl;
                usage();
                return EXIT_FAILURE;
        }
    }

    XrdLatencyTest *xrdlt = new XrdLatencyTest();

    xrdlt->addHost("vagabond", 1094);
    xrdlt->addHost("vagabond", 1094);
    xrdlt->addHost("vagabond:1094");

    xrdlt->printHosts();
    std::cout << "------------------------" << std::endl;

    xrdlt->setStatPath(path);
    std::cout << "Stat path: " << xrdlt->statpath << std::endl;

    xrdlt->setStatInterval(statinterval);
    std::cout << "Stat interval: " << xrdlt->statinterval << std::endl;

    xrdlt->setFloodInterval(floodinterval);
    std::cout << "Flood interval: " << xrdlt->floodinterval << std::endl;

    xrdlt->Start(!flood, flood);

    std::cin.get();
    return 0;
}
