//#include <QDebug>
#include <QSettings>
#include "appsettings.h"

AppSettings::AppSettings()
{
}

QVariant AppSettings::value(const QString &key, const QVariant &defaultValue)
{
//    qDebug() << prependGroup + key;

    if(settings.contains(prependGroup + key)) {
        return settings[prependGroup + key];
    } else {
        return defaultValue;
    }
}

void AppSettings::setValue(const QString &key, const QVariant &val)
{
//    qDebug() << prependGroup + key;

    settings.insert(prependGroup + key, val);
}

void AppSettings::beginGroup(const QString &group)
{
    prependGroup = group + '/';
}

void AppSettings::endGroup()
{
    prependGroup = "";
}

void AppSettings::saveSettings()
{
    QSettings s;
    
    foreach(QString key, settings.keys()) {
        s.setValue(key, settings[key]);
    }
}

