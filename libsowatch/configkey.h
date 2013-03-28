#ifndef SOWATCH_CONFIGKEY_H
#define SOWATCH_CONFIGKEY_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include "sowatch_global.h"

namespace sowatch
{

/** Interface for a configuration key / tree */
class SOWATCH_EXPORT ConfigKey : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString key READ key WRITE setKey NOTIFY keyChanged)
	Q_PROPERTY(QVariant value READ value WRITE set RESET unset NOTIFY changed USER true)
	Q_PROPERTY(QStringList dirs READ dirs)
	Q_PROPERTY(QStringList keys READ keys)

public:
	ConfigKey(QObject *parent = 0);

	virtual QString key() const = 0;
	virtual void setKey(const QString& key) = 0;

	virtual QVariant value() const = 0;
	virtual void set(const QVariant& value) = 0;
	virtual void unset() = 0;
	virtual bool isSet() const = 0;
	virtual bool isDir() const = 0;

	virtual QVariant value(const QString& subkey) const = 0;
	virtual QVariant value(const QString& subkey, const QVariant& def) const = 0;
	virtual void set(const QString& subkey, const QVariant& value) = 0;
	virtual void unset(const QString& subkey) = 0;
	virtual bool isSet(const QString& subkey) const = 0;
	virtual bool isDir(const QString& subkey) const = 0;

	virtual QStringList dirs() const = 0;
	virtual QStringList keys() const = 0;

	virtual void recursiveUnset() = 0;

	virtual ConfigKey* getSubkey(const QString& subkey, QObject *parent = 0) const = 0;

signals:
	/** Key property changed (via setKey) */
	void keyChanged();
	/** Value changed. */
	void changed();
	/** A value of a subkey changed. */
	void subkeyChanged(const QString& subkey);
};

}

#endif // SOWATCH_CONFIGKEY_H
