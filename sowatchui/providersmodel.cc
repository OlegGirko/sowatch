#include <QtDebug>

#include "providersmodel.h"

using namespace sowatch;

ProvidersModel::ProvidersModel(QObject *parent) :
    QAbstractListModel(parent),
    _config(0)
{
	QHash<int, QByteArray> roles = roleNames();
	roles[Qt::DisplayRole] = QByteArray("title");
	roles[NameRole] = QByteArray("name");
	roles[EnabledRole] = QByteArray("enabled");
	setRoleNames(roles);
}

QString ProvidersModel::configKey() const
{
	if (_config) {
		return _config->key();
	} else {
		return QString();
	}
}

void ProvidersModel::setConfigKey(const QString &configKey)
{
	QString oldConfigKey = this->configKey();
	if (_config) {
		delete _config;
		_config = 0;
	}
	if (!configKey.isEmpty()) {
		_config = new GConfKey(configKey, this);
		connect(_config, SIGNAL(changed()), SLOT(reload()));
	}
	if (this->configKey() != oldConfigKey) {
		reload();
		emit configKeyChanged();
	}
}

int ProvidersModel::rowCount(const QModelIndex &parent) const
{
	return _all_list.count();
}

QVariant ProvidersModel::data(const QModelIndex &index, int role) const
{
	switch (role) {
	case Qt::DisplayRole:
		return QVariant::fromValue(_info_list[index.row()].name);
	case NameRole:
		return QVariant::fromValue(_all_list[index.row()]);
	case EnabledRole:
		return QVariant::fromValue(_enabled.contains(_all_list[index.row()]));
	}
	return QVariant();
}

bool ProvidersModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	switch (role) {
	case EnabledRole:
		setProviderEnabled(_all_list[index.row()], value.toBool());
		return true;
	}
	return false;
}

void ProvidersModel::setProviderEnabled(const QString &id, bool enabled)
{
	if (enabled) {
		_enabled.insert(id);
	} else {
		_enabled.remove(id);
	}
	_config->set(QVariant::fromValue(QStringList(_enabled.toList())));
}

void ProvidersModel::reload()
{
	Registry *registry = Registry::registry();
	beginResetModel();
	_all_list.clear();
	_info_list.clear();
	_enabled.clear();

	_all_list = registry->allNotificationProviders();
	_info_list.reserve(_all_list.size());
	foreach (const QString& s, _all_list) {
		NotificationPluginInterface *plugin = registry->getNotificationPlugin(s);
		if (plugin) {
			_info_list.append(plugin->describeProvider(s));
		} else {
			NotificationPluginInterface::NotificationProviderInfo info;
			info.name = s;
			_info_list.append(info);
		}
	}

	_enabled = _config->value().toStringList().toSet();
	endResetModel();
}
