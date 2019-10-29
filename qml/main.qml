/*
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0

import Qt.labs.settings 1.0

ApplicationWindow {
    id: app
    visible: true
    minimumWidth: 720
    minimumHeight: 520
    title: qsTr("Banco de Pruebas PVTOL")

    Material.theme: Material.Dark
    Material.primary: Material.Green
    Material.accent: Material.Green
    Universal.theme: Universal.Dark
    Universal.accent: Universal.Green
    Universal.background: Qt.rgba(40/255, 42/255, 51/255, 1)

    Settings {
        property alias _x: app.x
        property alias _y: app.y
        property alias _w: app.width
        property alias _h: app.height
        property alias _p: kP.realValue
        property alias _i: kI.realValue
        property alias _d: kD.realValue
    }

    header: ToolBar {
        height: 48

        RowLayout {
            spacing: 8
            anchors.margins: 4
            anchors.top: parent.top
            anchors.centerIn: parent
            anchors.bottom: parent.bottom

            Item {
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("P")
                color: "white"
                font.bold: true
            }

            DoubleSpinBox {
                id: kP
                realValue: 0.01
                realStepSize: 0.01
                Layout.fillWidth: true
            }

            Item {
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("I")
                color: "white"
                font.bold: true
            }

            DoubleSpinBox {
                id: kI
                realValue: 0.01
                realStepSize: 0.01
                Layout.fillWidth: true
            }

            Item {
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("D")
                color: "white"
                font.bold: true
            }

            DoubleSpinBox {
                id: kD
                realValue: 0.01
                realStepSize: 0.01
                Layout.fillWidth: true
            }

            Item {
                Layout.fillWidth: true
            }
        }
    }

    RowLayout {
        spacing: 16
        anchors.margins: 8
        anchors.fill: parent
        anchors.centerIn: parent

        Graph {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Controls {
            Layout.fillHeight: true
            Layout.rightMargin: 16
        }
    }
}
