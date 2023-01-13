/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "room.h"
#include <QAbstractSocket>
#include <QPointer>
#include <QWidget>
class QStackedWidget;
class RuqolaMainWidget;
class RuqolaLoginWidget;
class RocketChatAccount;
class ServerErrorInfoMessageWidget;
class Room;

class LIBRUQOLAWIDGETS_TESTS_EXPORT RuqolaCentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RuqolaCentralWidget(QWidget *parent = nullptr);
    ~RuqolaCentralWidget() override;
    Q_REQUIRED_RESULT QString roomId() const;

    void setCurrentRocketChatAccount(RocketChatAccount *account);
    void selectNextUnreadChannel();
    Q_REQUIRED_RESULT Room::RoomType roomType() const;
    Room *room() const;
Q_SIGNALS:
    void channelSelected();
    void loginPageActivated(bool showLoginPage);

private:
    void slotLoginStatusChanged();
    void slotJobFailedInfo(const QString &messageError, const QString &accountName);
    void slotSocketError(QAbstractSocket::SocketError error, const QString &errorString);

    QStackedWidget *const mStackedWidget;
    RuqolaMainWidget *const mRuqolaMainWidget;
    RuqolaLoginWidget *const mRuqolaLoginWidget;
    ServerErrorInfoMessageWidget *const mServerErrorInfoMessageWidget;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
};
