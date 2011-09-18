#ifndef SOWATCH_TESTDECLARATIVEWATCHLET_H
#define SOWATCH_TESTDECLARATIVEWATCHLET_H

#include "declarativewatchlet.h"

namespace sowatch
{

class TestDeclarativeWatchlet : public DeclarativeWatchlet
{
    Q_OBJECT
public:
	explicit TestDeclarativeWatchlet(WatchServer* server);

};

}

#endif // SOWATCH_TESTDECLARATIVEWATCHLET_H
