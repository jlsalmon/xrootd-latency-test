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
    using namespace std;
    cout << "usage: xrd-latency-test [-l] [-S] [-F] [-p stat-path] ";
    cout << "[-s stat-int] [-f flood-int] [-v] [-h] host-file                           " << endl;
    cout << "                                                                           " << endl;
    cout << "positional arguments:                                                      " << endl;
    cout << "   host-file       path to a file containing hostname:port combinations    " << endl;
    cout << "                                                                           " << endl;
    cout << "optional arguments:                                                        " << endl;
    cout << "   -p stat-path    path to stat on the remote machine                      " << endl;
    cout << "   -s stat-int     time delay between stats if in stat mode                " << endl;
    cout << "   -f flood-int    time delay between stat floods if in flood mode         " << endl;
    cout << "   -l              enable loop mode                                        " << endl;
    cout << "   -S              enable synchronous mode                                 " << endl;
    cout << "   -F              enable flood mode                                       " << endl;
    cout << "   -v              be verbose                                              " << endl;
    cout << "   -h              show this help message and exit                         " << endl;
}

bool parseargs(XrdLatencyTest &xrdlt, int argc, const char* argv[]) {
    int c;

    while ((c = getopt(argc, (char**) argv, "p:s:f:lSFvh?")) != -1) {
        switch (c) {
            case 'p':
                xrdlt.setStatPath(optarg);
                break;
            case 's':
                xrdlt.setStatInterval(atoi(optarg));
                break;
            case 'f':
                xrdlt.setFloodInterval(atoi(optarg));
                break;
            case 'F':
                xrdlt.setFlood(true);
                break;
            case 'l':
                xrdlt.setLoop(true);
                break;
            case 'S':
                xrdlt.setAsync(false);
                break;
            case 'v':
                xrdlt.setVerbose(true);
                break;
            case 'h':
            case '?':
                usage();
                exit(EXIT_SUCCESS);
            default:
                std::cout << "unknown option: " << (char) c << std::endl;
                usage();
                return false;
        }
    }

    if (optind < argc) {
        std::string hostfile = argv[optind++];
        std::cout << "host file:            " << hostfile << std::endl;
        xrdlt.addHostsFromFile(hostfile);
        
    } else {
        std::cout << "hosts file empty or none given." << std::endl;
        return false;
    }

    return true;
}

int main(int argc, const char* argv[]) {

    XrdLatencyTest *xrdlt = new XrdLatencyTest();

    if (parseargs(*xrdlt, argc, argv)) {
        
        xrdlt->Start();
        XrdSysThread::Join(xrdlt->thread, NULL);
    
    } else {
        std::cout << "type \"xrd-latency-test -h\" for help." << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

