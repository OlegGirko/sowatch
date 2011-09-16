#include <QtCore/QDebug>
#include <QtGui/QPainter>

#include "testwatchlet.h"
#include "watch.h"

using namespace sowatch;

TestWatchlet::TestWatchlet(WatchServer* server) :
	Watchlet(server, "com.javispedro.sowatch.testwatchlet"), _timer(new QTimer(this)), _y(0)
{
	_timer->setInterval(50);
	connect(_timer, SIGNAL(timeout()), SLOT(interv()));
}

void TestWatchlet::activate()
{
	qDebug() << "test watchlet activated";
	QPainter p(watch());
	p.fillRect(0, 0, watch()->width(), watch()->height(), Qt::white);
	_timer->start();
}

void TestWatchlet::deactivate()
{
	_timer->stop();
	qDebug() << "test watchlet deactivated";
}

void TestWatchlet::interv()
{
	QPainter p(watch());
	//p.fillRect(8, _y, 8, 1, Qt::black);
	_y = (_y + 1) % watch()->height();
	p.fillRect(0, _y, _y, 2, Qt::black);
	//p.fillRect(0, 0, watch()->width(), watch()->height(), Qt::black);
}
