#ifndef CONFIGUREDWATCHLETSMODEL_H
#define CONFIGUREDWATCHLETSMODEL_H

#include <QAbstractListModel>

#include <sowatch.h>

class ConfiguredWatchletsModel : public QAbstractListModel
{
	Q_OBJECT
	Q_PROPERTY(QString configKey READ configKey WRITE setConfigKey NOTIFY configKeyChanged)
	Q_PROPERTY(bool displayUnadded READ displayUnadded WRITE setDisplayUnadded NOTIFY displayUnaddedChanged)

public:
	explicit ConfiguredWatchletsModel(QObject *parent = 0);

	enum DataRoles {
		NameRole = Qt::UserRole,
		ConfigQmlUrlRole
	};

	QString configKey() const;
	void setConfigKey(const QString& configKey);

	bool displayUnadded() const;
	void setDisplayUnadded(bool displayUnadded);

	int rowCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;

    QHash<int, QByteArray> getRoleNames() { return _roles; }

public slots:
	void addWatchlet(const QString& name);
	void removeWatchlet(const QString& name);
	void moveWatchletUp(const QString& name);
	void moveWatchletDown(const QString& name);

signals:
	void configKeyChanged();
	void displayUnaddedChanged();

private slots:
	void reload();
	void handleConfigChanged();

private:
	sowatch::ConfigKey *_config;
	bool _unadded;
	QStringList _list;
	QMap<QString, sowatch::WatchletPluginInterface::WatchletInfo> _info;
	QSet<QString> _enabled;
    QHash<int, QByteArray> _roles;
};

#endif // WATCHLETSMODEL_H
