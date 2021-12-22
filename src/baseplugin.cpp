/* BasePlugin is an extention for Swifty Assistant.

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

#include "baseplugin.h"

#include <QFile>
#include <QLocale>

#define PLUGIN_ID "fr.swifty.baseplugin"

/**
 * Return the keywords with the answers that match in xml format
 *
 * @return the keywords in xml format
 */
QString BasePlugin::getDataXml()
{
    QString locale = QLocale::system().name().section('_', 0, 0);
    QFile file(":/XML/BasePluginData_"+locale+".xml");
    if (!file.exists()) {
        file.setFileName(":/XML/BasePluginData_en.xml");
    }
    file.open(QIODevice::ReadOnly);

    return file.readAll();
}

/**
 * Returns the unique identifier of the plugin
 *
 * @return the id
 */
QString BasePlugin::pluginId()
{
    return PLUGIN_ID;
}

/**
 * Returns a list of proposals related to the plugin
 *
 * @return the list of proposals
 */
QList<QString> BasePlugin::getCommande()
{
    QList<QString> list;
    QString locale = QLocale::system().name().section('_', 0, 0);

    if (locale == "fr") list << "Entrez ici les propositions principals";
    else list << "Enter the main proposals here";

    return list;
}

/**
 * Called when a special action is defined in the xml
 */
void BasePlugin::execAction(QList<QString>) {}

/**
 * Called when a custom interface is displayed and sends a message
 */
void BasePlugin::messageReceived(QString, QString id)
{
    if (id == PLUGIN_ID) {
        // Insert here the code
    }
}
