#ifndef SETTING_H
#define SETTING_H

#include <QSettings>
#include <QJsonDocument>

#define GROUP_GENNERAL   "Genernal"
    #define KEY_SENTENCE_PATTERN   "Sentence_Pattern"
    #define KEY_PART_OF_SPEECH     "Part_Of_Speech"
    #define KEY_SENTENCE_TENSE     "Sentence_Tense"
    #define KEY_WORD_TAGS          "Word_Tags"

#define GROUP_SETTINGS   "Settings"
    #define KEY_AUTO_ADD_WORD      "Auto_Add_Word"
    #define KEY_SENTENCE_FILE      "Sentence_File"
    #define KEY_SOUND_DIR          "Sound_Directory"
    #define KEY_SOUND_VOLUME       "Sound_Volume"
    #define KEY_UNNOTE_WORD_FILE   "Unnote_Word_Trace_File"
    #define KEY_UPDATE_HOT         "Update_Hot"
    #define KEY_UPDATE_TIMESTAMP   "Update_Timestamp"

#define GROUP_SEARCH_FILES   "SearchFiles"

#define GROUP_EXPORT   "Export"
    #define KEY_EXPORT_WORD_FILE_ON_TAG                   "export_word_file_on_tag"
    #define KEY_EXPORT_SENTENCE_FILE_ON_TAG               "export_sentence_file_on_tag"
    #define KEY_EXPORT_FILE_ON_HOT                        "export_file_on_hot"
    #define KEY_EXPORT_WORD_FILE_ON_TIMESTAMP             "export_word_file_on_timestamp"
    #define KEY_EXPORT_SENTENCE_FILE_ON_TIMESTAMP         "export_sentence_file_on_timestamp"
    #define KEY_EXPORT_WORD_FILE_ON_TIMESTAMP_SCOPE       "export_word_file_on_timestamp_scope"
    #define KEY_EXPORT_SENTENCE_FILE_ON_TIMESTAMP_SCOPE   "export_sentence_file_on_timestamp_scope"

class Settings
{
public:
    static Settings &Instance();
    ~Settings();

    QVariant Value(const QString &key);
    QVariant operator[](const QString &key);
    QVariant DefaultValue(const QString &key);

    void SetValue(const QString &key, const QVariant &value);
    void Remove(const QString &key);

    QList<QVariant> GetGroup(const QString &group);
    QList<QVariant> DefaultGroup(const QString &group);
    void RemoveGroup(const QString &group);
    inline void Sync() { m_settings->sync(); }
    QString DefaultKey(const QString &key);

    static QStringList ToStringList(const QList<QVariant> &vl);

private:
    Settings();

private:
    static Settings *m_instance;
    static const QString m_default_settings;
    QSettings *m_settings;
    QJsonDocument m_json;
};

#define SETS  Settings::Instance()

#endif // SETTING_H
