#include <QtGui/QApplication>
#include <QtDeclarative/QtDeclarative>
#include "qmlapplicationviewer.h"

#include <sowatch.h>

#include "watchesmodel.h"
#include "watchscannermodel.h"
#include "providersmodel.h"
#include "configuredwatchletsmodel.h"

static sowatch::Registry *registry;
static WatchesModel *watches;
static WatchScannerModel *watchScanner;

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());

	registry = sowatch::Registry::registry();
	watches = new WatchesModel(app.data());
	watchScanner = new WatchScannerModel(app.data());

	qDebug() << "Starting" << watches << endl;

	qmlRegisterType<sowatch::ConfigKey>();
	qmlRegisterType<sowatch::GConfKey>("com.javispedro.sowatch", 1, 0, "GConfKey");
	qmlRegisterType<ProvidersModel>("com.javispedro.sowatch", 1, 0, "ProvidersModel");
	qmlRegisterType<ConfiguredWatchletsModel>("com.javispedro.sowatch", 1, 0, "ConfiguredWatchletsModel");

	viewer->rootContext()->setContextProperty("watches", watches);
	viewer->rootContext()->setContextProperty("watchScanner", watchScanner);

    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
	viewer->setMainQmlFile(QLatin1String("qml/main.qml"));
    viewer->showExpanded();

    return app->exec();
}
