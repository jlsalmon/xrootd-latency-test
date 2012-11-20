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

#include "AsyncHost.hh"

AsyncHost::AsyncHost(XrdSysCondVar *cv) {
    this->cv = cv;
}

AsyncHost::~AsyncHost() {
}

void* AsyncHost::DoStat(XrdCl::URL *url, std::string *statpath) {
    XrdCl::FileSystem fs(*url);
    Host::Initialize();
    fs.Stat(*statpath, this, 5);
}

void AsyncHost::HandleResponse(XrdCl::XRootDStatus* status,
        XrdCl::AnyObject* response) {

    cv->Lock();
    Host::Finalize();

    if (response != NULL) response->Get(statinfo);
    this->status = *status;
    if (this->status->IsError()) Disable();

    cv->Signal();
    cv->UnLock();
}
