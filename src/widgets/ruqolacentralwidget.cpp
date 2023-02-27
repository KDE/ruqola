/*
  SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolacentralwidget.h"
#include "rocketchataccount.h"
#include "ruqolaloginwidget.h"
#include "ruqolamainwidget.h"
#include "servererrorinfo.h"
#include "servererrorinfohistory/servererrorinfomessagewidget.h"
#include "servererrorinfohistorymanager.h"
#include "whatsnew/whatsnewmessagewidget.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QStackedWidget>

RuqolaCentralWidget::RuqolaCentralWidget(QWidget *parent)
    : QWidget(parent)
    , mStackedWidget(new QStackedWidget(this))
    , mRuqolaMainWidget(new RuqolaMainWidget(this))
    , mRuqolaLoginWidget(new RuqolaLoginWidget(this))
    , mServerErrorInfoMessageWidget(new ServerErrorInfoMessageWidget(this))
    , mWhatsNewMessageWidget(new WhatsNewMessageWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainlayout"));

    mServerErrorInfoMessageWidget->setObjectName(QStringLiteral("mServerErrorInfoMessageWidget"));
    mainLayout->addWidget(mServerErrorInfoMessageWidget);

    mWhatsNewMessageWidget->setObjectName(QStringLiteral("mWhatsNewMessageWidget"));
    mainLayout->addWidget(mWhatsNewMessageWidget);

    mStackedWidget->setObjectName(QStringLiteral("mStackedWidget"));
    mainLayout->addWidget(mStackedWidget);

    mRuqolaMainWidget->setObjectName(QStringLiteral("mRuqolaMainWidget"));
    mStackedWidget->addWidget(mRuqolaMainWidget);

    mRuqolaLoginWidget->setObjectName(QStringLiteral("mRuqolaLoginWidget"));
    mStackedWidget->addWidget(mRuqolaLoginWidget);

    mStackedWidget->setCurrentWidget(mRuqolaLoginWidget);
    connect(mRuqolaMainWidget, &RuqolaMainWidget::channelSelected, this, &RuqolaCentralWidget::channelSelected);
    connect(ServerErrorInfoHistoryManager::self(), &ServerErrorInfoHistoryManager::newServerErrorInfo, this, &RuqolaCentralWidget::slotNewErrorInfo);
}

RuqolaCentralWidget::~RuqolaCentralWidget() = default;

void RuqolaCentralWidget::slotNewErrorInfo()
{
    mServerErrorInfoMessageWidget->animatedShow();
}

void RuqolaCentralWidget::slotJobFailedInfo(const QString &messageError, const QString &accountName)
{
    ServerErrorInfo info;
    info.setAccountName(accountName);
    info.setMessage(messageError);
    ServerErrorInfoHistoryManager::self()->addServerErrorInfo(std::move(info));
}

void RuqolaCentralWidget::slotSocketError(QAbstractSocket::SocketError error, const QString &errorString)
{
    Q_UNUSED(error)
    // ## let's hope this happens while the login widget is visible, but that's quite likely
    // Testcase: try to connect to a server that doesn't exist
    mRuqolaLoginWidget->showError(errorString);
}

Room *RuqolaCentralWidget::room() const
{
    return mRuqolaMainWidget->room();
}

QString RuqolaCentralWidget::roomId() const
{
    return mRuqolaMainWidget->roomId();
}

Room::RoomType RuqolaCentralWidget::roomType() const
{
    return mRuqolaMainWidget->roomType();
}

void RuqolaCentralWidget::selectNextUnreadChannel()
{
    mRuqolaMainWidget->selectNextUnreadChannel();
}

void RuqolaCentralWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    if (mCurrentRocketChatAccount) {
        disconnect(mCurrentRocketChatAccount, nullptr, this, nullptr);
    }
    mCurrentRocketChatAccount = account;
    connect(mCurrentRocketChatAccount, &RocketChatAccount::loginStatusChanged, this, &RuqolaCentralWidget::slotLoginStatusChanged);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::socketError, this, &RuqolaCentralWidget::slotSocketError);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::jobFailed, this, &RuqolaCentralWidget::slotJobFailedInfo);
    mRuqolaMainWidget->setCurrentRocketChatAccount(mCurrentRocketChatAccount);
    // Check if account is connected or not.
    slotLoginStatusChanged();
}

void RuqolaCentralWidget::slotLoginStatusChanged()
{
    const auto loginStatus = mCurrentRocketChatAccount->loginStatus();
    mRuqolaLoginWidget->setLoginStatus(loginStatus);
    bool loginPage = false;
    if (loginStatus == DDPAuthenticationManager::LoggedIn) {
        mStackedWidget->setCurrentWidget(mRuqolaMainWidget);
    } else {
        mStackedWidget->setCurrentWidget(mRuqolaLoginWidget);
        mRuqolaLoginWidget->setRocketChatAccount(mCurrentRocketChatAccount);
        loginPage = true;
    }
    Q_EMIT loginPageActivated(loginPage);
}
