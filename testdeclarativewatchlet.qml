import QtQuick 1.0
import com.javispedro.sowatch 1.0

Rectangle {
	width: 96
	height: 96

	Image {
		anchors.centerIn: parent
		source: "qt.png"

		transform: Rotation {
			axis { x : 0; y : 1; z: 0 }
			origin { x: width / 4; y: 0; }
			RotationAnimation on angle {
				loops: Animation.Infinite
				running: true
				duration: 10000
				from: 0
				to: 360
			}
		}
	}

	/*color: "white"

	Rectangle {
		width: 90
		height: 90

		color: "black"

		Rectangle {
			width: 10
			height: 10

			x: 0
			y: 0

			SequentialAnimation on y {
				loops: Animation.Infinite
				NumberAnimation { from: 0; to: 60; duration: 1000 }
				NumberAnimation { from: 60; to: 0; duration: 1000 }
			}
		}

		Rectangle {
			width: 10
			height: 10

			x: 80
			y: 00

			SequentialAnimation on y {
				loops: Animation.Infinite
				NumberAnimation { from: 0; to: 60; duration: 1000 }
			}
		}

		Rectangle {
			width: 10
			height: 10
			x: 10
			y: 10

			SequentialAnimation on visible {
				loops: Animation.Infinite
				PropertyAnimation { to: false; duration: 1000 }
				PropertyAnimation { to: true; duration: 1000 }
			}
		}

		Rectangle {
			id: big1
			width: 20
			height: 20
			x: 40
			y: 40

			color: "white"
			visible: false
		}

		Text {
			id: name
			x: 20
			text: "hello"
			color: "white"

			SequentialAnimation on y {
				loops: Animation.Infinite
				NumberAnimation { from: 0; to: 60; duration: 1000 }
				NumberAnimation { from: 60; to: 0; duration: 1000 }
			}
		}

		Connections {
			target: watch
			onButtonPressed: {
				console.log(button);
			}
		}
	}*/
}
