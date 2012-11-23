/* 
 * File:   SyncStat.hh
 * Author: jsalmon
 *
 * Created on November 22, 2012, 2:16 PM
 */

#ifndef SYNCSTAT_HH
#define	SYNCSTAT_HH

#include "Stat.hh"

class SyncStat : public Stat {
public:

    SyncStat(XrdSysCondVar *cv) {
        this->cv = cv;
    }

    virtual ~SyncStat() {
        delete statinfo;
    }

    void Run(XrdCl::URL *url, std::string *statpath) {
        XrdCl::XRootDStatus status;
        XrdCl::FileSystem fs(*url);

        Stat::Initialize();
        status = fs.Stat(*statpath, statinfo, 5);
        Stat::Finalize();

        this->status = &status;
        if (this->status->IsError()) bad = true;
    }
};

#endif	/* SYNCSTAT_HH */

