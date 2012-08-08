#include <QtDebug>

#include "scanwatchesmodel.h"

ScanWatchesModel::ScanWatchesModel(QObject *parent) :
    QAbstractListModel(parent),
    _scanner(new ScannerProxy("com.javispedro.sowatchd", "/com/javispedro/sowatch/allscanner", QDBusConnection::sessionBus())),
    _timer(new QTimer(this)),
    _enabled(false), _active(false)
{
	QHash<int, QByteArray> roles = roleNames();
	roles[Qt::DisplayRole] = QByteArray("title");
	roles[Qt::StatusTipRole] = QByteArray("subtitle");
	roles[ObjectRole] = QByteArray("object");
	setRoleNames(roles);

	_timer->setSingleShot(true);
	_timer->setInterval(3000);

	connect(_scanner, SIGNAL(WatchFound(QVariantMap)), SLOT(handleWatchFound(QVariantMap)));
	connect(_scanner, SIGNAL(Started()), SLOT(handleStarted()));
	connect(_scanner, SIGNAL(Finished()), SLOT(handleFinished()));
	connect(_timer, SIGNAL(timeout()), SLOT(handleTimeout()));
}

ScanWatchesModel::~ScanWatchesModel()
{
}

bool ScanWatchesModel::enabled() const
{
	return _enabled;
}

void ScanWatchesModel::setEnabled(bool enabled)
{
	_timer->stop();

	_enabled = enabled;

	if (_enabled && !_active) {
		_scanner->Start();
	}
}

bool ScanWatchesModel::active() const
{
	return _active;
}

int ScanWatchesModel::rowCount(const QModelIndex &parent) const
{
	return _list.count();
}

QVariant ScanWatchesModel::data(const QModelIndex &index, int role) const
{
	qDebug() << "Asked for data" << index.row() << index.column() << role;
	const QVariantMap &info = _list.at(index.row());
	switch (role) {
	case Qt::DisplayRole:
		return info["name"];
	case Qt::StatusTipRole:
		return info["address"];
	case ObjectRole:
		return QVariant::fromValue(info);
	}
	return QVariant();
}

void ScanWatchesModel::handleWatchFound(const QVariantMap &info)
{
	qDebug() << "Watch found" << info << endl;
	if (!_list.contains(info)) {
		int count = _list.count();
		beginInsertRows(QModelIndex(), count, count);
		_list.append(info);
		endInsertRows();
	}
}

void ScanWatchesModel::handleStarted()
{
	_active = true;
	emit activeChanged();
}

void ScanWatchesModel::handleFinished()
{
	qDebug() << "Scan finished";
	_active = false;
	if (_enabled) {
		_timer->start();
	}
	emit activeChanged();
}

void ScanWatchesModel::handleTimeout()
{
	qDebug() << "Restarting scan";
	_scanner->Start();
}
