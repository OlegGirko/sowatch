import QtQuick 1.0
import com.javispedro.sowatch.qmap 1.0

Rectangle {
	color: "black"

	CompassView {
		anchors.centerIn: parent
		id: compass
		updateEnabled: watch.active
		updateInterval: 3000
		width: 36
		height: 36
	}
}
