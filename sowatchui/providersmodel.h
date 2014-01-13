#ifndef PROVIDERSMODEL_H
#define PROVIDERSMODEL_H

#include <QtCore/QAbstractListModel>

#include <sowatch.h>

class ProvidersModel : public QAbstractListModel
{
	Q_OBJECT
	Q_PROPERTY(QString configKey READ configKey WRITE setConfigKey NOTIFY configKeyChanged)

public:
	explicit ProvidersModel(QObject *parent = 0);

	enum DataRoles {
		NameRole = Qt::UserRole,
		EnabledRole
	};

	QString configKey() const;
	void setConfigKey(const QString& configKey);

	int rowCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);

    QHash<int, QByteArray> getRoleNames() { return _roles; }
public slots:
	void setProviderEnabled(const QString& id, bool enabled);

signals:
	void configKeyChanged();
	
private slots:
	void reload();
	void handleConfigChanged();

private:
	sowatch::ConfigKey *_config;
	QStringList _all_list;
	QList<sowatch::NotificationPluginInterface::NotificationProviderInfo> _info_list;
	QSet<QString> _enabled;
    QHash<int, QByteArray> _roles;
};

#endif // PROVIDERSMODEL_H
