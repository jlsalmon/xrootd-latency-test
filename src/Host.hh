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
#include "XrdSys/XrdSysPthread.hh"
#include <sys/time.h>

class Host {
public:
    Host();
    virtual ~Host();

    /**
     * 
     * @param fs
     */
    virtual void DoStat(XrdCl::FileSystem &fs, std::string statpath) = 0;

    /**
     * 
     */
    void Init() {
        gettimeofday(&reqtime, NULL);
        done = false;
    }

    /**
     * 
     * @return 
     */
    XrdCl::StatInfo* GetStatInfo() {
        return statinfo;
    }

    /**
     * 
     * @return 
     */
    XrdCl::XRootDStatus GetXrootdStatus() {
        return status;
    }

    /**
     * 
     */
    void Enable() {
        enabled = true;
    }

    /**
     * 
     */
    void Disable() {
        enabled = false;
    }

    /**
     * 
     * @return 
     */
    bool IsEnabled() {
        return enabled;
    }

    /**
     * 
     * @return 
     */
    bool IsDone() {
        return done;
    }

    /**
     * 
     * @return 
     */
    double GetLatency();

    /**
     * 
     * @return 
     */
    std::string GetLatencyAsString();

private:
    /**
     * convert tv_sec & tv_usec to millisecond
     * 
     * @param tv
     * @return 
     */
    double mstime(struct timeval tv);

    /**
     * 
     * @param req
     * @param resp
     * @return 
     */
    double timediff(struct timeval req, struct timeval resp);

protected:
    bool done;
    bool enabled;
    XrdCl::StatInfo *statinfo;
    XrdCl::XRootDStatus status;
    XrdSysCondVar cv;
    struct timeval reqtime;
    struct timeval resptime;
};

#endif	/* HOST_HH */

