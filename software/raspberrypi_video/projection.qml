import QtQuick 2.7
import QtQuick.Controls 2.1

Rectangle {
	id: root
	width: 400
	height: 400

	signal updateParameters(double fov, double scale, double pitch, double yaw, vector2d position)

	property var absAngleLimit: 45
	property var absOffsetLimit: 0.75

	Rectangle {
		anchors.fill: parent
		anchors.margins: 10

		Label {
			id: title
			anchors { left: parent.left; right: parent.right; top: parent.top; }
			text: "Projection parameters"
			font.pixelSize: 20
		}

		Column {
			id: options
			anchors { top: title.bottom; left: parent.left; right: parent.right; bottom: parent.bottom }
			anchors.topMargin: 10

			spacing: 2

			LabeledSlider {
				id: fov
				text: "FoV"
				from: 20
				to: 90
				value: 45
			}
			LabeledSlider {
				id: scale
				text: "Scale"
				from: 0.2
				to: 2
				decimals: 2
				value: 1
			}
			LabeledSlider {
				id: pitch
				text: "Pitch"
				from: -root.absAngleLimit
				to: root.absAngleLimit
				value: 0
			}
			LabeledSlider {
				id: yaw
				text: "Yaw"
				from: -root.absAngleLimit
				to: root.absAngleLimit
				value: 0
			}
			LabeledSlider {
				id: pos_x
				text: "Offset x"
				from: -root.absOffsetLimit
				to: root.absOffsetLimit
				decimals: 2
				value: 0
			}
			LabeledSlider {
				id: pos_y
				text: "Offset y"
				from: -root.absOffsetLimit
				to: root.absOffsetLimit
				decimals: 2
				value: 0
			}

			Row {
				anchors.horizontalCenter: parent.horizontalCenter
				spacing: 2
				Button {
					text: "Update"
					onClicked: {
						root.updateParameters(fov.value, scale.value, pitch.value, yaw.value, Qt.vector2d(pos_x.value,pos_y.value))
					}
				}
				Button {
					text: "Reset"
					onClicked: {
						fov.value = 45
						scale.value = 1
						pitch.value = 0
						yaw.value = 0
						pos_x.value = 0
						pos_y.value = 0
						root.updateParameters(fov.value, scale.value, pitch.value, yaw.value, Qt.vector2d(pos_x.value,pos_y.value))
					}
				}
			}
		}
	}
}
