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

#ifndef ASYNCSTATRESPONSE_HH
#define	ASYNCSTATRESPONSE_HH

#include "StatResponse.hh"


class AsyncStatResponse : public StatResponse, public XrdCl::ResponseHandler {
public:
    AsyncStatResponse();
    virtual ~AsyncStatResponse();
    
    /**
     * 
     */
    void init();
    
    /**
     * 
     * @param 
     * @param statpath
     */
    void DoStat(XrdCl::FileSystem, std::string statpath);
    
    /**
     * Handle response
     * 
     * @param status: status of the response
     * @param response: object containing extra info about the response
     */
    virtual void HandleResponse(XrdCl::XRootDStatus* status,
            XrdCl::AnyObject* response);
private:

};

#endif	/* ASYNCSTATRESPONSE_HH */

