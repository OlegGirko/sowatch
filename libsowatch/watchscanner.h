#ifndef SOWATCH_WATCHSCANNER_H
#define SOWATCH_WATCHSCANNER_H

#include <QtCore/QObject>
#include <QtCore/QVariant>

namespace sowatch
{


class WatchScanner : public QObject
{
	Q_OBJECT

protected:
	explicit WatchScanner(QObject *parent = 0);

public:
	virtual ~WatchScanner();

signals:
	void started();
	void finished();
	void watchFound(const QVariantMap& info);

public slots:
	virtual void start() = 0;
};

}

#endif // SOWATCH_WATCHSCANNER_H
