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

#include "Host.hh"

Host::Host(std::string hostname, bool async, XrdSysSemaphore* sem) {
    this->hostname = hostname;
    this->async = async;
    this->sem = sem;
    disabled = false;
}

Host::~Host() {
    while(!stats.empty()) delete stats.back(), stats.pop_back();
}

void Host::DoStat(XrdCl::URL* url, std::string statpath, int n) {
    Reset();

    for (int i = 0; i < n; i++) {
        stats.at(i)->Run(url, statpath);

        if (stats.front()->IsBad()) {
            SetDisabled(true);
            break;
        }
    }
}

void Host::Reset() {
    std::vector<Stat*>::iterator i;
    for (i = stats.begin(); i != stats.end(); ++i) {
        (*i)->Reset();
    }
}

double Host::GetLatency() {
    Stat* first = stats.at(0);
    return timediff(first->GetReqTime(), first->GetRespTime());
}

double Host::GetAverageLatency() {
    double total = 0;

    std::vector<Stat*>::iterator i;
    for (i = stats.begin(); i != stats.end(); ++i) {
        total += timediff((*i)->GetReqTime(), (*i)->GetRespTime());
    }

    return total / stats.size();
}

double Host::GetFirstRequest() {
    double first = 0;

    std::vector<Stat*>::iterator i;
    for (i = stats.begin(); i != stats.end(); ++i) {
        if (first == 0 || mstime((*i)->GetReqTime()) < first) {
            first = mstime((*i)->GetReqTime());
        }
    }

    return first;
}

double Host::GetLastResponse() {
    double last = 0;

    std::vector<Stat*>::iterator i;
    for (i = stats.begin(); i != stats.end(); ++i) {
        if (last == 0 || mstime((*i)->GetRespTime()) > last) {
            last = mstime((*i)->GetRespTime());
        }
    }

    return last;
}

std::string Host::GetHostname() {
    return hostname;
}

void Host::SetDisabled(bool disabled) {
    this->disabled = disabled;
}

bool Host::IsDisabled() {
    return disabled;
}

bool Host::IsDone() {
    std::vector<Stat*>::iterator i;
    for (i = stats.begin(); i != stats.end(); ++i) {
        if ((*i)->GetXrootdStatus() != NULL
                && (*i)->GetXrootdStatus()->IsError()) {
            SetDisabled(true);
        }
        if (!(*i)->IsDone()) return false;
    }
    return true;
}

double Host::timediff(struct timeval req, struct timeval resp) {
    return mstime(resp) - mstime(req);
}

double Host::mstime(struct timeval tv) {
    return (tv.tv_sec * 1000000.0 + tv.tv_usec) / 1000.0;
}