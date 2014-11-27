#include "datamanager.h"

DataManager::DataManager()
{
    m_user_data = new userData();
}

DataManager::~DataManager()
{
    delete m_user_data;
}
