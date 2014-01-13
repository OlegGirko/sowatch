#ifndef WATCHSCANNERMODEL_H
#define WATCHSCANNERMODEL_H

#include <QtCore/QAbstractListModel>

#include <sowatch.h>

class WatchScannerModel : public QAbstractListModel
{
	Q_OBJECT
	Q_PROPERTY(bool enabled READ enabled WRITE setEnabled)
	Q_PROPERTY(bool active READ active NOTIFY activeChanged)

public:
	explicit WatchScannerModel(QObject *parent = 0);
	~WatchScannerModel();

	enum DataRoles {
		ObjectRole = Qt::UserRole
	};

	bool enabled() const;
	void setEnabled(bool enabled);

	bool active() const;

	int rowCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;

    QHash<int, QByteArray> getRoleNames() { return _roles; }

signals:
	void activeChanged();

private slots:
	void handleWatchFound(const QVariantMap& info);
	void handleStarted();
	void handleFinished();
	void handleTimeout();

private:
	sowatch::WatchScanner *_scanner;
	QList<QVariantMap> _list;
	QTimer *_timer;
	bool _enabled;
	bool _active;
    QHash<int, QByteArray> _roles;
};

#endif // WATCHSCANNERMODEL_H
