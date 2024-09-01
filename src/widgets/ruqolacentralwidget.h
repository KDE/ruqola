/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

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
class QVBoxLayout;
class WelcomeWidget;

class LIBRUQOLAWIDGETS_TESTS_EXPORT RuqolaCentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RuqolaCentralWidget(QWidget *parent = nullptr);
    ~RuqolaCentralWidget() override;
    [[nodiscard]] QByteArray roomId() const;

    void setCurrentRocketChatAccount(RocketChatAccount *account);
    void selectNextUnreadChannel();
    [[nodiscard]] Room::RoomType roomType() const;
    [[nodiscard]] Room *room() const;
Q_SIGNALS:
    void channelSelected();
    void loginPageActivated(bool showLoginPage);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLoginStatusChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotJobFailedInfo(const QString &messageError, const QString &accountName);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSocketError(QAbstractSocket::SocketError error, const QString &errorString);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotNewErrorInfo();
    LIBRUQOLAWIDGETS_NO_EXPORT void createServerErrorInfoMessageWidget();

    QStackedWidget *const mStackedWidget;
    RuqolaMainWidget *const mRuqolaMainWidget;
    RuqolaLoginWidget *const mRuqolaLoginWidget;
    ServerErrorInfoMessageWidget *mServerErrorInfoMessageWidget = nullptr;
    WelcomeWidget *const mRuqolaWelcomeWidget;
    QVBoxLayout *const mMainLayout;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
};
