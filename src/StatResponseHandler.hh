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

#ifndef STATRESPONSEHANDLER_HH
#define	STATRESPONSEHANDLER_HH

#include "XrdSys/XrdSysPthread.hh"
#include "XrdCl/XrdClXRootDResponses.hh"
#include <string>
#include <cstdlib>
#include <iostream>

class StatResponseHandler : public XrdCl::ResponseHandler {
public:
    StatResponseHandler();
    ~StatResponseHandler();

    /**
     * Handle response
     * 
     * @param status: status of the response
     * @param response: object containing extra info about the response
     */
    virtual void HandleResponse(XrdCl::XRootDStatus* status,
            XrdCl::AnyObject* response);
    
    /**
     * 
     * @param tv
     * @return 
     */
    double mstime(struct timeval tv);
    
    /**
     */
    double timediff(struct timeval req, struct timeval resp);

private:
    struct timeval reqtime;
    struct timeval resptime;
};

#endif	/* STATRESPONSEHANDLER_HH */

