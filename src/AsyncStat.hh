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

#ifndef ASYNCSTAT_HH
#define	ASYNCSTAT_HH

#include "Stat.hh"

class AsyncStat : public Stat, XrdCl::ResponseHandler {
public:

    /**
     * @param cv: condition variable to monitor responses
     */
    AsyncStat(XrdSysCondVar *cv);
    virtual ~AsyncStat();

    /**
     * Perform the actual stat (async).
     * 
     * @param url: the URL to stat
     * @param statpath: path on the remote box to stat
     */
    void Run(XrdCl::URL *url, std::string *statpath);

    /**
     * Re-initialize this stat for re-use
     */
    void Reset();

    /**
     * Asynchronous response handler.
     * 
     * @param status: status of the response
     * @param response: object containing extra info about the response
     */
    void HandleResponse(XrdCl::XRootDStatus* status,
            XrdCl::AnyObject* response);
};

#endif	/* ASYNCSTAT_HH */

