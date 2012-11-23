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

#include "Host.hh"

#include "XrdCl/XrdClURL.hh"
#include "XrdCl/XrdClFileSystem.hh"
#include "XrdSys/XrdSysPthread.hh"

#include <iostream>
#include <sys/time.h>

class Stat {
public:

    Stat() {
        response = 0;
        statinfo = 0;
        status = 0;
        done = false;
        bad = false;
        reqtime.tv_sec = 0;
        reqtime.tv_usec = 0;
        resptime.tv_sec = 0;
        resptime.tv_usec = 0;
    }

    virtual ~Stat() {
    }

    void Initialize() {
        gettimeofday(&reqtime, NULL);
    }

    /**
     * 
     * @param url
     * @param statpath
     */
    virtual void Run(XrdCl::URL *url, std::string *statpath) = 0;

    /**
     * 
     */
    void Finalize() {
        gettimeofday(&resptime, NULL);
        done = true;
    }
    
    virtual void Reset() {
        response = 0;
        statinfo = 0;
        status = 0;
        done = false;
        bad = false;
        reqtime.tv_sec = 0;
        reqtime.tv_usec = 0;
        resptime.tv_sec = 0;
        resptime.tv_usec = 0;
    }

    bool IsDone() {
        return done;
    }

    bool IsBad() {
        return bad;
    }

    /**
     * @return the StatInfo object associated with this stat
     */
    XrdCl::StatInfo* GetStatInfo() {
        return statinfo;
    }

    /**
     * @return the XRootDStatus object associated with this stat
     */
    XrdCl::XRootDStatus* GetXrootdStatus() {
        return status;
    }

    /**
     * @return this stat's request time
     */
    struct timeval GetReqTime() {
        return reqtime;
    }

    /**
     * @return this stat's response time
     */
    struct timeval GetRespTime() {
        return resptime;
    }

protected:
    bool done;
    bool bad;
    XrdCl::AnyObject *response;
    XrdCl::StatInfo *statinfo;
    XrdCl::XRootDStatus *status;
    XrdSysCondVar *cv;
    struct timeval reqtime;
    struct timeval resptime;
};


#endif	/* STAT_HH */

