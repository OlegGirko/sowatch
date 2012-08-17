import QtQuick 1.0

Item {
	id: neko
	height: 32
	width: 32
	clip: true

	property int targetX;
	property int targetY;
	property alias imageSource: sprite.source
	property bool running: true

	property int maxSpeed: 6

	property int _anim
	property int _animFrames
	property int _animCurFrame

	property int _distanceX: targetX - x - (width / 2)
	property int _distanceY: targetY - y - (5 * height / 6)
	property int _distance: Math.sqrt(_distanceX * _distanceX + _distanceY * _distanceY)

	property int _calcSpeedX: _distance < maxSpeed ?
								  _distanceX :
								  ((maxSpeed * _distanceX) / _distance)
	property int _calcSpeedY: _distance < maxSpeed ?
								  _distanceY :
								  ((maxSpeed * _distanceY) / _distance)

	property real _calcAngle: Math.atan2(_calcSpeedY, _calcSpeedX)

	property int _speedX: 0
	property int _speedY: 0

	signal arrived

	function chooseWalkAnimation(speedX, speedY, angle) {
		var PiPer8 = Math.PI / 8;

		if (angle > 7 * PiPer8) {
			return 6; // left
		} else if (angle > 5 * PiPer8) {
			return 2; // dwleft
		} else if (angle > 3 * PiPer8) {
			return 0; // down
		} else if (angle > 1 * PiPer8) {
			return 3; // dwright
		} else if (angle > -1 * PiPer8) {
			return 9; // right
		} else if (angle > -3 * PiPer8) {
			return 14; // upright
		} else if (angle > -5 * PiPer8) {
			return 12; // up
		} else  {
			return 13; // upleft
		}
	}

	Image {
		id: sprite
		source: "neko.png"

		x: -(neko.width * neko._animCurFrame)
		y: -(neko.height * neko._anim)
	}

	Timer {
		id: timer
		interval: 250
		running: neko.running
		repeat: true
		onTriggered: {
			neko._animCurFrame = (_animCurFrame + 1) % _animFrames
			neko.x = neko.x + _speedX;
			neko.y = neko.y + _speedY;

			if (state == "WALKING" && _distance == 0) {
				neko.arrived();
			}
		}
	}

	Timer {
		id: idleToScratchTimer
		onTriggered: neko.state = "SCRATCHING"
	}

	Timer {
		id: idleToYawnTimer
		onTriggered: neko.state = "YAWNING"
	}

	Timer {
		id: scrachTimer
		interval: timer.interval * 2
		running: neko.running && state === "SCRATCHING"
		onTriggered: neko.state = "IDLING"
	}

	Timer {
		id: yawnTimer
		interval: timer.interval * 3
		running: neko.running && state === "YAWNING"
		onTriggered: neko.state = "IDLING"
	}

	onTargetXChanged: state = "WALKING"
	onTargetYChanged: state = "WALKING"

	onArrived: state = "IDLING"

	states: [
		State {
			name: "SLEEPING"
			PropertyChanges {
				target: neko
				_anim: 11 // sleep
				_animFrames: 2
				_speedX: 0
				_speedY: 0
			}
		},
		State {
			name: "IDLING"
			PropertyChanges {
				target: neko
				_anim: 4 // jare
				_animFrames: 1
				_animCurFrame: 0
				_speedX: 0
				_speedY: 0
			}
			PropertyChanges {
				target: idleToScratchTimer
				interval: Math.random() * 10000
				running: neko.running && state == "IDLING"
			}
			PropertyChanges {
				target: idleToYawnTimer
				interval: Math.random() * 20000
				running: neko.running && state == "IDLING"
			}
		},
		State {
			name: "SCRATCHING"
			PropertyChanges {
				target: neko
				_anim: 5 // kaki
				_animFrames: 2
				_speedX: 0
				_speedY: 0
			}
		},
		State {
			name: "YAWNING"
			PropertyChanges {
				target: neko
				_anim: 8 // mati
				_animFrames: 2
				_speedX: 0
				_speedY: 0
			}
		},
		State {
			name: "WALKING"
			PropertyChanges {
				target: neko
				_anim: chooseWalkAnimation(_calcSpeedX, _calcSpeedY, _calcAngle)
				_animFrames: 2
				_speedX: _calcSpeedX
				_speedY: _calcSpeedY
			}
		}
	]
}
