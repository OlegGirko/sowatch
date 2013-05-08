#ifndef LIVEVIEW_H
#define LIVEVIEW_H

#include <sowatch.h>
#include <sowatchbt.h>

namespace sowatch
{

class LiveView : public BluetoothWatch
{
	Q_OBJECT

public:
	explicit LiveView(ConfigKey *settings, QObject *parent = 0);
	~LiveView();

	QPaintEngine* paintEngine() const;
	int metric(PaintDeviceMetric metric) const;

	QString model() const;
	QStringList buttons() const;

	bool busy() const;

	void setDateTime(const QDateTime& dateTime);
	void queryDateTime();
	QDateTime dateTime() const;

	void queryBatteryLevel();
	int batteryLevel() const;

	void queryCharging();
	bool charging() const;

	void displayIdleScreen();
	void displayNotification(Notification *notification);
	void displayApplication();

	void vibrate(int msecs);

protected:
	static const int DelayBetweenMessages = 5;

	enum MessageType {
		NoMessage = 0,
		GetDisplayProperties = 1,
		GetDisplayPropertiesResponse = 2,
		DeviceStatusChange = 7,
		DeviceStatusChangeResponse = 8,
		DateTimeRequest = 38,
		DateTimeResponse = 39,
		EnableLed = 40,
		EnableLedResponse = 41,
		Ack = 44,
		GetSoftwareVersion = 68,
		GetSoftwareVersionResponse = 69
	};

	enum ResponseType {
		ResponseOk = 0,
		ResponseError = 1,
		ResponseOutOfMemory = 2,
		ResponseExit = 3,
		ResponseCancel = 4
	};

	enum DeviceStatus {
		DeviceOff = 0,
		DeviceOn = 1,
		DeviceMenu = 2
	};

	struct Message {
		MessageType type;
		QByteArray data;
		Message(MessageType ntype = NoMessage, QByteArray ndata = QByteArray()) :
			type(ntype), data(ndata)
		{ }
	};

	void setupBluetoothWatch();
	void desetupBluetoothWatch();

	void send(const Message& msg);
	void sendResponse(MessageType type, ResponseType response);

	void updateDisplayProperties();
	void updateSoftwareVersion();
	void enableLed(const QColor& color, int delay, int time);

	void handleMessage(const Message& msg);
	void handleDeviceStatusChange(const Message& msg);
	void handleDateTimeRequest(const Message& msg);
	void handleDisplayProperties(const Message& msg);
	void handleSoftwareVersion(const Message& msg);

private slots:
	void handleDataReceived();
	void handleSendTimerTick();

private:
	ConfigKey *_settings;

	bool _24hMode : 1;

	/** Message outbox queue. */
	QQueue<Message> _sendingMsgs;
	QTimer* _sendTimer;
	/** Incomplete message that is being received. */
	Message _receivingMsg;
};

}

#endif // LIVEVIEW_H
