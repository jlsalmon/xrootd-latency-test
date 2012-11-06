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
#include <string>
#include <cstdlib>
#include <iostream>

void usage() {
    std::cout << "Usage: xrd-latency-test <options>" << std::endl;
}

int main(int argc, const char* argv[]) {

    XrdLatencyTest *xrdlt = new XrdLatencyTest();

    int c;

    while ((c = getopt(argc, (char**) argv, "p:s:f:lvSFh?")) != -1) {
        switch (c) {
            case 'p':
                xrdlt->setStatPath(optarg);
                break;
            case 's':
                xrdlt->setStatInterval(atoi(optarg));
                break;
            case 'f':
                xrdlt->setFloodInterval(atoi(optarg));
                break;
            case 'F':
                xrdlt->setFlood(true);
                break;
            case 'l':
                xrdlt->setLoop(true);
                break;
            case 'S':
                xrdlt->setAsync(false);
                break;
            case 'v':
                xrdlt->setVerbose(true);
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

    xrdlt->addHost("eosdev", 1094);

    xrdlt->printHosts();
    std::cout << "------------------------" << std::endl;

    std::cout << "Stat path: " << xrdlt->statpath << std::endl;
    std::cout << "Loop: " << xrdlt->loop << std::endl;
    std::cout << "Flood: " << xrdlt->flood << std::endl;
    std::cout << "Stat interval: " << xrdlt->statinterval << std::endl;
    std::cout << "Flood interval: " << xrdlt->floodinterval << std::endl;

    xrdlt->Start();

    std::cin.get();
    return 0;
}

