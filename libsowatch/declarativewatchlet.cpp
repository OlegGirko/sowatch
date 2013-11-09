#include <QtCore/QDebug>
#include <QtDeclarative/QtDeclarative>
#include "watchserver.h"
#include "watch.h"
#include "watchletsmodel.h"
#include "notificationsmodel.h"
#include "gconfkey.h"
#include "declarativewatchwrapper.h"
#include "declarativewatchlet.h"

using namespace sowatch;

bool DeclarativeWatchlet::_registered = false;

DeclarativeWatchlet::DeclarativeWatchlet(Watch* watch, const QString& id) :
	GraphicsWatchlet(watch, id),
	_engine(0),
	_component(0),
	_item(0),
	_wrapper(0)
{
	setScene(new QGraphicsScene(this));
	scene()->setItemIndexMethod(QGraphicsScene::NoIndex);
	scene()->setStickyFocus(true);

	if (!_registered) {
		qRegisterMetaType<Notification::Type>("Notification::Type");
		qRegisterMetaType<Notification::Priority>("Notification::Priority");
		qRegisterMetaType<WeatherNotification::WeatherType>("WeatherNotification::WeatherType");
		qRegisterMetaType<WeatherNotification::Unit>("WeatherNotification::Unit");
		qmlRegisterUncreatableType<DeclarativeWatchWrapper>("com.javispedro.sowatch", 1, 0,
			"Watch", "Watch is only available via the 'watch' context property");
		qmlRegisterUncreatableType<WatchletsModel>("com.javispedro.sowatch", 1, 0,
			"Notification", "WatchletsModel is only available via the 'notifications' context property");
		qmlRegisterUncreatableType<Watchlet>("com.javispedro.sowatch", 1, 0,
			"Notification", "Watchlet is an abstract class");
		qmlRegisterUncreatableType<NotificationsModel>("com.javispedro.sowatch", 1, 0,
			"NotificationsModel", "NotificationsModel is only available via the 'notifications' context property");
		qmlRegisterUncreatableType<Notification>("com.javispedro.sowatch", 1, 0,
			"Notification", "Notification is an abstract class");
		qmlRegisterUncreatableType<WeatherNotification>("com.javispedro.sowatch", 1, 0,
			"WeatherNotification", "WeatherNotification is an abstract class");
		qmlRegisterType<ConfigKey>();
		qmlRegisterType<GConfKey>("com.javispedro.sowatch", 1, 0, "GConfKey");
		_registered = true;
	}

	// A dynamic property on the Watch object is used to share a single
	// DeclarativeEngine amongst all DeclarativeWatchlet instances.
	QVariant watchEngine = watch->property("declarativeEngine");
	if (!watchEngine.isValid()) {
		// Create the shared engine
		qDebug() << "Starting QDeclarativeEngine";
		_engine = new QDeclarativeEngine(watch);
		_engine->addImportPath(SOWATCH_QML_DIR);

		// Set context properties that are shared by all watchlets here
		_engine->rootContext()->setContextProperty("watchlets", 0);
		_engine->rootContext()->setContextProperty("notifications", 0);

		watch->setProperty("declarativeEngine", QVariant::fromValue(_engine));
	} else {
		_engine = watchEngine.value<QDeclarativeEngine*>();
	}

	_context = new QDeclarativeContext(_engine, this);

	_wrapper = new DeclarativeWatchWrapper(watch, this);
	_context->setContextProperty("watch", _wrapper);
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

QDeclarativeContext* DeclarativeWatchlet::context()
{
	return _context;
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
	} else {
		qWarning() << "Declarative watchlet will not render: missing root object";
	}
	GraphicsWatchlet::activate();
	_wrapper->activate();
}

void DeclarativeWatchlet::deactivate()
{
	_wrapper->deactivate();
	GraphicsWatchlet::deactivate();
}

void DeclarativeWatchlet::setWatchletsModel(WatchletsModel *model)
{
	_context->setContextProperty("watchlets", model);
}

void DeclarativeWatchlet::setNotificationsModel(NotificationsModel *model)
{
	_context->setContextProperty("notifications", model);
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

bool DeclarativeWatchlet::handlesNotification(Notification *notification) const
{
	if (_item) {
		QVariant arg = QVariant::fromValue(notification);
		QVariant result;
		if (QMetaObject::invokeMethod(_item, "handlesNotification",
									   Q_RETURN_ARG(QVariant, result),
									   Q_ARG(QVariant, arg))) {
			return result.toBool();
		}
	}

	return false;
}

void DeclarativeWatchlet::openNotification(Notification *notification)
{
	if (_item) {
		QVariant arg = QVariant::fromValue(notification);
		QVariant result;
		if (!QMetaObject::invokeMethod(_item, "openNotification",
		                               Q_RETURN_ARG(QVariant, result),
		                               Q_ARG(QVariant, arg))) {
			qWarning() << "No openNotification method in QML root object";
		}
	}
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
		obj = _component->create(_context);
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
