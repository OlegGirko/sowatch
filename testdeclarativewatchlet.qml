import QtQuick 1.0
import com.javispedro.sowatch 1.0

Rectangle {
	width: 96
	height: 96

	Text {
		text: watch.model
	}

	Image {
		anchors.centerIn: parent
		source: "qt.png"

		transform: Rotation {
			axis { x : 0; y : 1; z: 0 }
			origin { x: width / 4; y: 0; }
			RotationAnimation on angle {
				loops: Animation.Infinite
				running: watch.active
				duration: 10000
				from: 0
				to: 360
			}
		}
	}
}
