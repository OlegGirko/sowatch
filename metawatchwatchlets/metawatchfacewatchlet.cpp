#include "metawatchfacewatchlet.h"

using namespace sowatch;

const QLatin1String MetaWatchFaceWatchlet::myId("com.javispedro.sowatch.metawatch.watchface");

MetaWatchFaceWatchlet::MetaWatchFaceWatchlet(Watch *watch) :
    DeclarativeWatchlet(watch, myId)
{
	setSource(QUrl(SOWATCH_QML_DIR "/metawatchwatchlets/" + watch->model() + "-watchface.qml"));
}
