#include "Setting.h"
#include <QRegExp>

Setting *Setting::m_instance = nullptr;

Setting &Setting::Instance()
{
    if(nullptr == m_instance)
    {
        m_instance = new Setting;
    }

    return *m_instance;
}

Setting::Setting()
{
    m_settings = new QSettings("user.ini", QSettings::IniFormat);
    QStringList keys = m_settings->allKeys();

    for(int i = 0; i< keys.count(); i++)
    {
        m_sets[keys.at(i)] = m_settings->value(keys.at(i));
    }
}

Setting::~Setting()
{
    if(nullptr != m_settings)
    {
        delete m_settings;
        m_settings = nullptr;
    }
}

void Setting::RemoveValue(const QString &key)
{
    m_settings->remove(key);
    m_sets.remove(key);
}

QStringList Setting::GetGroupAllValue(const QString &group)
{
    QStringList res;

    for(QMap<QString, QVariant>::Iterator it = m_sets.begin(); it != m_sets.end(); it++)
    {
        if(it.key().contains(QRegExp(group + "/.*")))
        {
            res << it.value().toString();
        }
    }

    return res;
}

void Setting::RemoveGroupAllValue(const QString &group)
{
    for(QMap<QString, QVariant>::Iterator it = m_sets.begin(); it != m_sets.end(); it++)
    {
        if(it.key().contains(QRegExp(group + "/.*")))
        {
            RemoveValue(it.key());
        }
    }
}

void Setting::Sync()
{
    for(QMap<QString, QVariant>::Iterator it = m_sets.begin(); it != m_sets.end(); it++)
    {
        m_settings->setValue(it.key(), it.value());
    }

    m_settings->sync();
}
