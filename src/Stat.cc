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

#include "Stat.hh"

Stat::Stat() {
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

Stat::~Stat() {
}

void Stat::Initialize() {
    gettimeofday(&reqtime, NULL);
}

void Stat::Finalize() {
    gettimeofday(&resptime, NULL);
    done = true;
}

void Stat::Reset() {
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

bool Stat::IsDone() {
    return done;
}

bool Stat::IsBad() {
    return bad;
}

XrdCl::StatInfo* Stat::GetStatInfo() {
    return statinfo;
}

XrdCl::XRootDStatus* Stat::GetXrootdStatus() {
    return status;
}

struct timeval Stat::GetReqTime() {
    return reqtime;
}

struct timeval Stat::GetRespTime() {
    return resptime;
}
