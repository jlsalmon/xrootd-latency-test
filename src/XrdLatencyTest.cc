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
}

XrdLatencyTest::~XrdLatencyTest() {
}

/**
 * 
 * @param host
 * @param port
 * @return 
 */
int XrdLatencyTest::addHost(std::string host, int port) {
    std::stringstream ss;
    ss << port;
    addHost(host += ":" + ss.str());
}

/**
 * 
 * @param host "<node>[:port]"
 * @return 
 */
int XrdLatencyTest::addHost(std::string host) {
    hosts.push_back(host);
}

/**
 * 
 */
void XrdLatencyTest::printHosts() {
    std::vector<std::string>::const_iterator i;

    for (i = hosts.begin(); i != hosts.end(); ++i)
        std::cout << *i << std::endl;
}

/**
 * 
 * @param host
 * @param port
 * @return 
 */
int XrdLatencyTest::removeHost(std::string host, int port) {
    std::stringstream ss;
    ss << port;
    removeHost(host += ":" + ss.str());
}

/**
 * 
 * @param host
 * @return 
 */
int XrdLatencyTest::removeHost(std::string host) {
    hosts.erase(std::remove(hosts.begin(), hosts.end(), host), hosts.end());
}

/**
 * 
 * @param path
 * @return 
 */
int XrdLatencyTest::setStatPath(std::string path) {
    statpath = path;
}

/**
 * 
 * @param seconds
 * @return 
 */
int XrdLatencyTest::setStatInterval(size_t seconds) {
    statinterval = seconds;
}

/**
 * 
 * @param seconds
 * @return 
 */
int XrdLatencyTest::setFloodInterval(size_t seconds) {
    floodinterval = seconds;
}

/**
 * Print the latest results to out and errors to err
 * 
 * @param out
 * @param err
 */
void XrdLatencyTest::PrintOut(std::string& out, std::string& err) {

}

/**
 * Start the thread doing the measurements optionally with stat or flood option 
 * 
 * @param do_stat
 * @param do_flood
 * @return 
 */
bool XrdLatencyTest::Start(bool do_stat, bool do_flood) {

}

/**
 * Stop the thread doing the measurements
 *
 * @return 
 */
bool XrdLatencyTest::Stop() {

}

/**
 * 
 * @param measurement
 * @return true if measured, false if nothing measured
 */
bool XrdLatencyTest::GetLatencies(std::map<std::string, double>& measurement) {

}

/**
 * 
 * @param measurements
 * @return true if measured, false if nothing measured
 */
bool XrdLatencyTest::GetFloodRate(std::map<std::string, double>& measurements) {

}

void usage() {
    std::cout << "Usage: xrd-latency-test <options>" << std::endl;
}

int main(int argc, const char* argv[]) {

    bool verbose = 0;
    int c;

    while ((c = getopt(argc, (char**) argv, "ovh?")) != -1) {
        switch (c) {
            case 'o':
                std::cout << "One-shot mode" << std::endl;
                break;
            case 'v':
                std::cout << "Verbose" << std::endl;
                verbose = 1;
                break;
            case 'h':
            case '?':
                usage();
                break;
            default:
                std::cout << "Unknown option: " << (char) c << std::endl;
                usage;
                break;
        }
    }

    XrdLatencyTest *xrdlt = new XrdLatencyTest();

    xrdlt->addHost("manager.xrd.test", 1094);
    xrdlt->addHost("ds1.xrd.test", 1094);
    xrdlt->addHost("ds2.xrd.test:1094");

    xrdlt->printHosts();
    std::cout << "------------------------" << std::endl;

    xrdlt->removeHost("ds1.xrd.test:1094");
    xrdlt->removeHost("ds2.xrd.test", 1094);

    xrdlt->printHosts();
    std::cout << "------------------------" << std::endl;

    xrdlt->setStatPath("/data");
    std::cout << "Stat path: " << xrdlt->statpath;

    xrdlt->setStatInterval(5);
    std::cout << "Stat interval: " << xrdlt->statinterval;

    xrdlt->setFloodInterval(5);
    std::cout << "Flood interval: " << xrdlt->floodinterval;

    return 0;
}
