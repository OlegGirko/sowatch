import QtQuick 1.0
import QtMobility.location 1.2
import com.javispedro.sowatch.metawatch 1.0
import com.javispedro.sowatch.qmap 1.0

MWPage {
	MWTitle {
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		text: qsTr("Trip computer")
		icon.source: "trip-icon.png"
	}

	PositionSource {
		id: gps
		active: watch.active
		updateInterval: 2000
	}

	function formatSpeed(speed) {
		if (speed < 10) {
			return speed.toFixed(1);
		} else {
			return speed.toFixed(0);
		}
	}

	Column {
		anchors.verticalCenter: parent.verticalCenter
		MWLabel {
			text: qsTr("Speed")
		}
		MWLabel {
			id: speedLabel
			text: gps.position.speedValid ? formatSpeed(gps.position.speed) : ""
		}

		MWLabel {
			text: qsTr("Altitude")
		}
	}

	Column {

	}
}
