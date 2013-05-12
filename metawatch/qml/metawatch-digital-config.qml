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
		}
		MouseArea {
			id: hourModeArea
			anchors.fill: parent
			onClicked: hourModeSelect.open();
		}
		SelectionDialog {
			id: hourModeSelect
			titleText: qsTr("Time format")
			selectedIndex: hourModeKey.value ? 1 : 0
			model: ListModel {
				ListElement { name: QT_TR_NOOP("12 hour") }
				ListElement { name: QT_TR_NOOP("24 hour") }
			}
			onSelectedIndexChanged: hourModeKey.value = selectedIndex == 1
		}
		Column {
			anchors.verticalCenter: parent.verticalCenter
			anchors.left: parent.left

			Label {
				text: qsTr("Time format")
				font: UiConstants.TitleFont
			}

			Label {
				text: qsTr(hourModeSelect.model.get(hourModeSelect.selectedIndex).name)
				font: UiConstants.FieldLabelFont
				color: UiConstants.FieldLabelColor
			}
		}
		Image {
			anchors.verticalCenter: parent.verticalCenter
			anchors.right: parent.right
			source: "image://theme/meegotouch-combobox-indicator" + (theme.inverted ? "-inverted" : "")
		}
	}

	Item {
		id: dayMonthOrderItem
		width: parent.width
		height: UiConstants.ListItemHeightDefault

		GConfKey {
			id: dayMonthOrderKey
			key: configKey + "/day-month-order"
		}
		MouseArea {
			id: dayMonthOrderArea
			anchors.fill: parent
			onClicked: dayMonthOrderSelect.open();
		}
		SelectionDialog {
			id: dayMonthOrderSelect
			titleText: qsTr("Date format")
			selectedIndex: dayMonthOrderKey.value ? 1 : 0
			model: ListModel {
				ListElement { name: QT_TR_NOOP("Month/Day") }
				ListElement { name: QT_TR_NOOP("Day/Month") }
			}
			onSelectedIndexChanged: dayMonthOrderKey.value = selectedIndex == 1
		}
		Column {
			anchors.verticalCenter: parent.verticalCenter
			anchors.left: parent.left

			Label {
				text: qsTr("Date format")
				font: UiConstants.TitleFont
			}

			Label {
				text: qsTr(dayMonthOrderSelect.model.get(dayMonthOrderSelect.selectedIndex).name)
				font: UiConstants.FieldLabelFont
				color: UiConstants.FieldLabelColor
			}
		}
		Image {
			anchors.verticalCenter: parent.verticalCenter
			anchors.right: parent.right
			source: "image://theme/meegotouch-combobox-indicator" + (theme.inverted ? "-inverted" : "")
		}
	}
}
