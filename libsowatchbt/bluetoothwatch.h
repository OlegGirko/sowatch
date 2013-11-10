#ifndef SOWATCHBT_BLUETOOTHWATCH_H
#define SOWATCHBT_BLUETOOTHWATCH_H

#include <QBluetoothAddress>
#include <QBluetoothSocket>
#include <QBluetoothLocalDevice>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtSystemInfo/QSystemAlignedTimer>
#endif
#include <sowatch.h>
#include "sowatchbt_global.h"

namespace sowatch
{

using QTM_PREPEND_NAMESPACE(QBluetoothSocket);
using QTM_PREPEND_NAMESPACE(QBluetoothAddress);
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
using QTM_PREPEND_NAMESPACE(QSystemAlignedTimer);
#endif
using QTM_PREPEND_NAMESPACE(QBluetoothLocalDevice);

class SOWATCHBT_EXPORT BluetoothWatch : public Watch
{
	Q_OBJECT

public:
	BluetoothWatch(const QBluetoothAddress& address, QObject *parent = 0);
	~BluetoothWatch();

	bool isConnected() const;

protected:
	/** Start the initial connection attempt, reset failed connection timers. */
	void scheduleConnect();
	/** Schedule an new connection attempt, consider the current one failed. */
	void scheduleRetryConnect();
	/** Cancel any pending connection attempts. */
	void unscheduleConnect();

	/** Attempt a connection to the watch right now. */
	virtual void connectToWatch();

	/** To be overriden; should configure a newly connected watch. */
	virtual void setupBluetoothWatch() = 0;
	/** To be overriden; handle a sudden watch disconnection. */
	virtual void desetupBluetoothWatch() = 0;

private slots:
	void handleConnectTimer();
	void handleLocalDevModeChanged(QBluetoothLocalDevice::HostMode state);
	void handleSocketConnected();
	void handleSocketDisconnected();
	void handleSocketError(QBluetoothSocket::SocketError error);
	void handleSocketState(QBluetoothSocket::SocketState error);

protected:
	/** Local BT device used. */
	QBluetoothLocalDevice *_localDev;
	/** BT address of the watch we are trying to connect to. */
	QBluetoothAddress _address;
	/** Socket to the watch. */
	QBluetoothSocket *_socket;
	/** Whether we have succesfully connected to the watch or not. */
	bool _connected;

private:
	// Timers to retry the connection when the watch is not found.
	static const int connectRetryTimesSize = 6;
	static const int connectRetryTimes[connectRetryTimesSize];
	short _connectRetries;
    QTimer *_connectTimer;
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    QSystemAlignedTimer *_connectAlignedTimer;
#endif
};

}

#endif // SOWATCHBT_BLUETOOTHWATCH_H
