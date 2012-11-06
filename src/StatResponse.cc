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


StatResponse::StatResponse() {
    gettimeofday(&this->reqtime, NULL);
}

StatResponse::~StatResponse() {
}

void StatResponse::init() {
    gettimeofday(&this->reqtime, NULL);
}

double StatResponse::timediff(struct timeval req, struct timeval resp) {
    return mstime(resp) - mstime(req);
}

double StatResponse::mstime(struct timeval tv) {
    // convert tv_sec & tv_usec to millisecond
    return (tv.tv_sec * 1000000.0 + tv.tv_usec) / 1000.0;
}
