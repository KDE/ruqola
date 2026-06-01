/*
  SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolacentralwidget.h"
#include "accountmanager.h"
#include "loginwidget/ruqolaloginwidget.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolaglobalconfig.h"
#include "ruqolamainwidget.h"
#include "servererrorinfo.h"
#include "servererrorinfohistory/servererrorinfomessagewidget.h"
#include "servererrorinfohistorymanager.h"
#include "welcome/welcomewidget.h"

#if HAVE_WHATSNEWSNGSUPPORT
#include <TextAddonsWidgets/WhatsNewMessageNgWidget>
#include <TextAddonsWidgets/WhatsNewNgUtils>
#else
#include "whatsnew/whatsnewtranslations.h"
#include <TextAddonsWidgets/WhatsNewMessageWidget>
#endif

#include <KLocalizedString>
#include <QStackedWidget>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
RuqolaCentralWidget::RuqolaCentralWidget(
#if HAVE_WHATSNEWSNGSUPPORT
    const QList<KAboutRelease> &releases,
#endif
    QWidget *parent)
    : QWidget(parent)
    , mStackedWidget(new QStackedWidget(this))
    , mRuqolaMainWidget(new RuqolaMainWidget(this))
    , mRuqolaLoginWidget(new RuqolaLoginWidget(this))
    , mRuqolaWelcomeWidget(new WelcomeWidget(this))
    , mMainLayout(new QVBoxLayout(this))
{
    mMainLayout->setContentsMargins({});
    mMainLayout->setObjectName(u"mainlayout"_s);
    mMainLayout->setSpacing(0);
    QString newFeaturesMD5;
#if HAVE_WHATSNEWSNGSUPPORT
    mReleasesInfo = releases;
    if (!mReleasesInfo.isEmpty()) {
        newFeaturesMD5 = TextAddonsWidgets::WhatsNewNgUtils::createMD5(mReleasesInfo.constFirst().untranslatedDescription());
    }
#else
    const WhatsNewTranslations translations;
    newFeaturesMD5 = translations.newFeaturesMD5();
#endif
    if (!newFeaturesMD5.isEmpty()) {
        const QString previousNewFeaturesMD5 = RuqolaGlobalConfig::self()->previousNewFeaturesMD5();
        if (!previousNewFeaturesMD5.isEmpty()) {
            const bool hasNewFeature = (previousNewFeaturesMD5 != newFeaturesMD5);
            if (hasNewFeature) {
#if HAVE_WHATSNEWSNGSUPPORT
                auto whatsNewMessageWidget = new TextAddonsWidgets::WhatsNewMessageNgWidget(this);
                whatsNewMessageWidget->setReleases(mReleasesInfo);
                whatsNewMessageWidget->setObjectName(u"whatsNewMessageWidget"_s);
                mMainLayout->addWidget(whatsNewMessageWidget);
                RuqolaGlobalConfig::self()->setPreviousNewFeaturesMD5(newFeaturesMD5);
                RuqolaGlobalConfig::self()->save();
                whatsNewMessageWidget->animatedShow();
#else
                auto whatsNewMessageWidget = new TextAddonsWidgets::WhatsNewMessageWidget(this);
                whatsNewMessageWidget->setWhatsNewInfos(translations.createWhatsNewInfo());
                whatsNewMessageWidget->setObjectName(u"whatsNewMessageWidget"_s);
                mMainLayout->addWidget(whatsNewMessageWidget);
                RuqolaGlobalConfig::self()->setPreviousNewFeaturesMD5(newFeaturesMD5);
                RuqolaGlobalConfig::self()->save();
                whatsNewMessageWidget->animatedShow();
#endif
            }
        } else {
            RuqolaGlobalConfig::self()->setPreviousNewFeaturesMD5(newFeaturesMD5);
            RuqolaGlobalConfig::self()->save();
        }
    }

    mStackedWidget->setObjectName(u"mStackedWidget"_s);
    mMainLayout->addWidget(mStackedWidget);

    mRuqolaMainWidget->setObjectName(u"mRuqolaMainWidget"_s);
    mStackedWidget->addWidget(mRuqolaMainWidget);

    mRuqolaLoginWidget->setObjectName(u"mRuqolaLoginWidget"_s);
    mStackedWidget->addWidget(mRuqolaLoginWidget);

    mRuqolaWelcomeWidget->setObjectName(u"mRuqolaWelcomeWidget"_s);
    mStackedWidget->addWidget(mRuqolaWelcomeWidget);
    connect(mRuqolaWelcomeWidget, &WelcomeWidget::createNewAccount, this, &RuqolaCentralWidget::createNewAccount);

    mStackedWidget->setCurrentWidget(mRuqolaLoginWidget);
    mRuqolaLoginWidget->forceLoginFocus();
    connect(mRuqolaMainWidget, &RuqolaMainWidget::channelSelected, this, &RuqolaCentralWidget::channelSelected);
    connect(ServerErrorInfoHistoryManager::self(), &ServerErrorInfoHistoryManager::newServerErrorInfo, this, &RuqolaCentralWidget::slotNewErrorInfo);
    if (Ruqola::self()->accountManager()->isEmpty()) {
        mStackedWidget->setCurrentWidget(mRuqolaWelcomeWidget);
    }
}

RuqolaCentralWidget::~RuqolaCentralWidget() = default;

void RuqolaCentralWidget::createServerErrorInfoMessageWidget()
{
    mServerErrorInfoMessageWidget = new ServerErrorInfoMessageWidget(this);
    mServerErrorInfoMessageWidget->setObjectName(u"mServerErrorInfoMessageWidget"_s);
    mMainLayout->insertWidget(0, mServerErrorInfoMessageWidget);
}

#if HAVE_WHATSNEWSNGSUPPORT
QList<KAboutRelease> RuqolaCentralWidget::releasesInfo() const
{
    return mReleasesInfo;
}
#endif

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

void RuqolaCentralWidget::slotSocketError([[maybe_unused]] QAbstractSocket::SocketError error, const QString &errorString)
{
    // ## let's hope this happens while the login widget is visible, but that's quite likely
    // Testcase: try to connect to a server that doesn't exist
    mRuqolaLoginWidget->showError(errorString);
}

Room *RuqolaCentralWidget::room() const
{
    return mRuqolaMainWidget->room();
}

QByteArray RuqolaCentralWidget::roomId() const
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
    if (mCurrentRocketChatAccount) {
        connect(mCurrentRocketChatAccount, &RocketChatAccount::loginStatusChanged, this, &RuqolaCentralWidget::slotLoginStatusChanged);
        connect(mCurrentRocketChatAccount, &RocketChatAccount::socketError, this, &RuqolaCentralWidget::slotSocketError);
        connect(mCurrentRocketChatAccount, &RocketChatAccount::jobFailed, this, &RuqolaCentralWidget::slotJobFailedInfo);
        mRuqolaMainWidget->setCurrentRocketChatAccount(mCurrentRocketChatAccount);
        // Check if account is connected or not.
        slotLoginStatusChanged();
    }
}

void RuqolaCentralWidget::slotLoginStatusChanged()
{
    const auto loginStatus = mCurrentRocketChatAccount->loginStatus();
    mRuqolaLoginWidget->setLoginStatus(loginStatus);
    bool loginPage = false;
    if (loginStatus == AuthenticationManager::LoggedIn) {
        mStackedWidget->setCurrentWidget(mRuqolaMainWidget);
    } else {
        mStackedWidget->setCurrentWidget(mRuqolaLoginWidget);
        mRuqolaLoginWidget->forceLoginFocus();
        mRuqolaLoginWidget->setRocketChatAccount(mCurrentRocketChatAccount);
        loginPage = true;
    }
    Q_EMIT loginPageActivated(loginPage);
}

#include "moc_ruqolacentralwidget.cpp"
