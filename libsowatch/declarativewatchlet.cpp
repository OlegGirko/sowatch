#include <QtCore/QDebug>
#include <QtDeclarative/QtDeclarative>
#include "watchserver.h"
#include "watch.h"
#include "gconfkey.h"
#include "declarativewatchwrapper.h"
#include "declarativewatchlet.h"

using namespace sowatch;

bool DeclarativeWatchlet::_registered = false;

DeclarativeWatchlet::DeclarativeWatchlet(WatchServer* server, const QString& id) :
	GraphicsWatchlet(server, id),
	_engine(0),
	_component(0),
	_item(0),
	_wrapper(0)
{
	setScene(new QGraphicsScene(this));
	scene()->setItemIndexMethod(QGraphicsScene::NoIndex);
	scene()->setStickyFocus(true);

	if (!_registered) {
		qmlRegisterUncreatableType<DeclarativeWatchWrapper>("com.javispedro.sowatch", 1, 0,
			"Watch", "Watch is only available via the 'watch' context property");
		qmlRegisterUncreatableType<NotificationsModel>("com.javispedro.sowatch", 1, 0,
			"NotificationsModel", "NotificationsModel is only available via the 'notifications' context property");
		qmlRegisterType<ConfigKey>();
		qmlRegisterType<GConfKey>("com.javispedro.sowatch", 1, 0, "GConfKey");
		_registered = true;
	}

	_engine = new QDeclarativeEngine(this);
#if !defined(QT_NO_DEBUG)
	QString qmlDir = QDir::current().absoluteFilePath(SOWATCH_QML_DIR);
	qDebug() << "Using debug QML import path: " << qmlDir;
	_engine->addImportPath(SOWATCH_QML_DIR);
#else
	_engine->addImportPath(SOWATCH_QML_DIR);
#endif

	_wrapper = new DeclarativeWatchWrapper(server, server->watch(), this);
	_engine->rootContext()->setContextProperty("watch", _wrapper);
	_engine->rootContext()->setContextProperty("notifications",
	                                           const_cast<NotificationsModel*>(server->notifications()));
}

DeclarativeWatchlet::~DeclarativeWatchlet()
{

}

void DeclarativeWatchlet::setSource(const QUrl &url)
{
	if (_item) {
		scene()->removeItem(_item);
		delete _item;
		_item = 0;
	}
	if (_component) {
		delete _component;
		_component = 0;
	}
	if (!url.isEmpty()) {
		_component = new QDeclarativeComponent(_engine, url, this);
		connect(_component, SIGNAL(statusChanged(QDeclarativeComponent::Status)),
				SLOT(handleComponentStatus(QDeclarativeComponent::Status)));
		if (!_component->isLoading()) {
			/* No signals are going to be generated for this. */
			handleComponentStatus(_component->status());
		} else {
			connect(_component, SIGNAL(statusChanged(QDeclarativeComponent::Status)),
					this, SLOT(handleComponentStatus(QDeclarativeComponent::Status)));
		}
	}
}

QDeclarativeEngine* DeclarativeWatchlet::engine()
{
	return _engine;
}

QDeclarativeContext* DeclarativeWatchlet::rootContext()
{
	return _engine->rootContext();
}

QDeclarativeItem* DeclarativeWatchlet::rootObject()
{
	return _item;
}

void DeclarativeWatchlet::activate()
{
	// Now we certainly know the watch's area, so it is a good moment to
	// resize the root object if needed.
	if (_item) {
		Watch *watch = this->watch();
		if (!qFuzzyCompare(watch->width(), _item->width())) {
			qDebug() << "Resizing root object to width" << watch->width();
			_item->setWidth(watch->width());
		}
		if (!qFuzzyCompare(watch->height(), _item->height())) {
			qDebug() << "Resizing root object to height" << watch->width();
			_item->setHeight(watch->height());
		}
	}
	GraphicsWatchlet::activate();
	_wrapper->activate();
}

void DeclarativeWatchlet::deactivate()
{
	_wrapper->deactivate();
	GraphicsWatchlet::deactivate();
}

void DeclarativeWatchlet::setRootObject(QDeclarativeItem *item)
{
	Q_ASSERT(_item == 0); /* This function should not be called with a current object. */
	if (!item) {
		qWarning() << "QML root object is not a declarative item?";
		return;
	}

	_item = item;
	scene()->addItem(_item);
}

void DeclarativeWatchlet::handleComponentStatus(QDeclarativeComponent::Status status)
{
	QObject *obj;
	disconnect(_component, SIGNAL(statusChanged(QDeclarativeComponent::Status)),
			   this, SLOT(handleComponentStatus(QDeclarativeComponent::Status)));
	switch (status) {
	case QDeclarativeComponent::Null:
	case QDeclarativeComponent::Loading:
		/* Nothing to do */
		break;
	case QDeclarativeComponent::Ready:
		obj = _component->create();
		if (_component->isError()) {
			qWarning() << "QML has errors found while creating:";
			qWarning() <<  _component->errors();
			return;
		}
		setRootObject(qobject_cast<QDeclarativeItem*>(obj));
		break;
	case QDeclarativeComponent::Error:
		qWarning() << "QML has errors found while loading:";
		qWarning() <<  _component->errors();
		break;
	}
}
