#include "metawatchfacewatchlet.h"

using namespace sowatch;

MetaWatchFaceWatchlet::MetaWatchFaceWatchlet(Watch *watch) :
    DeclarativeWatchlet(watch, "com.javispedro.sowatch.metawatch.watchface")
{
	setSource(QUrl(SOWATCH_QML_DIR "/metawatch/" + watch->model() + "-watchface.qml"));
}
