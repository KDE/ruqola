/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "messagemodelgui.h"

#include <roomfilterproxymodel.h>
#include <message.h>
#include "messagemodel.h"

#include <KLocalizedContext>

#include <QApplication>
#include <QStandardPaths>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QHBoxLayout>

MessageModelGui::MessageModelGui(QWidget *parent)
    : QWidget(parent)
{
    fillModel();
    qmlRegisterType<RoomFilterProxyModel>("KDE.Ruqola.RoomFilterProxyModel", 1, 0, "RoomFilterProxyModel");
    qRegisterMetaType<Message::MessageType>();
    qmlRegisterUncreatableType<Message>("KDE.Ruqola.Message", 1, 0, "Message", QStringLiteral("MessageType is an enum container"));
    mEngine = new QQmlApplicationEngine;

    QQmlContext *ctxt = mEngine->rootContext();
    ctxt->setContextObject(new KLocalizedContext(mEngine));

    //Add model

    //Customize it.
    mEngine->load(QUrl(QStringLiteral("qrc:/Desktop.qml")));

    if (mEngine->rootObjects().isEmpty()) {
        //TODO create widget
    }
}

void MessageModelGui::fillModel()
{
    mModel = new MessageModel();
    Message m1;
    m1.setMessageType(Message::MessageType::NormalText);
    m1.setMessageId(QStringLiteral("1"));
    m1.setText(QStringLiteral("foo"));
    m1.setTimeStamp(1);

    mModel->addMessage(m1);
}

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);



    MessageModelGui w;
    w.show();
    return app.exec();
}
