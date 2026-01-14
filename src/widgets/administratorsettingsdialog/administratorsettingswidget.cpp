/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorsettingswidget.h"

#include "accounts/accountsettingswidget.h"
#include "analytics/analyticswidget.h"
#include "cas/cassettingswidget.h"
#include "conferencecall/conferencecallsettingswidget.h"
#include "ddpapi/ddpclient.h"
#include "email/emailsettingswidget.h"
#include "encryption/encryptionsettingswidget.h"
#include "enterprise/enterprisesettingswidget.h"
#include "fileupload/fileuploadsettingswidget.h"
#include "general/generalsettingswidget.h"
#include "irc/ircwidget.h"
#include "ircfederation/ircfederationwidget.h"
#include "layout/layoutsettingswidget.h"
#include "ldap/ldapsettingswidget.h"
#include "logs/logssettingswidget.h"
#include "message/messagesettingswidget.h"
#include "mobile/mobilesettingswidget.h"
#include "oauth/oauthsettingswidget.h"
#include "password/passwordsettingswidget.h"
#include "ratelimiter/ratelimiterwidget.h"
#include "retentionpolicy/retentionpolicysettingswidget.h"
#include "rocketchataccount.h"
#include "rocketchatbackend.h"
#include "slackbridge/slackbridgewidget.h"
#include "troubleshoot/troubleshootsettingswidget.h"
#include "userdatadownload/userdatadownloadwidget.h"
#include "videoconference/videoconferencewidget.h"
#include "webdav/webdavsettingswidget.h"
#include "webrtc/webrtcsettingswidget.h"
#include <KLocalizedString>
#include <QTabWidget>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
AdministratorSettingsWidget::AdministratorSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mTabWidget(new QTabWidget(this))
    , mAccountSettingsWidget(new AccountSettingsWidget(account, this))
    , mEncryptionSettingsWidget(new EncryptionSettingsWidget(account, this))
    , mMessageSettingsWidget(new MessageSettingsWidget(account, this))
    , mUploadFileSettingsWidget(new FileUploadSettingsWidget(account, this))
    , mRetentionPolicySettingsWidget(new RetentionPolicySettingsWidget(account, this))
    , mGeneralSettingsWidget(new GeneralSettingsWidget(account, this))
    , mRateLimiterWidget(new RateLimiterWidget(account, this))
    , mPasswordSettingsWidget(new PasswordSettingsWidget(account, this))
    , mVideoConferenceWidget(new VideoConferenceWidget(account, this))
    , mIrcFederationWidget(new IrcFederationWidget(account, this))
    , mWebDavSettingsWidget(new WebDavSettingsWidget(account, this))
    , mLDapSettingsWidget(new LDapSettingsWidget(account, this))
    , mLayoutSettingsWidget(new LayoutSettingsWidget(account, this))
    , mEnterpriseSettingsWidget(new EnterpriseSettingsWidget(account, this))
    , mUserDataDownloadWidget(new UserDataDownloadWidget(account, this))
    , mSlackBridgeWidget(new SlackBridgeWidget(account, this))
    , mLogsSettingsWidget(new LogsSettingsWidget(account, this))
    , mEmailSettingsWidget(new EmailSettingsWidget(account, this))
    , mMobileSettingsWidget(new MobileSettingsWidget(account, this))
    , mTroubleshootSettingsWidget(new TroubleshootSettingsWidget(account, this))
    , mConferenceCallSettingsWidget(new ConferenceCallSettingsWidget(account, this))
    , mWebRtcSettingsWidget(new WebRtcSettingsWidget(account, this))
    , mCasSettingsWidget(new CasSettingsWidget(account, this))
    , mOauthSettingsWidget(new OauthSettingsWidget(account, this))
    , mAnalyticsWidget(new AnalyticsWidget(account, this))
    , mIrcWidget(new IrcWidget(account, this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mTabWidget->setObjectName(u"mTabWidget"_s);
    mainLayout->addWidget(mTabWidget);

    mAccountSettingsWidget->setObjectName(u"mAccountSettingsWidget"_s);
    mEncryptionSettingsWidget->setObjectName(u"mEncryptionSettingsWidget"_s);
    mMessageSettingsWidget->setObjectName(u"mMessageSettingsWidget"_s);
    mUploadFileSettingsWidget->setObjectName(u"mUploadFileSettingsWidget"_s);
    mRetentionPolicySettingsWidget->setObjectName(u"mRetentionPolicySettingsWidget"_s);
    mGeneralSettingsWidget->setObjectName(u"mGeneralSettingsWidget"_s);
    mRateLimiterWidget->setObjectName(u"mRateLimiterWidget"_s);
    mPasswordSettingsWidget->setObjectName(u"mPasswordSettingsWidget"_s);
    mVideoConferenceWidget->setObjectName(u"mVideoConferenceWidget"_s);
    mIrcFederationWidget->setObjectName(u"mIrcFederationWidget"_s);
    mWebDavSettingsWidget->setObjectName(u"mWebDavSettingsWidget"_s);
    mLDapSettingsWidget->setObjectName(u"mLDapSettingsWidget"_s);
    mLayoutSettingsWidget->setObjectName(u"mLayoutSettingsWidget"_s);
    mEnterpriseSettingsWidget->setObjectName(u"mEnterpriseSettingsWidget"_s);
    mUserDataDownloadWidget->setObjectName(u"mUserDataDownloadWidget"_s);
    mSlackBridgeWidget->setObjectName(u"mSlackBridgeWidget"_s);
    mLogsSettingsWidget->setObjectName(u"mLogsSettingsWidget"_s);
    mEmailSettingsWidget->setObjectName(u"mEmailSettingsWidget"_s);
    mMobileSettingsWidget->setObjectName(u"mMobileSettingsWidget"_s);
    mTroubleshootSettingsWidget->setObjectName(u"mTroubleshootSettingsWidget"_s);
    mConferenceCallSettingsWidget->setObjectName(u"mConferenceCallSettingsWidget"_s);
    mWebRtcSettingsWidget->setObjectName(u"mWebRtcSettingsWidget"_s);
    mCasSettingsWidget->setObjectName(u"mCasSettingsWidget"_s);
    mOauthSettingsWidget->setObjectName(u"mOauthSettingsWidget"_s);
    mAnalyticsWidget->setObjectName(u"mAnalyticsWidget"_s);
    mIrcWidget->setObjectName(u"mIrcWidget"_s);
    mTabWidget->addTab(mAccountSettingsWidget, i18n("Accounts"));
    mTabWidget->addTab(mEncryptionSettingsWidget, i18n("Encryption"));
    mTabWidget->addTab(mMessageSettingsWidget, i18n("Message"));
    mTabWidget->addTab(mUploadFileSettingsWidget, i18n("File Upload"));
    mTabWidget->addTab(mRetentionPolicySettingsWidget, i18n("Retention Policy"));
    mTabWidget->addTab(mGeneralSettingsWidget, i18n("General"));
    mTabWidget->addTab(mRateLimiterWidget, i18n("Rate Limiter"));
    mTabWidget->addTab(mPasswordSettingsWidget, i18n("Password"));
    mTabWidget->addTab(mVideoConferenceWidget, i18n("Video Conference"));
    mTabWidget->addTab(mIrcFederationWidget, i18n("IRC Federation"));
    mTabWidget->addTab(mIrcWidget, i18n("IRC"));
    mTabWidget->addTab(mWebDavSettingsWidget, i18n("Webdav"));
    mTabWidget->addTab(mLDapSettingsWidget, i18n("LDAP"));
    mTabWidget->addTab(mLayoutSettingsWidget, i18n("Layout"));
    mTabWidget->addTab(mEnterpriseSettingsWidget, i18n("Enterprise"));
    mTabWidget->addTab(mUserDataDownloadWidget, i18n("User Data Download"));
    mTabWidget->addTab(mSlackBridgeWidget, i18n("Slack Bridge"));
    const int logPage = mTabWidget->addTab(mLogsSettingsWidget, i18n("Logs"));
    if (mRocketChatAccount && mRocketChatAccount->hasAtLeastVersion(8, 0, 0)) {
        // Hide it
        mTabWidget->setTabVisible(logPage, false);
    }
    mTabWidget->addTab(mEmailSettingsWidget, i18n("Email"));
    mTabWidget->addTab(mMobileSettingsWidget, i18n("Mobile"));
    mTabWidget->addTab(mTroubleshootSettingsWidget, i18n("Troubleshoot"));
    mTabWidget->addTab(mConferenceCallSettingsWidget, i18n("Conference Call"));
    mTabWidget->addTab(mWebRtcSettingsWidget, i18n("WebRTC"));
    mTabWidget->addTab(mCasSettingsWidget, i18n("CAS"));
    mTabWidget->addTab(mOauthSettingsWidget, i18n("Oauth"));
    mTabWidget->addTab(mAnalyticsWidget, i18n("Analytics"));
    if (mRocketChatAccount) {
        // connect(mRocketChatAccount, &RocketChatAccount::publicSettingLoaded, this, &AdministratorSettingsWidget::initialize);
        connect(mRocketChatAccount, &RocketChatAccount::privateSettingLoaded, this, [this](const QJsonObject &obj) {
            // qDebug() << " RocketChatAccount::privateSettingLoaded " << obj;
            initialize(obj);
        });
    }
    updateState(false);
}

AdministratorSettingsWidget::~AdministratorSettingsWidget() = default;

void AdministratorSettingsWidget::loadSettings()
{
    // Load from database ?
    mRocketChatAccount->ddp()->loadPublicSettingsAdministrator();
    mRocketChatAccount->ddp()->loadPrivateSettingsAdministrator();
}

void AdministratorSettingsWidget::initialize(const QJsonObject &obj)
{
    QJsonArray configs = obj.value("result"_L1).toArray();
    // qDebug() << " obj XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXx" << obj;
    QMap<QString, SettingsWidgetBase::SettingsInfo> mapSettings;
    for (QJsonValueRef currentConfig : configs) {
        const QJsonObject currentConfObject = currentConfig.toObject();
        const QString id = currentConfObject["_id"_L1].toString();
        const QVariant value = currentConfObject["value"_L1].toVariant();
        const bool readonly = currentConfObject["readonly"_L1].toBool(false);
        const bool enterprise = currentConfObject["enterprise"_L1].toBool(false);
        const QJsonArray modulesArrays = currentConfObject["modules"_L1].toArray();
        QStringList modules;
        for (auto module : modulesArrays) {
            modules.append(module.toString());
            // qDebug() << "modules" << modules;
        }
        // qDebug() << "id  " << id << " value " << value << "readonly " << readonly << "modules: " <<modules ;
        const SettingsWidgetBase::SettingsInfo settingsInfo{
            .readOnly = readonly,
            .enterprise = enterprise,
            .modules = modules,
            .value = value,
        };

        mapSettings.insert(id, settingsInfo);
    }

    initializeValues(mAccountSettingsWidget, mapSettings);
    initializeValues(mEncryptionSettingsWidget, mapSettings);
    initializeValues(mMessageSettingsWidget, mapSettings);
    initializeValues(mUploadFileSettingsWidget, mapSettings);
    initializeValues(mRetentionPolicySettingsWidget, mapSettings);
    initializeValues(mGeneralSettingsWidget, mapSettings);
    initializeValues(mRateLimiterWidget, mapSettings);
    initializeValues(mPasswordSettingsWidget, mapSettings);
    initializeValues(mVideoConferenceWidget, mapSettings);
    initializeValues(mIrcFederationWidget, mapSettings);
    initializeValues(mWebDavSettingsWidget, mapSettings);
    initializeValues(mLDapSettingsWidget, mapSettings);
    initializeValues(mLayoutSettingsWidget, mapSettings);
    initializeValues(mEnterpriseSettingsWidget, mapSettings);
    initializeValues(mUserDataDownloadWidget, mapSettings);
    initializeValues(mSlackBridgeWidget, mapSettings);
    if (mRocketChatAccount && !mRocketChatAccount->hasAtLeastVersion(8, 0, 0)) {
        initializeValues(mLogsSettingsWidget, mapSettings);
    }
    initializeValues(mEmailSettingsWidget, mapSettings);
    initializeValues(mMobileSettingsWidget, mapSettings);
    initializeValues(mTroubleshootSettingsWidget, mapSettings);
    initializeValues(mConferenceCallSettingsWidget, mapSettings);
    initializeValues(mWebRtcSettingsWidget, mapSettings);
    initializeValues(mCasSettingsWidget, mapSettings);
    initializeValues(mOauthSettingsWidget, mapSettings);
    initializeValues(mAnalyticsWidget, mapSettings);
    initializeValues(mIrcWidget, mapSettings);
    updateState(true);
}

void AdministratorSettingsWidget::initializeValues(SettingsWidgetBase *widget, const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    widget->initialize(mapSettings);
}

void AdministratorSettingsWidget::updateState(bool state)
{
    mAccountSettingsWidget->setEnabled(state);
    mEncryptionSettingsWidget->setEnabled(state);
    mMessageSettingsWidget->setEnabled(state);
    mUploadFileSettingsWidget->setEnabled(state);
    mRetentionPolicySettingsWidget->setEnabled(state);
    mRateLimiterWidget->setEnabled(state);
    mPasswordSettingsWidget->setEnabled(state);
    mVideoConferenceWidget->setEnabled(state);
    mIrcFederationWidget->setEnabled(state);
    mWebDavSettingsWidget->setEnabled(state);
    mLDapSettingsWidget->setEnabled(state);
    mLayoutSettingsWidget->setEnabled(state);
    mEnterpriseSettingsWidget->setEnabled(state);
    mUserDataDownloadWidget->setEnabled(state);
    mSlackBridgeWidget->setEnabled(state);
    mLogsSettingsWidget->setEnabled(state);
    mEmailSettingsWidget->setEnabled(state);
    mMobileSettingsWidget->setEnabled(state);
    mTroubleshootSettingsWidget->setEnabled(state);
    mConferenceCallSettingsWidget->setEnabled(state);
    mWebRtcSettingsWidget->setEnabled(state);
    mCasSettingsWidget->setEnabled(state);
    mOauthSettingsWidget->setEnabled(state);
    mAnalyticsWidget->setEnabled(state);
    mIrcWidget->setEnabled(state);
}

#include "moc_administratorsettingswidget.cpp"
