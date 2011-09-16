#ifndef TESTWATCHLET_H
#define TESTWATCHLET_H

#include <QtCore/QTimer>
#include "watchlet.h"

namespace sowatch
{

class TestWatchlet : public Watchlet
{
    Q_OBJECT
public:
	explicit TestWatchlet(WatchServer* server);

	void activate();
	void deactivate();

signals:

public slots:

protected slots:
	void interv();

private:
	QTimer *_timer;
	int _y;

};

}

#endif // TESTWATCHLET_H
