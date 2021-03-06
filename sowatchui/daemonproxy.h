/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -c DaemonProxy -p daemonproxy ../sowatchd/daemon.xml
 *
 * qdbusxml2cpp is Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef DAEMONPROXY_H_1344472584
#define DAEMONPROXY_H_1344472584

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface com.javispedro.sowatch.Daemon
 */
class DaemonProxy: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "com.javispedro.sowatch.Daemon"; }

public:
    DaemonProxy(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~DaemonProxy();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<QString> GetWatchStatus(const QString &watch)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(watch);
        return asyncCallWithArgumentList(QLatin1String("GetWatchStatus"), argumentList);
    }

    inline QDBusPendingReply<> Terminate()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Terminate"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void WatchStatusChanged(const QString &watch, const QString &status);
};

namespace com {
  namespace javispedro {
    namespace sowatch {
      typedef ::DaemonProxy Daemon;
    }
  }
}
#endif
