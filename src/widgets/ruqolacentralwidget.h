/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "config-ruqola.h"
#include "libruqolawidgets_private_export.h"
#if HAVE_WHATSNEWSNGSUPPORT
#include <KAboutData>
#endif
#include "room.h"
#include <QAbstractSocket>
#include <QPointer>
#include <QWidget>
class QStackedWidget;
class RuqolaMainWidget;
class RuqolaLoginWidget;
class RocketChatAccount;
class ServerErrorInfoMessageWidget;
class QVBoxLayout;
class WelcomeWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RuqolaCentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RuqolaCentralWidget(
#if HAVE_WHATSNEWSNGSUPPORT
        const QList<KAboutRelease> &releases,
#endif
        QWidget *parent = nullptr);
    ~RuqolaCentralWidget() override;
    [[nodiscard]] QByteArray roomId() const;

    void setCurrentRocketChatAccount(RocketChatAccount *account);
    void selectNextUnreadChannel();
    [[nodiscard]] Room::RoomType roomType() const;
    [[nodiscard]] Room *room() const;
#if HAVE_WHATSNEWSNGSUPPORT
    [[nodiscard]] QList<KAboutRelease> releasesInfo() const;
#endif

Q_SIGNALS:
    void channelSelected();
    void loginPageActivated(bool showLoginPage);
    void createNewAccount();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLoginStatusChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotJobFailedInfo(const QString &messageError, const QString &accountName);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSocketError(QAbstractSocket::SocketError error, const QString &errorString);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotNewErrorInfo();
    LIBRUQOLAWIDGETS_NO_EXPORT void createServerErrorInfoMessageWidget();

#if HAVE_WHATSNEWSNGSUPPORT
    QList<KAboutRelease> mReleasesInfo;
#endif
    QStackedWidget *const mStackedWidget;
    RuqolaMainWidget *const mRuqolaMainWidget;
    RuqolaLoginWidget *const mRuqolaLoginWidget;
    ServerErrorInfoMessageWidget *mServerErrorInfoMessageWidget = nullptr;
    WelcomeWidget *const mRuqolaWelcomeWidget;
    QVBoxLayout *const mMainLayout;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
};
