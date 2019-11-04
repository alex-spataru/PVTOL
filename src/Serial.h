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

#ifndef SERIAL_MANAGER_H
#define SERIAL_MANAGER_H

#include <QtQml>
#include <QObject>

class QSerialPort;
class Serial : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool connected
               READ connected
               NOTIFY connectionChanged)
    Q_PROPERTY(bool fileLoggingEnabled
               READ fileLoggingEnabled
               WRITE enableFileLogging
               NOTIFY fileLoggingEnabledChanged)
    Q_PROPERTY(QString receivedBytes
               READ receivedBytes
               NOTIFY packetReceived)
    Q_PROPERTY(QStringList serialDevices
               READ serialDevices
               NOTIFY serialDevicesChanged)
    Q_PROPERTY(QStringList baudRates
               READ baudRates
               CONSTANT)
    Q_PROPERTY(int baudRate
               READ baudRate
               WRITE setBaudRate
               NOTIFY baudRateChanged)

signals:
    void baudRateChanged();
    void connectionChanged();
    void serialDevicesChanged();
    void fileLoggingEnabledChanged();
    void packetLogged(const QString& data);
    void packetReceived(const QByteArray& data);
    void connectionError(const QString& deviceName);
    void connectionSuccess(const QString& deviceName);

private:
    Serial();
    ~Serial();

public:
    static Serial* getInstance();

    int baudRate() const;
    bool connected() const;
    bool fileLoggingEnabled() const;

    QString deviceName() const;
    QString receivedBytes() const;
    QStringList baudRates() const;
    QStringList serialDevices() const;
    
    QSerialPort* port() {
        return m_port;
    }

public slots:
    void openLogFile();
    void setBaudRate(const int rate);
    void startComm(const int device);
    void enableFileLogging(const bool enabled);

private slots:
    void onDataReceived();
    void disconnectDevice();
    void configureLogFile();
    void refreshSerialDevices();
    void formatReceivedPacket(const QByteArray& data);

private:
    bool packetLogAvailable() const;
    QString sizeStr(const qint64 bytes) const;

private:
    int m_baudRate;
    QFile m_packetLog;
    qint64 m_dataLen;
    QByteArray m_buffer;
    QSerialPort* m_port;
    QStringList m_serialDevices;

    bool m_enableFileLogging;
};

#endif
