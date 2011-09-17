#include "testdeclarativewatchlet.h"

using namespace sowatch;

TestDeclarativeWatchlet::TestDeclarativeWatchlet(WatchServer* server) :
	DeclarativeWatchlet(server, "com.javispedro.sowatch.testdeclarativewatchlet")
{
	setSource(QUrl("qrc:/testdeclarativewatchlet.qml"));
}
