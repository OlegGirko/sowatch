#include "allscanner.h"

using namespace sowatch;

AllScanner::AllScanner(QObject *parent) :
	WatchScanner(parent), _finishedCount(0)
{
	QList<WatchPluginInterface*> plugins = Registry::registry()->getWatchPlugins();
	foreach (WatchPluginInterface* driver, plugins) {
		WatchScanner* scanner = driver->getScanner(this);
		if (scanner) {
			_scanners += scanner;
			connect(scanner, SIGNAL(finished()), this, SLOT(handleFinished()));
			connect(scanner, SIGNAL(watchFound(QVariantMap)),
			        this, SIGNAL(watchFound(QVariantMap)));
		}
	}
}

void AllScanner::start()
{
	if (_scanners.empty()) {
		emit finished();
	} else {
		foreach (WatchScanner* scanner, _scanners) {
			scanner->start();
		}
		emit started();
	}
}

void AllScanner::handleFinished()
{
	qDebug() << "one finished";
	_finishedCount++;
	if (_finishedCount >= _scanners.length()) {
		qDebug() << "all finished";
		emit finished();
	}
}
