#include "nekowatchlet.h"

using namespace sowatch;

const QLatin1String NekoWatchlet::myId("com.javispedro.sowatch.neko");

NekoWatchlet::NekoWatchlet(Watch* watch) :
	DeclarativeWatchlet(watch, myId)
{
	// Workaround Qt's stupid clip region calculation when
	//  - There's a QML item with clip = true
	//  - And we are using "compat" updateRects() signal mode
	setFullUpdateMode(true);

	setSource(QUrl(SOWATCH_QML_DIR "/nekowatchlet/" + watch->model() + ".qml"));
}

