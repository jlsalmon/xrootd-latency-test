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

using namespace std;

extern "C" void __libc_freeres(void);

void usage() {
    cout << "usage: xrd-latency-test [-l] [-S] [-F] [-p stat-path] ";
    cout << "[-s stat-int] [-f flood-int] [-c flood-count] [-v] [-h] host-file          " << endl;
    cout << "                                                                           " << endl;
    cout << "positional arguments:                                                      " << endl;
    cout << "   host-file       path to a file containing hostname:port combinations    " << endl;
    cout << "                                                                           " << endl;
    cout << "optional arguments:                                                        " << endl;
    cout << "   -p stat-path    path to stat on the remote machine                      " << endl;
    cout << "   -s stat-int     time delay between stats if in stat mode                " << endl;
    cout << "   -f flood-int    time delay between stat floods if in flood mode         " << endl;
    cout << "   -c flood-count  number of stats in a flood                              " << endl;
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
            case 'c':
                xrdlt.setFloodCount(atoi(optarg));
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
                cout << "unknown option: " << (char) c << endl;
                usage();
                return false;
        }
    }

    if (optind < argc) {
        string hostfile = argv[optind++];
        cout << "host file:            " << hostfile << endl;
        xrdlt.addHostsFromFile(hostfile);

    } else {
        cout << "hosts file empty or none given." << endl;
        return false;
    }

    return true;
}

int main(int argc, const char* argv[]) {
    atexit(__libc_freeres);

    XrdLatencyTest *xrdlt = new XrdLatencyTest();


    if (parseargs(*xrdlt, argc, argv)) {

        if (xrdlt->verbose) {
            cout << "host count:           " << xrdlt->hosts.size() << endl;
            cout << "stat path:            " << xrdlt->statpath << endl;
            cout << "asynchronous:         " << boolalpha << xrdlt->async << endl;
            cout << "flood mode:           " << boolalpha << xrdlt->flood << endl;
            cout << "loop mode:            " << boolalpha << xrdlt->loop << endl;
            cout << "stat interval:        " << xrdlt->statinterval << endl;
            cout << "flood interval:       " << xrdlt->floodinterval << endl;
            cout << "flood count:          " << xrdlt->floodcount << endl;
        }

        // Start the show
        //xrdlt->Start();
        // We're running from the command line, so wait for the thread
        // to exit (or ctrl-C)
        //XrdSysThread::Join(xrdlt->thread, NULL);
        //delete xrdlt;

    } else {
        cout << "type \"xrd-latency-test -h\" for help." << endl;
        exit(EXIT_FAILURE);
    }



    xrdlt->Start();
    if (0) {
   // if (!fork()) {
        XrdLatencyTest *xrdlt2 = new XrdLatencyTest();
        parseargs(*xrdlt2, argc, argv);
        xrdlt2->setLoop(true);
        xrdlt2->setAsync(xrdlt->async);
        xrdlt2->setFlood(true);
        xrdlt2->Start();

        cout << "host count:           " << xrdlt2->hosts.size() << endl;
        cout << "stat path:            " << xrdlt2->statpath << endl;
        cout << "asynchronous:         " << boolalpha << xrdlt2->async << endl;
        cout << "flood mode:           " << boolalpha << xrdlt2->flood << endl;
        cout << "loop mode:            " << boolalpha << xrdlt2->loop << endl;
        cout << "stat interval:        " << xrdlt2->statinterval << endl;
        cout << "flood interval:       " << xrdlt2->floodinterval << endl;
        cout << "flood count:          " << xrdlt2->floodcount << endl;
        XrdSysThread::Join(xrdlt2->thread, NULL);
        //exit(0);
    }

    XrdSysThread::Join(xrdlt->thread, NULL);


    exit(EXIT_SUCCESS);
}

