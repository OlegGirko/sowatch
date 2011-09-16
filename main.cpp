#include <QtGui/QApplication>
#include <QtConnectivity/QBluetoothAddress>

#include "metawatchsimulator.h"
#include "metawatch.h"
#include "watchserver.h"
#include "testwatchlet.h"
#include "testdeclarativewatchlet.h"

using namespace sowatch;
QTM_USE_NAMESPACE

static Watch *watch;
static WatchServer *server;
static Watchlet *watchlet;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	//watch = new MetaWatchSimulator();
	watch = new MetaWatch(QBluetoothAddress("D0:37:61:C3:C7:99"));
	server = new WatchServer(watch);
	//watchlet = new TestDeclarativeWatchlet(server);
	watchlet = new TestWatchlet(server);

	//server->runWatchlet("com.javispedro.sowatch.testdeclarativewatchlet");
	server->runWatchlet("com.javispedro.sowatch.testwatchlet");

    return a.exec();
}
