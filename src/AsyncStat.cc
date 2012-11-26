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

#include <xrootd/XrdSys/XrdSysPthread.hh>

#include "AsyncStat.hh"

AsyncStat::AsyncStat(XrdSysSemaphore *sem) {
    this->sem = sem;
}

AsyncStat::~AsyncStat() {
    delete response;
}

void AsyncStat::Run(XrdCl::URL *url, std::string *statpath) {
    XrdCl::FileSystem fs(*url);

    Stat::Initialize();
    fs.Stat(*statpath, this, 1);
}

void AsyncStat::Reset() {
    delete response;
    delete status;
    Stat::Reset();
}

void AsyncStat::HandleResponse(XrdCl::XRootDStatus* status,
        XrdCl::AnyObject* response) {

    Stat::Finalize();

    if (response != NULL) {
        this->response = response;
        response->Get(statinfo);
    }

    this->status = status;
    if (this->status->IsError()) bad = true;

    sem->Post();
}
