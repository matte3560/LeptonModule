import QtQuick 2.7
import QtQuick.Controls 2.1

Rectangle {
	id: root
	width: 400
	height: 400

	signal updateParameters(double fov, double scale, double pitch, double yaw, vector3d position)

	property var absAngleLimit: 45
	property var absOffsetLimit: 0.75

	property var defaultScale: 1
	property var defaultXOff: 0
	property var defaultYOff: 0
	property var defaultPFoV: 45
	property var defaultTFoV: 35
	property var defaultPitch: 0
	property var defaultYaw: 0
	property var defaultPDist: 3
	property var defaultTDist: 2

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
				id: pfov
				text: "P. FoV"
				from: 20
				to: 90
				value: defaultPFoV
			}
			LabeledSlider {
				id: tfov
				text: "T. FoV"
				from: 20
				to: 90
				value: defaultTFoV
			}
			LabeledSlider {
				id: scale
				text: "Scale"
				from: 0.2
				to: 2
				decimals: 2
				value: defaultScale
			}
			LabeledSlider {
				id: pitch
				text: "Pitch"
				from: -root.absAngleLimit
				to: root.absAngleLimit
				value: defaultPitch
			}
			LabeledSlider {
				id: yaw
				text: "Yaw"
				from: -root.absAngleLimit
				to: root.absAngleLimit
				value: defaultYaw
			}
			LabeledSlider {
				id: pos_x
				text: "Offset x"
				from: -root.absOffsetLimit
				to: root.absOffsetLimit
				decimals: 2
				value: defaultXOff
			}
			LabeledSlider {
				id: pos_y
				text: "Offset y"
				from: -root.absOffsetLimit
				to: root.absOffsetLimit
				decimals: 2
				value: defaultYOff
			}
			LabeledSlider {
				id: p_dist
				text: "P. Dist"
				from: 1
				to: 8
				decimals: 2
				value: defaultPDist
			}
			LabeledSlider {
				id: t_dist
				text: "T. Dist"
				from: 1
				to: 8
				decimals: 2
				value: defaultTDist
			}

			Row {
				anchors.horizontalCenter: parent.horizontalCenter
				spacing: 2
				Button {
					text: "Update"
					onClicked: {
						root.updateParameters(
							pfov.value, scale.value*2*Math.tan(tfov.value * Math.PI/180)*t_dist.value,
							pitch.value, yaw.value, Qt.vector3d(pos_x.value, pos_y.value, p_dist.value))
					}
				}
				Button {
					text: "Reset"
					onClicked: {
						pfov.value = defaultPFoV
						tfov.value = defaultTFoV
						scale.value = defaultScale
						pitch.value = defaultPitch
						yaw.value = defaultYaw
						pos_x.value = defaultXOff
						pos_y.value = defaultYOff
						p_dist.value = defaultPDist
						t_dist.value = defaultTDist
						root.updateParameters(
							pfov.value, scale.value*2*Math.tan(tfov.value * Math.PI/180)*t_dist.value,
							pitch.value, yaw.value, Qt.vector3d(pos_x.value, pos_y.value, p_dist.value))
					}
				}
			}
		}
	}
}
