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
import QtQuick.Controls 2.3
import QtQuick.Controls.Universal 2.0

import Qt.labs.settings 1.0

ApplicationWindow {
    id: app

    //
    // Window options
    //
    visible: true
    minimumWidth: 780
    minimumHeight: 620
    title: CAppName + " v" + CAppVersion

    //
    // Theme options
    //
    Universal.theme: Universal.Dark
    Universal.accent: Qt.rgba(56/255, 173/255, 107/255, 1)
    Universal.background: Qt.rgba(35/255, 35/255, 43/255, 1)

    //
    // Constants
    //
    readonly property string monoFont: {
        if (Qt.platform.os == "osx")
            return "Menlo"

        else if (Qt.platform.os == "windows")
            return "Consolas"

        else
            return "Monospace"
    }

    //
    // Settings module
    //
    Settings {
        property alias _x: app.x
        property alias _y: app.y
        property alias _w: app.width
        property alias _h: app.height
        property alias _p: kP.realValue
        property alias _i: kI.realValue
        property alias _d: kD.realValue
    }

    //
    // PID controls
    //
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
                Layout.fillWidth: true
                onValueChanged: CManager.setP(realValue)
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
                Layout.fillWidth: true
                onValueChanged: CManager.setI(realValue)
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
                Layout.fillWidth: true
                onValueChanged: CManager.setD(realValue)
            }

            Item {
                Layout.fillWidth: true
            }
        }
    }

    //
    // UI
    //
    RowLayout {
        spacing: 16
        anchors.margins: 8
        anchors.fill: parent
        anchors.centerIn: parent

        Graph {
            modo: controls.rango
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Controls {
            id: controls
            Layout.rightMargin: 16
            Layout.fillHeight: true
            Layout.fillWidth: false
        }
    }

    //
    // Connection status dialog
    //
    Dialog {
        id: dialog

        //
        // Only close the dialog when the user
        // presses on the 'OK' button
        //
        modal: true
        standardButtons: Dialog.Ok
        closePolicy: Dialog.NoAutoClose

        //
        // Center dialog on app window
        //
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        //
        // Set min. width to 320 pixels
        //
        width: Math.max(implicitWidth, 320)

        //
        // Used to select <None> after the user closes
        // this dialog (only in the case that we could not
        // connect to the serial device)
        //
        property bool error: false
        onAccepted: {
            if (error)
                controls.currentDevice = 0
        }

        //
        // Message description
        //
        Label {
            id: description
            anchors.fill: parent
            verticalAlignment: Qt.AlignTop
            horizontalAlignment: Qt.AlignLeft
        }

        //
        // Show dialog when serial device connection status changes
        //
        Connections {
            target: CSerial

            onConnectionError: {
                dialog.error = true
                dialog.title = qsTr("Advertencia")
                description.text = qsTr("Desconectado de \"%1\"").arg(deviceName)
                dialog.open()
            }
        }
    }
}
