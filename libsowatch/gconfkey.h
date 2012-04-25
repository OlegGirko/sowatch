#ifndef SOWATCH_GCONFKEY_H
#define SOWATCH_GCONFKEY_H

#include "configkey.h"

namespace sowatch
{

class SOWATCH_EXPORT GConfKey : public ConfigKey
{
	Q_OBJECT

public:
	GConfKey(const QString& key, QObject *parent = 0);
	~GConfKey();

	QString key() const;

	QVariant value() const;
	void set(const QVariant& value);
	void unset();
	bool isSet() const;
	bool isDir() const;

	QVariant value(const QString& subkey) const;
	QVariant value(const QString& subkey, const QVariant& def) const;
	void set(const QString& subkey, const QVariant& value);
	void unset(const QString& subkey);
	bool isSet(const QString& subkey) const;
	bool isDir(const QString& subkey) const;

	QStringList dirs() const;
	QStringList keys() const;

	void recursiveUnset();

	ConfigKey* getSubkey(const QString& subkey, QObject *parent = 0) const;

	void notifyChanged(const QString& key);

protected:
	void connectNotify(const char *signal);

private:
	QString _key;
	uint _notify;

	QString fullpath(const QString& subkey = QString()) const;
	QByteArray getNativeKey() const;
};

}

#endif // SOWATCH_CONFIGKEY_H
