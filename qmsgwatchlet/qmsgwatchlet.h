#ifndef QMSGWATCHLET_H
#define QMSGWATCHLET_H

#include <sowatch.h>

namespace sowatch
{

class QMsgWatchlet : public DeclarativeWatchlet
{
    Q_OBJECT
public:
	explicit QMsgWatchlet(WatchServer* server);
};

}

#endif // QMSGWATCHLET_H
