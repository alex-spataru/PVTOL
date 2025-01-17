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

#include <QtQml>
#include <QQuickStyle>
#include <QApplication>
#include <QQmlApplicationEngine>

#include "Serial.h"
#include "DataManager.h"

/**
 * Entry-point function of the application
 *
 * @param argc system argument count
 * @param argv system argument strings
 * @return
 */
int main(int argc, char** argv) {
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    app.setOrganizationName("UNAQ");
    app.setApplicationName("Banco de Pruebas PVTOL");
    app.setApplicationVersion("0.1");

    DataManager manager;
    QQmlApplicationEngine engine;
    QQuickStyle::setStyle("Universal");
    engine.rootContext()->setContextProperty("CAppName", app.applicationName());
    engine.rootContext()->setContextProperty("CAppVersion", app.applicationVersion());
    engine.rootContext()->setContextProperty("CSerial", Serial::getInstance());
    engine.rootContext()->setContextProperty("CManager", &manager);
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    if (engine.rootObjects().isEmpty())
        return EXIT_FAILURE;

    return app.exec();
}
