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

#ifndef STAT_HH
#define	STAT_HH

#include "XrdCl/XrdClURL.hh"
#include "XrdCl/XrdClFileSystem.hh"
#include "XrdSys/XrdSysPthread.hh"

#include <iostream>
#include <sys/time.h>

class Stat {
public:

    /**
     * Constructor - initialize member variables
     */
    Stat();

    virtual ~Stat();
    
    /**
     * Record the request time
     */
    void Initialize();

    /**
     * Abstract declaration to perform the actual stat.
     * 
     * @param url: the URL to stat
     * @param statpath: path on the remote box to stat
     */
    virtual void Run(XrdCl::URL *url, std::string statpath) = 0;

    /**
     * Record the response time and mark this stat as done.
     */
    void Finalize();
    
    /**
     * Re-initialize this stat for re-use
     */
    void Reset();

    /**
     * @return whether the response has been received yet.
     */
    bool IsDone();

    /**
     * @return whether this stat's response had an error.
     */
    bool IsBad();

    /**
     * @return the StatInfo object associated with this stat
     */
    XrdCl::StatInfo* GetStatInfo();

    /**
     * @return the XRootDStatus object associated with this stat
     */
    XrdCl::XRootDStatus* GetXrootdStatus();

    /**
     * @return this stat's request time
     */
    struct timeval GetReqTime();

    /**
     * @return this stat's response time
     */
    struct timeval GetRespTime();

protected:
    bool done;
    bool bad;
    XrdCl::AnyObject *response;
    XrdCl::StatInfo *statinfo;
    XrdCl::XRootDStatus *status;
    XrdSysSemaphore *sem;
    struct timeval reqtime;
    struct timeval resptime;
};


#endif	/* STAT_HH */

