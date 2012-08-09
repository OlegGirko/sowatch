import QtQuick 1.1
import com.nokia.meego 1.1
import com.javispedro.sowatch 1.0

Column {
	property string configKey;

	Item {
		id: hourModeItem
		width: parent.width
		height: UiConstants.ListItemHeightDefault

		GConfKey {
			id: hourModeKey
			key: configKey + "/24h-mode"
			value: hourModeSwitch.checked
		}
		Label {
			text: qsTr("24-hour clock mode")
			font: UiConstants.TitleFont
			anchors.verticalCenter: parent.verticalCenter
			anchors.left: parent.left
		}
		Switch {
			id: hourModeSwitch
			anchors.verticalCenter: parent.verticalCenter
			anchors.right: parent.right
			checked: hourModeKey.value
		}
	}

	Item {
		id: dayMonthOrderItem
		width: parent.width
		height: UiConstants.ListItemHeightDefault

		GConfKey {
			id: dayMonthOrderKey
			key: configKey + "/day-month-order"
			value: dayMonthOrderSwitch.checked
		}
		Label {
			text: qsTr("Use DD/MM instead of MM/DD")
			font: UiConstants.TitleFont
			anchors.verticalCenter: parent.verticalCenter
			anchors.left: parent.left
		}
		Switch {
			id: dayMonthOrderSwitch
			anchors.verticalCenter: parent.verticalCenter
			anchors.right: parent.right
			checked: dayMonthOrderKey.value
		}
	}
}
