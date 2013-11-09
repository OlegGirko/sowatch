import QtQuick 1.0
import com.javispedro.sowatch 1.0
import com.javispedro.sowatch.metawatch 1.0

MWPage {
	id: page

	Column {
		Item {
			id: systemArea
			// Firmware draws top 30 lines;
			// ensure we do not draw anything on top
			width: page.width
			height: 30
		}

		Image {
			width: page.width
			height: 2
			source: "idle-border.png"
		}

		Row {
			width: page.width
			height: 30

			Text {
				id: labelForecast
				width: 36
				anchors.verticalCenter: parent.verticalCenter
				font.family: "MetaWatch Small caps 8pt"
				font.pixelSize: 8
				wrapMode: Text.Wrap
			}

			Image {
				id: iconForecast
				anchors.verticalCenter: parent.verticalCenter
				width: 24
			}

			Text {
				id: labelTemperature
				width: 36
				anchors.verticalCenter: parent.verticalCenter
				font.family: "MetaWatch Large 16pt"
				font.pixelSize: 16
				wrapMode: Text.Wrap
			}
		}

		Image {
			width: page.width
			height: 2
			source: "idle-border.png"
		}

		Item {
			width: page.width
			height: 2
		}

		Row {
			anchors.horizontalCenter: parent.horizontalCenter
			height: 30
			spacing: 8
			Column {
				spacing: 2
				Image {
					width: 24
					height: 18
					source: "idle-call.png"
				}
				Text {
					id: labelCalls
					anchors.horizontalCenter: parent.horizontalCenter
					font.family: "MetaWatch Large caps 8pt"
					font.pixelSize: 8
					text: "0"
				}
			}
			Column {
				spacing: 2
				Image {
					width: 24
					height: 18
					source: "idle-msg.png"
				}
				Text {
					id: labelMsgs
					anchors.horizontalCenter: parent.horizontalCenter
					font.family: "MetaWatch Large caps 8pt"
					font.pixelSize: 8
					text: "0"
				}
			}
			Column {
				spacing: 2
				Image {
					width: 24
					height: 18
					source: "idle-mail.png"
				}
				Text {
					id: labelMails
					anchors.horizontalCenter: parent.horizontalCenter
					font.family: "MetaWatch Large caps 8pt"
					font.pixelSize: 8
					text: "0"
				}
			}
		}
	}

	function _getImageForWeather(type) {
		switch (type) {
		case WeatherNotification.Sunny:
			return "weather-sunny.png";
		case WeatherNotification.PartlyCloudy:
		case WeatherNotification.Cloudy:
		case WeatherNotification.Fog:
			return "weather-cloudy.png";
		case WeatherNotification.Rain:
			return "weather-rain.png";
		case WeatherNotification.Thunderstorm:
			return "weather-thunderstorm.png";
		case WeatherNotification.Snow:
			return "weather-snow.png";
		}
	}

	function updateUnreadCounts() {
		labelCalls.text = notifications.fullCountByType(Notification.MissedCallNotification);
		labelMsgs.text = notifications.fullCountByType(Notification.SmsNotification) +
				notifications.fullCountByType(Notification.MmsNotification) +
				notifications.fullCountByType(Notification.ImNotification);
		labelMails.text = notifications.fullCountByType(Notification.EmailNotification);
	}

	function updateWeather() {
		var weather = notifications.getMostRecentByType(Notification.WeatherNotification);
		if (weather) {
			var unit = weather.temperatureUnits == WeatherNotification.Celsius ? "°C" : "°F";
			labelForecast.text = weather.body
			labelTemperature.text = weather.temperature + unit
			iconForecast.source = _getImageForWeather(weather.forecast)
		} else {
			labelForecast.text = ""
			labelTemperature.text = ""
			iconForecast.source = ""
		}
	}

	function update() {
		updateUnreadCounts();
		updateWeather();
	}

	Connections {
		target: notifications
		onModelChanged: update();
	}
}
