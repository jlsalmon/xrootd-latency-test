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

#ifndef HOST_HH
#define	HOST_HH

#include "Stat.hh"
#include "AsyncStat.hh"
#include "SyncStat.hh"

#include "XrdCl/XrdClFileSystem.hh"

#include <iomanip>
#include <iostream>

class Host {
public:

    /**
     * Default constructor
     */
    Host(std::string hostname, bool async, XrdSysCondVar* cv) {
        this->hostname = hostname;
        this->async = async;
        this->cv = cv;
        disabled = false;
    }

    /**
     * Destructor (unused)
     */
    virtual ~Host() {
    };

    /**
     * Perform the actual stat.
     * 
     * @param url: pointer to the URL to stat.
     * @param statpath: path on the remote box to stat.
     * @param n: number of stats to send.
     */
    void DoStat(XrdCl::URL *url, std::string *statpath, int n) {
        Reset();
        
        for (int i = 0; i < n; i++) {
            stats.at(i)->Run(url, statpath);

            if (stats.front()->IsBad()) {
                SetDisabled(true);
                break;
            }
        }
    }

    void Reset() {
        std::vector<Stat*>::iterator i;
        for (i = stats.begin(); i != stats.end(); ++i) {
            (*i)->Reset();
        }
    }

    double GetLatency() {
        Stat* first = stats.at(0);
        return timediff(first->GetReqTime(), first->GetRespTime());
    }

    double GetAverageLatency() {
        double total = 0;

        std::vector<Stat*>::iterator i;
        for (i = stats.begin(); i != stats.end(); ++i) {
            total += timediff((*i)->GetReqTime(), (*i)->GetRespTime());
        }

        return total / stats.size();
    }

    double GetFloodRate() {

    }

    double GetFirstRequest() {
        double first = 0;

        std::vector<Stat*>::iterator i;
        for (i = stats.begin(); i != stats.end(); ++i) {
            if (first == 0 || mstime((*i)->GetReqTime()) < first) {
                first = mstime((*i)->GetReqTime());
            }
        }

        return first;
    }

    double GetLastResponse() {
        double last = 0;

        std::vector<Stat*>::iterator i;
        for (i = stats.begin(); i != stats.end(); ++i) {
            if (last == 0 || mstime((*i)->GetRespTime()) > last) {
                last = mstime((*i)->GetRespTime());
            }
        }

        return last;
    }

    std::string GetHostname() {
        return hostname;
    }

    /**
     * Disable this host for testing.
     */
    void SetDisabled(bool disabled) {
        this->disabled = disabled;
    }

    /**
     * @return whether this host is disabled or not.
     */
    bool IsDisabled() {
        return disabled;
    }

    /**
     * @return whether this host has received and processed its
     * response yet or not.
     */
    bool IsDone() {
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

    /**
     * Calculate the time difference between two times.
     * 
     * @param req: request (start) time
     * @param resp: response (end) time
     * @return time difference in milliseconds
     */
    double timediff(struct timeval req, struct timeval resp) {
        return mstime(resp) - mstime(req);
    }

    /**
     * Convert tv_sec & tv_usec to milliseconds.
     * 
     * @param tv: timeval to convert
     * @return timeval in milliseconds
     */
    double mstime(struct timeval tv) {
        return (tv.tv_sec * 1000000.0 + tv.tv_usec) / 1000.0;
    }

    std::string hostname;
    bool disabled;
    bool async;
    std::vector<Stat*> stats;
    XrdSysCondVar *cv;
};

#endif	/* HOST_HH */

