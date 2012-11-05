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

#ifndef XRDLATENCYTEST_H
#define	XRDLATENCYTEST_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

class XrdLatencyTest {
public:
    XrdLatencyTest();
    virtual ~XrdLatencyTest();

    std::vector<std::string> hosts;
    std::string statpath;
    
    int statinterval;
    int floodinterval;

    int addHost(std::string host, int port);
    int addHost(std::string host);
    void printHosts();
    int removeHost(std::string host, int port);
    int removeHost(std::string host);
    int setStatPath(std::string path = "/");
    int setStatInterval(size_t seconds);
    int setFloodInterval(size_t seconds);

    void PrintOut(std::string &out, std::string &err);
    bool Start(bool do_stat, bool do_flood);
    bool Stop();

    bool GetLatencies(std::map<std::string, double> &measurement);
    bool GetFloodRate(std::map<std::string, double> &measurements);

};

#endif	/* XRDLATENCYTEST_H */

