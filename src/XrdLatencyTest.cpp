/* 
 * File:   XrdLatencyTest.cpp
 * Author: jsalmon
 * 
 * Created on November 5, 2012, 10:19 AM
 */

#include "XrdLatencyTest.h"

XrdLatencyTest::XrdLatencyTest() {
}

XrdLatencyTest::~XrdLatencyTest() {
}

int XrdLatencyTest::addHost(std::string host, int port) {

}

int XrdLatencyTest::addHost(std::string host) {

}

int XrdLatencyTest::removeHost(std::string host, int port) {

}

int XrdLatencyTest::removeHost(std::string host) {

}

int XrdLatencyTest::setStatPath(std::string path) {

}

int XrdLatencyTest::setStatInterval(size_t seconds) {

}

int XrdLatencyTest::setFloodInterval(size_t seconds) {

}

void XrdLatencyTest::PrintOut(std::string& out, std::string& err) {

}

bool XrdLatencyTest::Start(bool do_stat, bool do_flood) {

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

    return 0;
}