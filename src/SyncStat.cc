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

#include "SyncStat.hh"

SyncStat::SyncStat(XrdSysCondVar *cv) {
    this->cv = cv;
}

SyncStat::~SyncStat() {
    delete statinfo;
}

void SyncStat::Run(XrdCl::URL *url, std::string *statpath) {
    XrdCl::XRootDStatus status;
    XrdCl::FileSystem fs(*url);

    Stat::Initialize();
    status = fs.Stat(*statpath, statinfo, 5);
    Stat::Finalize();

    this->status = &status;
    if (this->status->IsError()) bad = true;
}

void SyncStat::Reset() {
    delete statinfo;
    delete response;
    Stat::Reset();
}
