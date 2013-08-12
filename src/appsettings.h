#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QVariant>
#include <QHash>

/*
 * Store settings for whole application
 */
class AppSettings
{
public:
    AppSettings();

    QVariant value(const QString &key, const QVariant &defaultValue = QVariant());
    void setValue(const QString &key, const QVariant &val);

    void beginGroup(const QString &group);
    void endGroup();

    void saveSettings();

private:
    QString prependGroup;
    QHash<QString, QVariant> settings;
};

#endif // APPSETTINGS_H
