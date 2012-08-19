import QtQuick 1.0
import QtMobility.location 1.2
import com.javispedro.sowatch.metawatch 1.0
import com.javispedro.sowatch.qmap 1.0

MWPage {

	function formatSpeed(speed) {
		var kmh = speed * 3.6;
		if (kmh < 0) {
			return "";
		} else if (kmh < 10) {
			return kmh.toFixed(1) + " km/h";
		} else {
			return kmh.toFixed(0) + " km/h";
		}
	}

	Column {
		anchors.top: parent.top
		width: parent.width

		CompassView {
			anchors.horizontalCenter: parent.horizontalCenter
			id: compass
			updateEnabled: watch.active
			updateInterval: 3000
			width: 48
			height: 48
		}

		Row {
			MWLabel {
				text: qsTr("Speed") + " "
			}
			MWLabel {
				id: speedLabel
				text: formatSpeed(compass.currentSpeed)
			}
		}
	}
}
