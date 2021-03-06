#include "watchscannermodel.h"

WatchScannerModel::WatchScannerModel(QObject *parent) :
    QAbstractListModel(parent),
    _scanner(new sowatch::AllWatchScanner(this)),
    _timer(new QTimer(this)),
    _enabled(false), _active(false)
{
    _roles[Qt::DisplayRole] = QByteArray("title");
    _roles[Qt::DecorationRole] = QByteArray("iconSource");
    _roles[Qt::StatusTipRole] = QByteArray("subtitle");
    _roles[ObjectRole] = QByteArray("object");

	_timer->setSingleShot(true);
	_timer->setInterval(3000);

	connect(_scanner, SIGNAL(watchFound(QVariantMap)), SLOT(handleWatchFound(QVariantMap)));
	connect(_scanner, SIGNAL(started()), SLOT(handleStarted()));
	connect(_scanner, SIGNAL(finished()), SLOT(handleFinished()));
	connect(_timer, SIGNAL(timeout()), SLOT(handleTimeout()));
}

WatchScannerModel::~WatchScannerModel()
{
}

bool WatchScannerModel::enabled() const
{
	return _enabled;
}

void WatchScannerModel::setEnabled(bool enabled)
{
	_timer->stop();

	_enabled = enabled;

	if (_enabled && !_active) {
		_scanner->start();
	}
}

bool WatchScannerModel::active() const
{
	return _active;
}

int WatchScannerModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return _list.count();
}

QVariant WatchScannerModel::data(const QModelIndex &index, int role) const
{
	const QVariantMap &info = _list.at(index.row());
	switch (role) {
	case Qt::DisplayRole:
		return info["name"];
	case Qt::DecorationRole:
		return QVariant(SOWATCH_RESOURCES_DIR "/sowatch64.png");
	case Qt::StatusTipRole:
		return info["address"];
	case ObjectRole:
		return QVariant::fromValue(info);
	}
	return QVariant();
}

void WatchScannerModel::handleWatchFound(const QVariantMap &info)
{
	qDebug() << "Scan found a watch:" << info << endl;
	if (!_list.contains(info)) {
		int count = _list.count();
		beginInsertRows(QModelIndex(), count, count);
		_list.append(info);
		endInsertRows();
	}
}

void WatchScannerModel::handleStarted()
{
	_active = true;
	emit activeChanged();
}

void WatchScannerModel::handleFinished()
{
	qDebug() << "Scan finished";
	_active = false;
	if (_enabled) {
		qDebug() << "Setting timer for next scan";
		_timer->start();
	}
	emit activeChanged();
}

void WatchScannerModel::handleTimeout()
{
	qDebug() << "Restarting scan now";
	_scanner->start();
}
