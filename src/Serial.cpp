/*
 * Copyright (c) 2018 Kaan-Sat <https://kaansat.com.mx/>
 *
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

#include <cmath>

#include <QDir>
#include <QFile>
#include <QTimer>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDesktopServices>

#include "Serial.h"

#define EOT_PRIMARY QChar('\n')
#define MAX_BUFFER_SIZE 1024 * 1024

/**
 * @brief Pointer to the only instance of this class.
 *
 * We need to use a single-instance approach so that all application modules
 * and classes can communicate and use the current serial device without dealing
 * with permission and/or race issues.
 */
static Serial* instance = Q_NULLPTR;

/**
 * @brief Constructor for the @a SerialManager class
 */
Serial::Serial() :
    m_baudRate(9600),
    m_dataLen(-1),
    m_port(Q_NULLPTR)
{
    QTimer::singleShot(500, this, &Serial::refreshSerialDevices);
}

/**
 * @brief Closes the comm. channels with the current device and closes
 *        the current log file
 */
Serial::~Serial() {
    if (m_port != Q_NULLPTR)
        m_port->close();
}

/**
 * @returns The only instance of the @c SerialManager class, we need
 *          to have only one instance for every application module to
 *          interface with the same serial device without getting into
 *          permission issues or using additional resources
 */
Serial* Serial::getInstance() {
    if (instance == Q_NULLPTR)
        instance = new Serial();

    return instance;
}

/**
 * @returns the baud rate used to communicate with the serial device
 */
int Serial::baudRate() const {
    return m_baudRate;
}

/**
 * @returns @c true if the application is connected to any serial
 *          device, otherwise, this function shall return @c false
 */
bool Serial::connected() const  {
    if (m_port != Q_NULLPTR)
        return m_port->isOpen();

    return false;
}

/**
 * @brief SerialManager::deviceName
 * @return
 */
QString Serial::deviceName() const {
    if (m_port)
        return m_port->portName();

    return "Undefined";
}

/**
 * @returns An user-friendly string that represents the number of bytes
 *          received from the current serial device.
 */
QString Serial::receivedBytes() const {
    if (m_port != Q_NULLPTR && m_dataLen >= 0)
        return sizeStr(m_dataLen);

    return "0 " + tr("bytes");
}

QStringList Serial::baudRates() const {
    return QStringList {
        "1200",
        "2400",
        "4800",
        "9600",
        "19200",
        "38400",
        "57600",
        "115200"
    };
}

/**
 * @returns A list with the port names (such as COM1, COM2, ttyACMO) of the
 *          ports that have any serial device connected to them
 */
QStringList Serial::serialDevices() const {
    return m_serialDevices;
}

/**
 * Changes the baud @a rate used to communicate with the serial device
 */
void Serial::setBaudRate(const int rate) {
    if (rate > 0) {
        m_baudRate = rate;

        if (m_port)
            m_port->setBaudRate(baudRate());

        emit baudRateChanged();
    }
}

/**
 * @brief SerialManager::startComm
 * @param device
 */
void Serial::startComm(const int device) {
    // Disconnect current serial port device
    disconnectDevice();

    // Ignore the <Select Port> virtual device
    if (device > 0) {
        // Get 'real' port ID
        int portId = device - 1;
        QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

        // Check if port ID is valid
        if (portId < ports.count()) {
            // Configure new serial port device
            m_port = new QSerialPort(ports.at(portId));
            m_port->setBaudRate(baudRate());

            // Connect signals/slots
            connect(m_port, SIGNAL(readyRead()),
                    this,     SLOT(onDataReceived()));
            connect(m_port, SIGNAL(aboutToClose()),
                    this,     SLOT(disconnectDevice()));

            // Try to open the serial port device
            if (m_port->open(QIODevice::ReadWrite)) {
                emit connectionChanged();
                emit connectionSuccess(m_port->portName());
            }

            // There was an error opening the serial port device
            else
                disconnectDevice();
        }

        // Port ID is invalid
        else
            emit connectionError("Invalid");
    }
}

/**
 * @brief SerialManager::onDataReceived
 */
void Serial::onDataReceived() {
    // Check serial port pointer
    if (!m_port)
        return;

    // Read incoming data
    m_dataLen += m_port->bytesAvailable();
    m_buffer.append(m_port->readAll());

    // Buffer contains EOT byte, which represents a packet
    if (m_buffer.contains(EOT_PRIMARY.toLatin1())) {
        // We could have received part of the next packet - or more than one
        // packet - so we split the data into packets to read each one
        // separately.
        QList<QByteArray> packets = m_buffer.split(EOT_PRIMARY.toLatin1());

        // Check if last packet is complete or not
        if (m_buffer.endsWith(EOT_PRIMARY.toLatin1()))
            m_buffer.clear();
        else {
            m_buffer = packets.last();
            packets.removeLast();
        }

        // Read each packet separately
        foreach (QByteArray packet, packets) {
            if (!packet.isEmpty())
                emit packetReceived(packet);
        }
    }

    // Ensure that buffer stays withing size limits
    if (m_buffer.size() > MAX_BUFFER_SIZE)
        m_buffer.clear();
}

/**
 * Closes the comm. channel with the current serial device and notifies
 * the rest of the application modules, so that the following tasks can
 * be performed:
 *     - Close the current log/received data file
 *     - Update the user interface
 *     - Disconnect serial device signals/slots to the SerialManager
 */
void Serial::disconnectDevice() {
    // Reset byte counter
    m_dataLen = -1;

    // Check if serial port pointer is valid
    if (m_port != Q_NULLPTR) {
        // Get serial port name
        QString name = m_port->portName();

        // Disconnect signals/slots of serial port
        m_port->disconnect(this, SLOT(onDataReceived()));
        m_port->disconnect(this, SLOT(disconnectDevice()));

        // Close and delete the serial port
        m_port->close();
        m_port->deleteLater();

        // Reset pointer
        m_port = Q_NULLPTR;

        // Warn user(if serial port was valid)
        if (!name.isEmpty())
            emit connectionError(name);
    }

    // Update UI
    emit connectionChanged();
}

/**
 * @brief SerialManager::refreshSerialDevices
 */
void Serial::refreshSerialDevices() {
    // Create list starting with invalid virtual device
    QStringList devices;
    devices.append(tr("Seleccionar Puerto"));

    // Search for serial devices
    foreach(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        if (!port.description().isEmpty() && !port.isNull())
            devices.append(port.description());
    }

    // If the obtained list is different from previous list, update UI
    if (devices != m_serialDevices) {
        m_serialDevices = devices;
        emit serialDevicesChanged();
    }

    // Call this function again in one second
    QTimer::singleShot(1000, this, &Serial::refreshSerialDevices);
}

/**
 * @brief SerialManager::sizeStr
 * @param bytes
 * @return
 */
QString Serial::sizeStr(const qint64 bytes) const {
    QString units;
    double size = bytes;

    if (size < 1024)
        units = " " + tr("bytes");

    else if (size < pow(1024, 2)) {
        size /= 1024.0;
        units = " " + tr("KB");
    }

    else {
        size /= pow(1024, 2);
        units = " " + tr("MB");
    }

    return QString::number(size, 'g', 2) + units;
}
