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

#ifndef STATRESPONSE_HH
#define	STATRESPONSE_HH

#include "XrdCl/XrdClFileSystem.hh"
#include "XrdSys/XrdSysPthread.hh"
#include <sys/time.h>

class StatResponse {
public:
    StatResponse();
    virtual ~StatResponse();

    /**
     * 
     */
    void init();

    /**
     * 
     * @param fs
     */
    virtual void DoStat(XrdCl::FileSystem &fs, std::string statpath) = 0;

    /**
     * 
     * @return 
     */
    XrdCl::StatInfo* GetStatInfo();

    /**
     * 
     * @return 
     */
    XrdCl::XRootDStatus GetXrootdStatus();

    /**
     * 
     * @return 
     */
    bool IsDone();

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

    /**
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
    XrdCl::StatInfo *si;
    XrdCl::XRootDStatus status;
    XrdSysCondVar cv;
    struct timeval reqtime;
    struct timeval resptime;
};

#endif	/* STATRESPONSE_HH */

