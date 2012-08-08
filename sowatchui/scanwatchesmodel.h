#ifndef SCANWATCHESMODEL_H
#define SCANWATCHESMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>
#include <sowatch.h>

#include "scannerproxy.h"

class ScanWatchesModel : public QAbstractListModel
{
	Q_OBJECT
	Q_PROPERTY(bool enabled READ enabled WRITE setEnabled)
	Q_PROPERTY(bool active READ active NOTIFY activeChanged)

public:
	explicit ScanWatchesModel(QObject *parent = 0);
	~ScanWatchesModel();

	enum DataRoles {
		ObjectRole = Qt::UserRole
	};

	bool enabled() const;
	void setEnabled(bool enabled);

	bool active() const;

	int rowCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;

signals:
	void activeChanged();

private slots:
	void handleWatchFound(const QVariantMap& info);
	void handleStarted();
	void handleFinished();
	void handleTimeout();

private:
	ScannerProxy *_scanner;
	QList<QVariantMap> _list;
	QTimer *_timer;
	bool _enabled;
	bool _active;
};

#endif // SCANWATCHESMODEL_H
