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

#include "XrdCl/XrdClFileSystem.hh"

#include <sys/time.h>
#include <iomanip>
#include <iostream>

class Host {
public:

    /**
     * Default constructor
     */
    Host();

    /**
     * Destructor (unused)
     */
    virtual ~Host();

    /**
     * Abstract function declaration to perform the actual stat.
     * 
     * @param url: pointer to the URL to stat.
     * @param statpath: path on the remote box to stat.
     */
    virtual void* DoStat(XrdCl::URL *url, std::string *statpath) = 0;

    /**
     * Initialize this host
     */
    void Init() {
        gettimeofday(&reqtime, NULL);
        done = false;
    }

    /**
     * Grab the current StatInfo object from this host.
     * 
     * @return latest StatInfo object 
     */
    XrdCl::StatInfo* GetStatInfo() {
        return statinfo;
    }

    /**
     * Grab the current XRootDStatus object from this host.
     * 
     * @return latest XRootDStatus object
     */
    XrdCl::XRootDStatus GetXrootdStatus() {
        return status;
    }

    /**
     * Enable this host for testing.
     */
    void Enable() {
        disabled = false;
    }

    /**
     * Disable this host for testing.
     */
    void Disable() {
        disabled = true;
    }

    /**
     * @return whether this host is disabled or not.
     */
    bool IsDisabled() {
        return disabled;
    }

    /**
     * @return whether this host has received and processed its
     * response yet or not.
     */
    bool IsDone() {
        return done;
    }

    /**
     * @return the most recent latency value for this host.
     */
    double GetLatency();

    /**
     * @return the most recent latency value for this host as a string.
     */
    std::string GetLatencyAsString();

private:

    /**
     * Convert tv_sec & tv_usec to milliseconds.
     * 
     * @param tv: timeval to convert
     * @return timeval in milliseconds
     */
    double mstime(struct timeval tv);

    /**
     * Calculate the time difference between two times.
     * 
     * @param req: request (start) time
     * @param resp: response (end) time
     * @return time difference in milliseconds
     */
    double timediff(struct timeval req, struct timeval resp);

protected:
    bool done;
    bool disabled;
    XrdCl::StatInfo *statinfo;
    XrdCl::XRootDStatus status;
    XrdSysCondVar *cv;
    struct timeval reqtime;
    struct timeval resptime;
};

#endif	/* HOST_HH */

