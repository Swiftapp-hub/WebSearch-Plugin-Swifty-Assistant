/* WebSearch is an extention for Swifty Assistant.

   Copyright (C) <2021>  <SwiftyApp>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#ifndef ECHOPLUGIN_H
#define ECHOPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include <QtNetwork>

#include "plugininterface.h"

class WebSearch : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "fr.swiftapp.swiftyassistant.plugin" FILE "res/websearch.json")
    Q_INTERFACES(PluginInterface)

public:
    QString getDataXml() override;
    QString pluginId() override;
    void execAction(QList<QString> cmd) override;
    QList<QString> getCommande() override;
    QObject* getObject() { return this; }

private:
    QNetworkAccessManager networkManager;
    bool isFirst = true;

    bool isDetails = false;
    QString urlDuckDuckGo;
    QString cmdText;

signals:
    void sendMessage(QString reply, bool isFin, QString typeMessage, QString id, QList<QString> url = QList<QString>(), QList<QString> textUrl = QList<QString>());
    void sendMessageToQml(QString message);
    void showQml(QString qml, QString id);
    void execAction(QString action);

    void html(QString html);

public slots:
    void messageReceived(QString message, QString pluginIid) override;

    void handleNetworkData(QNetworkReply *networkReply);
};

#endif
