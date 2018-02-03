/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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

#include "roommodelgui.h"
#include <QApplication>
#include <QStandardPaths>
#include <QHBoxLayout>
#include <QQmlContext>
#include <qqmlapplicationengine.h>
#include <ruqolautils.h>
#include <ruqola.h>
#include <rocketchataccount.h>
#include <model/roomfilterproxymodel.h>
#include <KLocalizedContext>

RoomModelGui::RoomModelGui(QWidget *parent)
    : QWidget(parent)
{
    RocketChatAccount *account = new RocketChatAccount(QString(), this);
    mModel = new RoomModel(account, this);
    mRoomFilterProxyModel = new RoomFilterProxyModel(this);
    mRoomFilterProxyModel->setSourceModel(mModel);
    fillModels();
    qmlRegisterSingletonType(QUrl(QStringLiteral("qrc:/ExtraColors.qml")), "KDE.Ruqola.ExtraColors", 1, 0, "RuqolaSingleton");
    qmlRegisterSingletonType<RuqolaUtils>("KDE.Ruqola.RuqolaUtils", 1, 0, "RuqolaUtils", ruqolautils_singletontype_provider);
    qmlRegisterSingletonType<Ruqola>("KDE.Ruqola.Ruqola", 1, 0, "Ruqola", ruqola_singletontype_provider);
    qmlRegisterType<RoomFilterProxyModel>("KDE.Ruqola.RoomFilterProxyModel", 1, 0, "RoomFilterProxyModel");
    qmlRegisterType<RocketChatAccount>("KDE.Ruqola.RocketChatAccount", 1, 0, "RocketChatAccount");
    qRegisterMetaType<Message::MessageType>();
    qmlRegisterUncreatableType<Message>("KDE.Ruqola.Message", 1, 0, "Message", QStringLiteral("MessageType is an enum container"));
    mEngine = new QQmlApplicationEngine(this);

    QQmlContext *ctxt = mEngine->rootContext();
    ctxt->setContextObject(new KLocalizedContext(mEngine));
    ctxt->setContextProperty(QStringLiteral("roomModel"), mRoomFilterProxyModel);

    mEngine->load(QUrl(QStringLiteral("qrc:/messagemodelgui.qml")));
}

RoomModelGui::~RoomModelGui()
{
}

void RoomModelGui::fillModels()
{
    //TODO
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);

    RoomModelGui w;
    w.show();
    return app.exec();
}
