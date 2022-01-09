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

#include "websearch.h"

#include <QFile>
#include <QLocale>
#include <QDomDocument>
#include <QDebug>
#include <QDomDocument>

#define PLUGIN_ID "fr.swifty.websearch"

/**
 * Return the keywords with the answers that match in xml format
 *
 * @return the keywords in xml format
 */
QString WebSearch::getDataXml()
{
    const QString locale = QLocale::system().name().section('_', 0, 0);
    QFile file(":/XML/WebSearchData_"+locale+".xml");
    if (!file.exists()) {
        file.setFileName(":/XML/WebSearchData_en.xml");
    }
    file.open(QIODevice::ReadOnly);

    return file.readAll();
}

/**
 * Returns the unique identifier of the plugin
 *
 * @return the id
 */
QString WebSearch::pluginId()
{
    return PLUGIN_ID;
}

/**
 * Returns a list of proposals related to the plugin
 *
 * @return the list of proposals
 */
QList<QString> WebSearch::getCommande()
{
    const QList<QString> list;

    return list;
}

/**
 * Called when a special action is defined in the xml
 */
void WebSearch::execAction(QList<QString> cmd)
{
    if (isFirst) connect(&networkManager, &QNetworkAccessManager::finished, this, &WebSearch::handleNetworkData);
    isFirst = false;

    if (cmd.at(0) == "websearch") {
        cmdText = cmd.at(1);
        urlDuckDuckGo = "https://api.duckduckgo.com/?q="+cmdText+"&format=xml";
        networkManager.get(QNetworkRequest(urlDuckDuckGo));
    }
    else if (cmd.at(0) == "showDetails") {
        networkManager.get(QNetworkRequest(urlDuckDuckGo));
        isDetails = true;
    }
}

/**
 * Called when a custom interface is displayed and sends a message
 */
void WebSearch::messageReceived(QString, QString id)
{
    if (id == PLUGIN_ID) {
        // Insert here the code
    }
}

void WebSearch::handleNetworkData(QNetworkReply *networkReply)
{
    if (networkReply->error() == QNetworkReply::NoError) {
        const QByteArray response(networkReply->readAll());
        QDomDocument doc;
        doc.setContent(response);

        const QDomElement root = doc.documentElement();
        QDomElement element = root.firstChildElement();

        bool isResult = false;

        QString heading;
        QString image;
        QString abstractText;
        QString abstractUrl;
        QString abstractSource;

        QString result;
        QString urlResult;
        QString urlIconResult;

        while (!element.isNull()) {
            if (element.tagName() == "Type") {
                if (element.text() == "A") isResult = true;
            }

            else if (element.tagName() == "Heading" && isResult) heading = element.text();

            else if (element.tagName() == "Image" && isResult) image = "https://api.duckduckgo.com"+element.text();

            else if (element.tagName() == "AbstractText" && isResult) abstractText = element.text();

            else if (element.tagName() == "AbstractURL" && isResult) abstractUrl = element.text();

            else if (element.tagName() == "AbstractSource" && isResult) abstractSource = element.text();

            else if (element.tagName() == "Results" && isResult) {
                if (!element.firstChildElement().isNull()) {
                    QDomElement resultElements = element.firstChildElement().firstChildElement();

                    while (!resultElements.isNull()) {
                        if (resultElements.tagName() == "Icon") urlIconResult = "https://api.duckduckgo.com"+resultElements.text();

                        else if (resultElements.tagName() == "Text") result = resultElements.text();

                        else if (resultElements.tagName() == "FirstURL") urlResult = resultElements.text();

                        resultElements = resultElements.nextSiblingElement();
                    }
                }
            }

            element = element.nextSiblingElement();
        }

        if (isResult && !isDetails) {
            QString text = abstractText;
            text.truncate(150);
            text.append("...");

            emit sendMessage(tr("Voici ce que j'ai trouver sur internet pour ")+heading+":", false, "message", PLUGIN_ID);
            emit sendMessage(text, true, "message", PLUGIN_ID, QList<QString>() << "showDetails", QList<QString>() << tr("Detail"));
        }
        else if (isResult && isDetails) {
            QFile qmlFile(":/qml/src/res/DetailDuckDuckGo.qml");
            qmlFile.open(QIODevice::ReadOnly);

            emit showQml(qmlFile.readAll(), PLUGIN_ID);

            qmlFile.close();

            emit sendMessageToQml("h "+heading);
            if (!urlResult.isEmpty()) emit sendMessageToQml("l "+urlResult);
            emit sendMessageToQml("i "+image);
            emit sendMessageToQml("c "+abstractText);
            emit sendMessageToQml("s "+abstractSource);
            emit sendMessageToQml("u "+abstractUrl);

            isDetails = false;
        }
        else {
            emit sendMessage(tr("Désolé, je ne comprends pas ! 😕"), true, "message", PLUGIN_ID, QList<QString>() << "web_message with_action_btn search "+cmdText, QList<QString>() << tr("Chercher sur le web"));
        }
    }
    else {
        emit sendMessage(tr("Désolé, je n'ai pas trouvé d'accès à internet sur votre pc !"), true, "message", PLUGIN_ID);
    }

    networkReply->deleteLater();
}
