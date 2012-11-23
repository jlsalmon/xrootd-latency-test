/* 
 * File:   AsyncStat.hh
 * Author: jsalmon
 *
 * Created on November 22, 2012, 2:16 PM
 */

#ifndef ASYNCSTAT_HH
#define	ASYNCSTAT_HH

#include "Stat.hh"

class AsyncStat : public Stat, XrdCl::ResponseHandler {
public:

    AsyncStat(XrdSysCondVar *cv) {
        this->cv = cv;
    }

    virtual ~AsyncStat() {
        delete response;
    }

    void Run(XrdCl::URL *url, std::string *statpath) {
        XrdCl::FileSystem fs(*url);

        Stat::Initialize();
        fs.Stat(*statpath, this, 5);
    }

    void HandleResponse(XrdCl::XRootDStatus* status,
            XrdCl::AnyObject* response) {

        cv->Lock();
        Stat::Finalize();

        if (response != NULL) {
            this->response = response;
            response->Get(statinfo);
        }

        this->status = status;
        if (this->status->IsError()) bad = true;;

        cv->Signal();
        cv->UnLock();
    }
};

#endif	/* ASYNCSTAT_HH */

