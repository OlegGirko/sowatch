#ifndef TESTDECLARATIVEWATCHLET_H
#define TESTDECLARATIVEWATCHLET_H

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

#endif // TESTDECLARATIVEWATCHLET_H
