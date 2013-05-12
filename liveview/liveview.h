#ifndef LIVEVIEW_H
#define LIVEVIEW_H

#include <sowatch.h>
#include <sowatchbt.h>

namespace sowatch
{

class LiveViewPaintEngine;

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

	void setWatchletsModel(WatchletsModel *model);

	// Only for application mode
	QImage* image();
	/** Render a image in a certain position. */
	void renderImage(int x, int y, const QImage& image);
	/** Clear the current display to black. */
	void clear();

protected:
	static const int DelayBetweenMessages = 5;

	enum MessageType {
		NoMessage = 0,
		GetDisplayProperties = 1,
		GetDisplayPropertiesResponse = 2,
		DeviceStatusChange = 7,
		DeviceStatusChangeResponse = 8,
		DisplayBitmap = 19,
		DisplayBitmapResponse = 20,
		DisplayClear = 21,
		DisplayClearResponse = 22,
		SetMenuSize = 23,
		SetMenuSizeResponse = 24,
		MenuItemRequest = 25,
		MenuItemResponse = 26,
		NotificationRequest = 27,
		NotificationResponse = 28,
		Navigation = 29,
		NavigationResponse = 30,
		MenuItemsRequest = 35,
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

	enum MenuItemType {
		MenuNotificationList = 0,
		MenuOther = 1
	};

	enum Mode {
		RootMenuMode = 0,
		ApplicationMode
	};

	enum NavigationEvent {
		SelectPress = 13,
		SelectLongPress = 14,
		SelectDoublePress = 15,
		SelectMenu = 32
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

	/** Update the device menu (after a power on, etc.) */
	void refreshMenu();

protected:
	void send(const Message& msg);
	void sendResponse(MessageType type, ResponseType response);

	void updateDisplayProperties();
	void updateSoftwareVersion();
	void displayBitmap(unsigned char x, unsigned char y, const QByteArray& image);
	void displayClear();
	void setMenuSize(unsigned char size);
	void sendMenuItem(unsigned char id, MenuItemType type, unsigned short unread, const QString& text, const QByteArray& image);
	void enableLed(const QColor& color, unsigned short delay, unsigned short time);

	void handleMessage(const Message& msg);
	void handleDeviceStatusChange(const Message& msg);
	void handleMenuItemRequest(const Message& msg);
	void handleNotificationRequest(const Message& msg);
	void handleNavigation(const Message& msg);
	void handleMenuItemsRequest(const Message& msg);
	void handleDateTimeRequest(const Message& msg);
	void handleDisplayProperties(const Message& msg);
	void handleSoftwareVersion(const Message& msg);

private slots:
	void handleDataReceived();
	void handleSendTimerTick();
	void handleWatchletsChanged();

private:
	ConfigKey *_settings;
	WatchletsModel *_watchlets;

	bool _24hMode : 1;

	int _screenWidth;
	int _screenHeight;
	QStringList _buttons;

	Mode _mode;

	// Required by QPaintDevice
	mutable LiveViewPaintEngine* _paintEngine;
	QImage _image;

	/** Message outbox queue. */
	QQueue<Message> _sendingMsgs;
	QTimer* _sendTimer;
	/** Incomplete message that is being received. */
	Message _receivingMsg;
};

}

#endif // LIVEVIEW_H