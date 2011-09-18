#ifndef SOWATCH_TESTWATCHLET_H
#define SOWATCH_TESTWATCHLET_H

#include <QtCore/QTimer>
#include "watchlet.h"

namespace sowatch
{

class TestWatchlet : public Watchlet
{
    Q_OBJECT
public:
	explicit TestWatchlet(WatchServer* server);

protected slots:
	void interv();
	void handleActivated();
	void handleDeactivated();

private:
	QTimer *_timer;
	int _y;

};

}

#endif // SOWATCH_TESTWATCHLET_H
