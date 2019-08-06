#ifndef SETTING_H
#define SETTING_H

#include <QSettings>
#include <QJsonDocument>

class SettingsKey
{
public:
    using Key = enum {
        KEY_UPDATE_HOT = 0,
        KEY_UPDATE_TIMESTAMP,
        KEY_AUTO_ADD_WORD,
        KEY_UNNOTE_WORD_FILE,
        KEY_SENTENCE_FILE,
        KEY_SOUND_DIR,
        KEY_SOUND_VOLUME,
        KEY_SENTENCE_PATTERN,
        KEY_SENTENCE_TENSE,

        KEY_WORD_MEANS,
        KEY_WORD_TAGS
    };
};

class SettingsGroup
{
public:
    using Group = enum {
        E_GROUP_SETTINGS = 0,
        E_GROUP_SEARCH_FILES,
        GROUP_EXPORT
    };
};



class Settings
{
public:


public:
    static Settings &Instance();
    ~Settings();

    QVariant Value(const QString &key);
    QVariant operator[](const QString &key);
    QStringList GetGroupAllValue(const QString &group);
    void RemoveGroupAllValue(const QString &group);
    inline void Sync() { m_settings->sync(); }
    QString FindKey(const QString &key);

private:
    Settings();


private:
    static Settings *m_instance;
    static const QString m_default;
    QSettings *m_settings;
    QJsonDocument m_json;
};

#define SETS  Settings::Instance()

#endif // SETTING_H
