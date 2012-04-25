#ifndef ALLSCANNER_H
#define ALLSCANNER_H

#include <QtCore/QObject>
#include <QtCore/QList>

#include <sowatch.h>

namespace sowatch
{

class AllScanner : public WatchScanner
{
	Q_OBJECT
public:
	explicit AllScanner(QObject *parent = 0);
	void start();

private:
	QList<WatchScanner*> _scanners;
	int _finishedCount;

private slots:
	void handleFinished();
};

}

#endif // ALLSCANNER_H
