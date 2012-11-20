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

#ifndef SYNCHOST_HH
#define	SYNCHOST_HH

#include "Host.hh"

class SyncHost : public Host {
public:
    
    /**
     * Constructor which takes an XrdSysCondVar, used to avoid having
     * to have a busy-wait loop to check when all the async responses 
     * are back. 
     * 
     * @param cv: condition variable to use
     */
    SyncHost(XrdSysCondVar *cv);
    
    /**
     * Destructor (unused)
     */
    virtual ~SyncHost();
    
    /**
     * Perform the actual stat.
     * 
     * @param url: pointer to the URL to stat.
     * @param statpath: path on the remote box to stat.
     */
    void* DoStat(XrdCl::URL *url, std::string *statpath);
};

#endif	/* SYNCHOST_HH */
