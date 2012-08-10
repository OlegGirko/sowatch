import Qt 4.7
import com.javispedro.sowatch.metawatch 1.0

Rectangle {
	width: 96
	height: 96

	color: "white"

	Column {
		anchors.fill: parent
		spacing: 4

		MWTitle {
			id: title
			text: qsTr("Phone Info")
			icon.source: "icon.png"
		}

		MWSmallLabel {
			text: "Battery: " + batteryLevel + "%"
		}

		Rectangle {
			id: battery
			x: 12
			width: 72
			height: 16

			border.color: "black"
			border.width: 1

			Rectangle {
				width: (batteryLevel / 100) * parent.width
				height: parent.height

				color: "black"
			}
		}

		MWSmallLabel {
			width: parent.width
			text: "Connected to:"
		}

		MWLabel {
			width: parent.width
			text: networkName
			horizontalAlignment: Text.AlignHCenter
			wrapMode: Text.Wrap
		}
	}
}
