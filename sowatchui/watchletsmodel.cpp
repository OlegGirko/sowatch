#include "watchletsmodel.h"

using namespace sowatch;

static const QString watchletsSubKey("/watchlets");

WatchletsModel::WatchletsModel(QObject *parent) :
    QAbstractListModel(parent),
    _config(0),
    _unadded(false)
{
	QHash<int, QByteArray> roles = roleNames();
	roles[Qt::DisplayRole] = QByteArray("title");
	roles[Qt::DecorationRole] = QByteArray("iconSource");
	roles[NameRole] = QByteArray("name");
	setRoleNames(roles);
}

QString WatchletsModel::configKey() const
{
	if (_config) {
		QString key = _config->key();
		return key.left(key.length() - watchletsSubKey.length());
	} else {
		return QString();
	}
}

void WatchletsModel::setConfigKey(const QString &configKey)
{
	qDebug() << "Set confgKey" << configKey;
	QString oldConfigKey = this->configKey();
	if (_config) {
		delete _config;
		_config = 0;
	}
	if (!configKey.isEmpty()) {
		_config = new GConfKey(configKey + watchletsSubKey, this);
		connect(_config, SIGNAL(changed()), SLOT(handleConfigChanged()));
	}
	if (this->configKey() != oldConfigKey) {
		reload();
		emit configKeyChanged();
	}
}

bool WatchletsModel::displayUnadded() const
{
	return _unadded;
}

void WatchletsModel::setDisplayUnadded(bool displayUnadded)
{
	qDebug() << "Set dunadded" << displayUnadded;
	_unadded = displayUnadded;
	if (_config) reload();
	emit displayUnaddedChanged();
}

int WatchletsModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return _list.count();
}

QVariant WatchletsModel::data(const QModelIndex &index, int role) const
{
	const QString id = _list[index.row()];
	switch (role) {
	case Qt::DisplayRole:
		return QVariant::fromValue(_info[id].name);
	case Qt::DecorationRole:
		return QVariant::fromValue(_info[id].icon);
	case NameRole:
		return QVariant::fromValue(id);
	}
	return QVariant();
}

void WatchletsModel::addWatchlet(const QString &name)
{
	if (!_config) return;
	QStringList enabled = _config->value().toStringList();
	if (_enabled.contains(name)) return;
	enabled << name;
	_config->set(enabled);
}

void WatchletsModel::removeWatchlet(const QString &name)
{
	if (!_config) return;
	QStringList enabled = _config->value().toStringList();
	enabled.removeAll(name);
	_config->set(enabled);
}

void WatchletsModel::moveWatchletUp(const QString &name)
{
	if (!_config) return;
	QStringList enabled = _config->value().toStringList();
	int index = enabled.indexOf(name);
	qDebug() << "move up" << enabled << index << enabled[index];
	if (index > 0 && index < enabled.size()) {
		enabled.swap(index - 1, index);
	}
	_config->set(enabled);
}

void WatchletsModel::moveWatchletDown(const QString &name)
{
	if (!_config) return;
	QStringList enabled = _config->value().toStringList();
	int index = enabled.indexOf(name);
	qDebug() << "move down" << enabled << index << enabled[index];
	if (index >= 0 && index < enabled.size() - 1) {
		enabled.swap(index, index + 1);
	}
	_config->set(enabled);
}

void WatchletsModel::reload()
{
	Registry *registry = Registry::registry();
	Q_ASSERT(_config);
	beginResetModel();
	_list.clear();
	_info.clear();
	_enabled.clear();

	qDebug() << "Reloading watchlets";

	QStringList all = registry->allWatchlets();
	foreach (const QString& s, all) {
		WatchletPluginInterface *plugin = registry->getWatchletPlugin(s);
		if (plugin) {
			_info[s] = plugin->describeWatchlet(s);
		} else {
			WatchletPluginInterface::WatchletInfo info;
			info.name = s;
			_info[s] = info;
		}
	}

	QStringList enabled = _config->value().toStringList();
	_enabled = enabled.toSet();

	if (_unadded) {
		qDebug() << "Listing unadded watchlets from" << all;
		foreach (const QString& s, all) {
			if (!_enabled.contains(s)) {
				_list.append(s);
			}
		}
		_list.sort();
	} else {
		qDebug() << "Listing added watchlets from" << enabled;
		_list = enabled;
	}
	endResetModel();
}

void WatchletsModel::handleConfigChanged()
{
	// TODO
	reload();
}
