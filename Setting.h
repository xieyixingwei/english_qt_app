#ifndef SETTING_H
#define SETTING_H

#include <QSettings>
#include <QMap>

class Setting
{
public:
    static Setting &Instance();
    ~Setting();

    inline QVariant Value(const QString &key) { return m_sets[key]; }
    inline void SetValue(const QString &key, const QVariant &value) { m_sets[key] = value; }
    QVariant operator[](const QString &key) { return m_sets[key]; }
    void RemoveValue(const QString &key);
    QStringList GetGroupAllValue(const QString &group);
    void RemoveGroupAllValue(const QString &group);
    void Sync();

private:
    Setting();

private:
    static Setting *m_instance;
    QSettings *m_settings;
    QMap<QString, QVariant> m_sets;
};

#define SETS  Setting::Instance()

#endif // SETTING_H
