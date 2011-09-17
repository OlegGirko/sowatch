#include <QtCore/QDebug>
#include <QtDeclarative/QtDeclarative>
#include "watchserver.h"
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

	if (!_registered) {
		qmlRegisterUncreatableType<DeclarativeWatchWrapper>("com.javispedro.sowatch", 1, 0,
			"Watch", "Watch is only available via the 'watch' object");
		_registered = true;
	}

	_engine = new QDeclarativeEngine(this);
	_wrapper = new DeclarativeWatchWrapper(server->watch(), this);

	_engine->rootContext()->setContextProperty("watch", _wrapper);
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

void DeclarativeWatchlet::activate()
{
	Watchlet::activate();
	_wrapper->activate();
	_scene->update();
}

void DeclarativeWatchlet::deactivate()
{
	Watchlet::deactivate();
	_wrapper->deactivate();
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
			qWarning() << "QML has instantation errors:";
			qWarning() <<  _component->errors();
			return;
		}
		Q_ASSERT(_item == 0);
		_item = qobject_cast<QDeclarativeItem*>(obj);
		scene()->addItem(_item);
		break;
	case QDeclarativeComponent::Error:
		qWarning() << "QML has errors:";
		qWarning() <<  _component->errors();
		break;
	}
}
