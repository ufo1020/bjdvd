#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "userdata.h"
#include "cookiejar.h"

class DataManager
{
public:
    DataManager();

    ~DataManager();

private:
    userData* m_user_data;
};

#endif // DATAMANAGER_H
