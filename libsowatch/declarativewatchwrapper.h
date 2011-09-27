#ifndef SOWATCH_DECLARATIVEWATCHWRAPPER_H
#define SOWATCH_DECLARATIVEWATCHWRAPPER_H

#include <QtDeclarative/QtDeclarative>

namespace sowatch
{

class Watch;
class DeclarativeWatchlet;

class DeclarativeWatchWrapper : public QObject
{
    Q_OBJECT
	Q_PROPERTY(QString model READ model CONSTANT)
	Q_PROPERTY(bool active READ active NOTIFY activeChanged)

public:
	explicit DeclarativeWatchWrapper(Watch *watch, QObject *parent = 0);

	Q_INVOKABLE QString model() const;
	Q_INVOKABLE bool active() const;

public slots:
	void vibrate(int msecs);

signals:
	void buttonPressed(int button);
	void buttonReleased(int button);

	void activeChanged();

protected:
	Watch* _watch;
	bool _active;

	void activate();
	void deactivate();

friend class DeclarativeWatchlet;
};

}

QML_DECLARE_TYPE(sowatch::DeclarativeWatchWrapper)

#endif // SOWATCH_DECLARATIVEWATCHWRAPPER_H
