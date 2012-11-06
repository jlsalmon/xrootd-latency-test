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

#include "StatResponseHandler.hh"
#include "XrdLatencyTest.hh"
#include <sys/time.h>
#include <iomanip>

StatResponseHandler::StatResponseHandler() : XrdCl::ResponseHandler() {
    gettimeofday(&this->reqtime, NULL);
}

StatResponseHandler::~StatResponseHandler() {
}

void StatResponseHandler::HandleResponse(XrdCl::XRootDStatus* status,
        XrdCl::AnyObject* response) {

    gettimeofday(&this->resptime, NULL);
    double diff = timediff(this->reqtime, this->resptime);

    if (status != NULL) {

        std::cout << "-------------------------------------" << std::endl;
        std::cout << "Latency: " << std::setprecision(3) << std::fixed << diff << " ms" << std::endl;
        //        std::cout << "ToStr: " << status->ToString() << std::endl;
        //        std::cout << "Code: " << status->code << std::endl;
        //        std::cout << "ErrNo: " << status->errNo << std::endl;
        //        std::cout << "Status: " << status->status << std::endl;
        //        std::cout << "ErrMsg: " << status->GetErrorMessage() << std::endl;
        //        std::cout << "ShellCode: " << status->GetShellCode() << std::endl;
        //        std::cout << "IsError: " << status->IsError() << std::endl;
        //        std::cout << "IsFatal: " << status->IsFatal() << std::endl;
    }

    if (response != NULL) {
        XrdCl::StatInfo *si = NULL;
        response->Get(si);

        //        std::cout << "Flags: " << si->GetFlags() << std::endl;
        //        std::cout << "Id: " << si->GetId() << std::endl;
        //        std::cout << "ModTime: " << si->GetModTime() << std::endl;
        //        std::cout << "ModTimeString: " << si->GetModTimeAsString() << std::endl;
        //        std::cout << "Code: " << si->GetSize() << std::endl;
        //        std::cout << "IsDir: " << si->IsDir << std::endl;
        //        std::cout << "IsReadable: " << si->IsReadable << std::endl;
        //        std::cout << "IsWritable: " << si->IsWritable << std::endl;
        //        std::cout << "Offline: " << si->Offline << std::endl;
        //        std::cout << "Other: " << si->Other << std::endl;
        //        std::cout << "POSCPending: " << si->POSCPending << std::endl;
        //        std::cout << "XBitSet: " << si->XBitSet << std::endl;
    }
}

double StatResponseHandler::timediff(struct timeval req, struct timeval resp) {
    return mstime(resp) - mstime(req);
}

double StatResponseHandler::mstime(struct timeval tv) {
    // convert tv_sec & tv_usec to millisecond
    return (tv.tv_sec * 1000000.0 + tv.tv_usec) / 1000.0;
}

