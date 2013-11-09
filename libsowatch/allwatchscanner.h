#ifndef SOWATCH_ALLWATCHSCANNER_H
#define SOWATCH_ALLWATCHSCANNER_H

#include <QtCore/QObject>
#include <QtCore/QList>

#include "watchscanner.h"

namespace sowatch
{

/** This is just a simple wrapper class that runs all registered WatchScanners */
class AllWatchScanner : public WatchScanner
{
	Q_OBJECT
public:
	explicit AllWatchScanner(QObject *parent = 0);
	void start();

private:
	QList<WatchScanner*> _scanners;
	int _finishedCount;

private slots:
	void handleFinished();
};

}

#endif // SOWATCH_ALLWATCHSCANNER_H
