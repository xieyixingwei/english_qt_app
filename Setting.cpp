#include "Setting.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

#define JSON_OBJ(s)    "{" s "}"
#define GV_END(k, v)   "\"" k "\":{" v "}"
#define GV(k, v)       GV_END(k, v) ","

#define KV_END(k,v)      "\"" k "\":" v
#define KV(k,v)          KV_END(k,v) ","

#define STR(s)   "\"" s "\""

const QString Settings::m_default_settings(
JSON_OBJ(
    GV(GROUP_GENNERAL,
        KV_END(KEY_PART_OF_SPEECH, STR("n,v,vi,vt,adj,adv,"
                                   "prepositions,pronouns,articles,conjunctions,numerals,"
                                   "quantifiers,interjections,interrogative-words"))
        )
    GV(GROUP_SETTINGS,
        KV(KEY_AUTO_ADD_WORD, "true")
        KV(KEY_SENTENCE_FILE, STR("C:/GaoNian/Project/WordNote/WordNote/thesaurus/sentences/sentences.md"))
        KV(KEY_SOUND_DIR, STR("C:/GaoNian/Project/WordNote/WordNote/thesaurus/sounds/"))
        KV(KEY_SOUND_VOLUME, "30")
        KV(KEY_UNNOTE_WORD_FILE, STR("C:/GaoNian/Project/WordNote/WordNote/thesaurus/words/word-unnote.md"))
        KV(KEY_UPDATE_HOT, "true")
        KV_END(KEY_UPDATE_TIMESTAMP, "true")
        )
    GV(GROUP_SEARCH_FILES,
        KV("0", STR("C:/GaoNian/Project/WordNote/WordNote/thesaurus/words/*.md"))
        KV("1", STR("C:/GaoNian/Project/WordNote/WordNote/thesaurus/sentences/*.md"))
        KV_END("2", STR("C:/GaoNian/Project/WordNote/WordNote/thesaurus/essays/*.md"))
        )
    GV_END(GROUP_EXPORT,
        KV(KEY_EXPORT_WORD_FILE_ON_TAG, STR("C:/GaoNian/Project/WordNote/WordNote/thesaurus/exports/export_word_on_tag.md"))
        KV(KEY_EXPORT_SENTENCE_FILE_ON_TAG, STR("C:/GaoNian/Project/WordNote/WordNote/thesaurus/exports/export_sentence_on_tag.md"))
        KV(KEY_EXPORT_FILE_ON_HOT, STR("C:/GaoNian/Project/WordNote/WordNote/thesaurus/exports/export_on_hot.md"))
        KV(KEY_EXPORT_WORD_FILE_ON_TIMESTAMP, STR("C:/GaoNian/Project/WordNote/WordNote/thesaurus/exports/export_word_on_timestamp.md"))
        KV(KEY_EXPORT_SENTENCE_FILE_ON_TIMESTAMP, STR("C:/GaoNian/Project/WordNote/WordNote/thesaurus/exports/export_sentence_on_timestamp.md"))
        KV(KEY_EXPORT_WORD_FILE_ON_TIMESTAMP_SCOPE, STR("C:/GaoNian/Project/WordNote/WordNote/thesaurus/exports/export_word_on_timestamp_scope.md"))
        KV_END(KEY_EXPORT_SENTENCE_FILE_ON_TIMESTAMP_SCOPE, STR("C:/GaoNian/Project/WordNote/WordNote/thesaurus/exports/export_sentence_on_timestamp_scope.md"))
        )
    )
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
    QJsonParseError error;
    m_json = QJsonDocument::fromJson(m_default_settings.toUtf8(), &error);
    //qDebug() << error.error;
}

Settings::~Settings()
{
    if(nullptr != m_settings)
    {
        delete m_settings;
        m_settings = nullptr;
    }
}

QString Settings::DefaultKey(const QString &key)
{
    if(!m_json[key].isUndefined())
    {
        return key;
    }

    QStringList childKeys = m_json.object().keys();
    for(int i = 0; i < childKeys.count(); i++)
    {
        if(m_json.object()[childKeys[i]].isObject()
                && m_json.object()[childKeys[i]].toObject().contains(key))
        {
            return (childKeys[i] + "/" + key);
        }
    }

    return key;
}

QVariant Settings::DefaultValue(const QString &key)
{
    if(!m_json[key].isUndefined())
    {
        return m_json[key].toVariant();
    }

    QStringList keys = m_json.object().keys();
    for(int i = 0; i < keys.count(); i++)
    {
        if(m_json[keys[i]].isObject() && m_json[keys[i]].toObject().contains(key))
        {
            return m_json[keys[i]].toObject().value(key).toVariant();
        }
    }

    return QVariant(0);
}

QList<QVariant> Settings::DefaultGroup(const QString &group)
{
    QList<QVariant> res;

    if(m_json[group].isObject())
    {
        QStringList keys = m_json[group].toObject().keys();
        for(int i = 0; i < keys.count(); i++)
        {
            res << m_json[group].toObject().value(keys[i]).toVariant();
        }
    }

    return res;
}

QVariant Settings::Value(const QString &key)
{
    QString newKey = DefaultKey(key);
    if(m_settings->contains(newKey))
    {
        return m_settings->value(newKey);
    }

    QVariant val = DefaultValue(key);

    if(!val.isNull() && val.isValid())
    {
        m_settings->setValue(newKey, val);
        m_settings->sync();
    }

    return val;
}

QVariant Settings::operator[](const QString &key)
{
    return Value(key);
}

QList<QVariant> Settings::GetGroup(const QString &group)
{
    QList<QVariant> res;

    if(m_settings->childGroups().contains(group))
    {
        m_settings->beginGroup(group);
        QStringList keys = m_settings->childKeys();
        for(int i = 0; i < keys.count(); i++)
        {
            res << m_settings->value(keys[i]);
        }
        m_settings->endGroup();
        return res;
    }

    if(m_json[group].isObject())
    {
        m_settings->beginGroup(group);
        QStringList keys = m_json[group].toObject().keys();
        for(int i = 0; i < keys.count(); i++)
        {
             m_settings->setValue(keys[i], m_json[group].toObject().value(keys[i]).toVariant());
        }
        m_settings->endGroup();
        m_settings->sync();
    }

    return DefaultGroup(group);
}

void Settings::RemoveGroup(const QString &group)
{
    if(m_settings->childGroups().contains(group))
    {
        m_settings->beginGroup(group);
        QStringList keys = m_settings->childKeys();
        for(int i = 0; i < keys.count(); i++)
        {
            m_settings->remove(keys[i]);
        }
        m_settings->endGroup();
    }
}

void Settings::SetValue(const QString &key, const QVariant &value)
{
    m_settings->setValue(DefaultKey(key), value);
}

void Settings::Remove(const QString &key)
{
    m_settings->remove(DefaultKey(key));
}

QStringList Settings::ToStringList(const QList<QVariant> &vl)
{
    QStringList sl;
    for(int i = 0; i < vl.count(); i++)
    {
        sl << vl[i].toString();
    }

    return sl;
}
