/*
  SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolacentralwidget.h"
#include "rocketchataccount.h"
#include "ruqolaglobalconfig.h"
#include "ruqolaloginwidget.h"
#include "ruqolamainwidget.h"
#include "servererrorinfo.h"
#include "servererrorinfohistory/servererrorinfomessagewidget.h"
#include "servererrorinfohistorymanager.h"
#include "whatsnew/whatsnewmessagewidget.h"
#include "whatsnew/whatsnewwidget.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QStackedWidget>

RuqolaCentralWidget::RuqolaCentralWidget(QWidget *parent)
    : QWidget(parent)
    , mStackedWidget(new QStackedWidget(this))
    , mRuqolaMainWidget(new RuqolaMainWidget(this))
    , mRuqolaLoginWidget(new RuqolaLoginWidget(this))
    , mMainLayout(new QVBoxLayout(this))
{
    mMainLayout->setContentsMargins({});
    mMainLayout->setObjectName(QStringLiteral("mainlayout"));
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    mMainLayout->setSpacing(0);
#endif

    const QString newFeaturesMD5 = WhatsNewWidget::newFeaturesMD5();
    const bool hasNewFeature = (RuqolaGlobalConfig::self()->previousNewFeaturesMD5() != newFeaturesMD5);
    if (hasNewFeature) {
        auto whatsNewMessageWidget = new WhatsNewMessageWidget(this);
        whatsNewMessageWidget->setObjectName(QStringLiteral("whatsNewMessageWidget"));
        mMainLayout->addWidget(whatsNewMessageWidget);
        RuqolaGlobalConfig::self()->setPreviousNewFeaturesMD5(newFeaturesMD5);
        whatsNewMessageWidget->animatedShow();
    }

    mStackedWidget->setObjectName(QStringLiteral("mStackedWidget"));
    mMainLayout->addWidget(mStackedWidget);

    mRuqolaMainWidget->setObjectName(QStringLiteral("mRuqolaMainWidget"));
    mStackedWidget->addWidget(mRuqolaMainWidget);

    mRuqolaLoginWidget->setObjectName(QStringLiteral("mRuqolaLoginWidget"));
    mStackedWidget->addWidget(mRuqolaLoginWidget);

    mStackedWidget->setCurrentWidget(mRuqolaLoginWidget);
    connect(mRuqolaMainWidget, &RuqolaMainWidget::channelSelected, this, &RuqolaCentralWidget::channelSelected);
    connect(ServerErrorInfoHistoryManager::self(), &ServerErrorInfoHistoryManager::newServerErrorInfo, this, &RuqolaCentralWidget::slotNewErrorInfo);
}

RuqolaCentralWidget::~RuqolaCentralWidget() = default;

void RuqolaCentralWidget::createServerErrorInfoMessageWidget()
{
    mServerErrorInfoMessageWidget = new ServerErrorInfoMessageWidget(this);
    mServerErrorInfoMessageWidget->setObjectName(QStringLiteral("mServerErrorInfoMessageWidget"));
    mMainLayout->insertWidget(0, mServerErrorInfoMessageWidget);
}

void RuqolaCentralWidget::slotNewErrorInfo()
{
    if (!mServerErrorInfoMessageWidget) {
        createServerErrorInfoMessageWidget();
    }
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

#include "moc_ruqolacentralwidget.cpp"
