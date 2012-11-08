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

#include "StatResponse.hh"
#include <sys/time.h>
#include <iomanip>
#include <iostream>

StatResponse::StatResponse() {
    this->si = 0;
    this->status = 0;
    this->done = false;
}

StatResponse::~StatResponse() {
}

void StatResponse::init() {
    gettimeofday(&this->reqtime, NULL);
}

XrdCl::StatInfo* StatResponse::GetStatInfo() {
    return this->si;
}

XrdCl::XRootDStatus StatResponse::GetXrootdStatus() {
    return this->status;
}

bool StatResponse::IsDone() {
    return this->done;
}

double StatResponse::GetLatency() {
    return timediff(this->reqtime, this->resptime);
}

std::string StatResponse::GetLatencyAsString() {
    std::stringstream sstm;
    double diff = GetLatency();
    sstm << "latency: " << std::setprecision(3) << std::fixed << diff << "ms";
    return sstm.str();
}

double StatResponse::timediff(struct timeval req, struct timeval resp) {
    return mstime(resp) - mstime(req);
}

double StatResponse::mstime(struct timeval tv) {
    // convert tv_sec & tv_usec to millisecond
    return (tv.tv_sec * 1000000.0 + tv.tv_usec) / 1000.0;
}
