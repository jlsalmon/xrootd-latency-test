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

#ifndef HOST_HH
#define	HOST_HH

#include "Stat.hh"
#include "AsyncStat.hh"
#include "SyncStat.hh"

#include "XrdCl/XrdClFileSystem.hh"

#include <iomanip>
#include <iostream>

class Host {
public:

    /**
     * Default constructor
     */
    Host(std::string hostname, bool async, XrdSysSemaphore* sem);

    /**
     * Destructor
     */
    virtual ~Host();

    /**
     * Tell this host to perform the actual stat(s).
     * 
     * @param url: pointer to the URL to stat.
     * @param statpath: path on the remote box to stat.
     * @param n: number of stats to send.
     */
    void DoStat(XrdCl::URL *url, std::string statpath, int n);

    /**
     * Re-initialize this host to the default state
     */
    void Reset();

    /**
     * @return the latency of the first stat performed (used if
     * not in flood mode)
     */
    double GetLatency();

    /**
     * @return the average latency for all stats (used if in
     * flood mode)
     */
    double GetAverageLatency();

    /**
     * @return first request time for this host over all stats.
     */
    double GetFirstRequest();

    /**
     * @return last response time for this host over all stats.
     */
    double GetLastResponse();

    /**
     * @return this host's hostname.
     */
    std::string GetHostname();

    /**
     * Disable this host for testing.
     */
    void SetDisabled(bool disabled);

    /**
     * @return whether this host is disabled or not.
     */
    bool IsDisabled();

    /**
     * @return whether this host has received and processed its
     * response yet or not.
     */
    bool IsDone();

    /**
     * Calculate the time difference between two times.
     * 
     * @param req: request (start) time
     * @param resp: response (end) time
     * @return time difference in milliseconds
     */
    double timediff(struct timeval req, struct timeval resp);

    /**
     * Convert tv_sec & tv_usec to milliseconds.
     * 
     * @param tv: timeval to convert
     * @return timeval in milliseconds
     */
    double mstime(struct timeval tv);

    std::string hostname;
    bool disabled;
    bool async;
    std::vector<Stat*> stats;
    XrdSysSemaphore *sem;
};

#endif	/* HOST_HH */

