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
        KV(KEY_PART_OF_SPEECH, STR("n,v,vi,vt,adj,adv,prepositions,pronouns,articles,conjunctions,numerals,quantifiers,interjections,interrogative-words"))
        KV(KEY_SENTENCE_TENSE, STR("was/ware + Ving,"
                                   "am/is/are + Ving,"
                                   "will be + Ving,"
                                   "would be + Ving,"
                                   "had + Ved,"
                                   "have/has + Ved,"
                                   "will have + Ved,"
                                   "would have + Ved,"
                                   "did + V,"
                                   "does/do + V,"
                                   "will + V,"
                                   "would + V,"
                                   "had + been Ving,"
                                   "have/has + been Ving,"
                                   "will have + been Ving,"
                                   "would have + been Ving"))
        KV(KEY_SENTENCE_PATTERN, STR("simple-sentences: S + Vi,"
                                     "simple-sentences: S + Vi: + adv,"
                                     "simple-sentences: S + Vi: + to do,"
                                     "simple-sentences: S + Vi: + prep-phrase,"
                                     "simple-sentences: S + Vi: + adverbial-clause,"
                                     "simple-sentences: S + Vi: There/Here + Vi + S,"
                                     "simple-sentences: S + Vi + SC,"
                                     "simple-sentences: S + Vt + O,"
                                     "simple-sentences: S + Vt + IO + DO,"
                                     "simple-sentences: S + Vt + O + O,"
                                     "simple-sentences: S + V + P,"
                                     "declarative-sentences: do,"
                                     "declarative-sentences: don't,"
                                     "interrogative-sentences: general,"
                                     "interrogative-sentences: special,"
                                     "interrogative-sentences: inverse,"
                                     "interrogative-sentences: select,"
                                     "imperative-sentences: form: do,"
                                     "imperative-sentences: form: question,"
                                     "imperative-sentences: form: special,"
                                     "imperative-sentences: first person,"
                                     "imperative-sentences: second persion,"
                                     "imperative-sentences: third persion,"
                                     "exclamatory-sentences: what,"
                                     "exclamatory-sentences: how,"
                                     "exclamatory-sentences: others,"
                                     "noun-clauses: subject-clauses,"
                                     "noun-clauses: it-form-subject,"
                                     "noun-clauses: object-clauses,"
                                     "noun-clauses: statement-clauses,"
                                     "noun-clauses: apposition-clause,"
                                     "atrributive-clauses: relative pronoun,"
                                     "atrributive-clauses: relative adverb,"
                                     "atrributive-clauses: limited and non-restrictive,"
                                     "adverbial-clauses: time,"
                                     "adverbial-clauses: location,"
                                     "adverbial-clauses: method,"
                                     "adverbial-clauses: concession,"
                                     "adverbial-clauses: comparison,"
                                     "adverbial-clauses: condition,"
                                     "adverbial-clauses: reason,"
                                     "adverbial-clauses: purpose,"
                                     "adverbial-clauses: result,"
                                     ))
        KV_END(KEY_WORD_TAGS, STR("contact-verb: change or result,"
                                  "contact-verb: status,"
                                  "vt: double-object: to,"
                                  "vt: double-object: for,"))
        )
    GV(GROUP_SETTINGS,
        KV(KEY_AUTO_ADD_WORD, "true")
        KV(KEY_SENTENCE_FILE, STR("C:/GaoNian/Project/WordNote/WordNote/test/sentences.md"))
        KV(KEY_SOUND_DIR, STR("D:/GaoNian/English/thesaurus/sounds/"))
        KV(KEY_SOUND_VOLUME, "30")
        KV(KEY_UNNOTE_WORD_FILE, STR("C:/GaoNian/Project/WordNote/WordNote/test/word-unnote.md"))
        KV(KEY_UPDATE_HOT, "true")
        KV_END(KEY_UPDATE_TIMESTAMP, "true")
        )
    GV(GROUP_SEARCH_FILES,
        KV_END("0", STR("C:/GaoNian/Project/WordNote/WordNote/test/*.md"))
        )
    GV_END(GROUP_EXPORT,
        KV(KEY_EXPORT_WORD_FILE_ON_TAG, STR("C:/GaoNian/Project/WordNote/WordNote/test/export/export_word_on_tag.md"))
        KV(KEY_EXPORT_SENTENCE_FILE_ON_TAG, STR("C:/GaoNian/Project/WordNote/WordNote/test/export/export_sentence_on_tag.md"))
        KV(KEY_EXPORT_FILE_ON_HOT, STR("C:/GaoNian/Project/WordNote/WordNote/test/export/export_on_hot.md"))
        KV(KEY_EXPORT_WORD_FILE_ON_TIMESTAMP, STR("C:/GaoNian/Project/WordNote/WordNote/test/export/export_word_on_timestamp.md"))
        KV(KEY_EXPORT_SENTENCE_FILE_ON_TIMESTAMP, STR("C:/GaoNian/Project/WordNote/WordNote/test/export/export_sentence_on_timestamp.md"))
        KV(KEY_EXPORT_WORD_FILE_ON_TIMESTAMP_SCOPE, STR("C:/GaoNian/Project/WordNote/WordNote/test/export/export_word_on_timestamp_scope.md"))
        KV_END(KEY_EXPORT_SENTENCE_FILE_ON_TIMESTAMP_SCOPE, STR("C:/GaoNian/Project/WordNote/WordNote/test/export/export_sentence_on_timestamp_scope.md"))
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
        if(m_json.object()[childKeys[i]].isObject() && m_json.object()[childKeys[i]].toObject().contains(key))
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
