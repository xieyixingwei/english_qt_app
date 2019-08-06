#include "Setting.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

const QString Settings::m_default(
        "{\
                    \"Genneral\":{\
                           \"Sentence_Pattern\": \"S + Vi,S + Vt + O,S + Vt + Io + Do,S + V + P\",\
                           \"Part_Of_Speech\": \"n,adj,v,vi,vt,adv,prep\",\
                           \"Sentence_Tense\": \"was/ware + Ving,am/is/are + Ving,will be + Ving,would be + Ving,had + Ved,have/has + Ved,will have + Ved\",\
                           \"Word_Tags\": [\"highfreq,time\"]},\
                    \"Settings\":{\
                           \"Auto_Add_Word\": 1,\
                            \"Sentence_File\": \"C:/GaoNian/Project/WordNote/WordNote/test/sentence.md\",\
                            \"Sound_Directory\":\"D:/GaoNian/English/thesaurus/sounds/\",\
                            \"Sound_Volume\": 30,\
                            \"Unnote_Word_Trace_File\": \"C:/GaoNian/Project/WordNote/WordNote/test/word-unnote.md\",\
                            \"Update_Hot\": 1,\
                            \"Update_Timestamp\": 1},\
                    \"SearchFiles\":{\
                           \"0\": \"C:/GaoNian/Project/WordNote/WordNote/test/*.md\",\
                           \"1\": \"1\"},\
                    \"Export\":{\
                           \"export_file_on_hot\": \"C:/GaoNian/Project/WordNote/WordNote/test/export/export_on_hot.md\",\
                           \"export_file_on_timestamp\": \"C:/GaoNian/Project/WordNote/WordNote/test/export/export_on_timestamp.md\",\
                           \"export_file_on_timestamp_scope\": \"C:/GaoNian/Project/WordNote/WordNote/test/export/export_on_timestamp_scope.md\"}\
                    }"
);

Settings *Settings::m_instance = nullptr;

Settings &Settings::Instance()
{
    if(nullptr == m_instance)
    {
        m_instance = new Settings;
    }

    return *m_instance;
}

Settings::Settings()
{
    m_settings = new QSettings("user.ini", QSettings::IniFormat);
    m_json = QJsonDocument::fromJson(m_default.toUtf8());
}

Settings::~Settings()
{
    if(nullptr != m_settings)
    {
        delete m_settings;
        m_settings = nullptr;
    }
}

QString Settings::FindKey(const QString &key)
{
    if(!m_json[key].isUndefined())
    {
        return key;
    }

    QString findKey;

    for(QJsonObject::const_iterator it = m_json.object().begin();
        it != m_json.object().end(); it++)
    {
        if(it->isObject() && it->toObject().contains(key))
        {
            findKey = it.key() + "/" + key;
            break;
        }
    }

    return findKey;
}

QVariant Settings::Value(const QString &key)
{
    return m_settings->value(FindKey(key));
}

QVariant Settings::operator[](const QString &key)
{

}

QStringList Settings::GetGroupAllValue(const QString &group)
{
    QStringList res;
/*
    for(QSettings::Iterator it = m_settings.begin(); it != m_settings.end(); it++)
    {
        if(it.key().contains(QRegExp(group + "/.*")))
        {
            res << it.value().toString();
        }
    }
*/
    return res;
}

void Settings::RemoveGroupAllValue(const QString &group)
{
    /*
    for(QMap<QString, QVariant>::Iterator it = m_sets.begin(); it != m_sets.end(); it++)
    {
        if(it.key().contains(QRegExp(group + "/.*")))
        {
            RemoveValue(it.key());
        }
    }*/
}
