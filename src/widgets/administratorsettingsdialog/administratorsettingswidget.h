/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>

class RocketChatAccount;
class QTabWidget;
class AccountSettingsWidget;
class EncryptionSettingsWidget;
class MessageSettingsWidget;
class FileUploadSettingsWidget;
class RetentionPolicySettingsWidget;
class GeneralSettingsWidget;
class RateLimiterWidget;
class PasswordSettingsWidget;
class VideoConferenceWidget;
class IrcFederationWidget;
class WebDavSettingsWidget;
class LDapSettingsWidget;
class LayoutSettingsWidget;
class EnterpriseSettingsWidget;
class SettingsWidgetBase;
class UserDataDownloadWidget;
class SlackBridgeWidget;
class LogsSettingsWidget;
class EmailSettingsWidget;
class MobileSettingsWidget;
class TroubleshootSettingsWidget;
class ConferenceCallSettingsWidget;
class WebRtcSettingsWidget;
class CasSettingsWidget;
class OauthSettingsWidget;
class AnalyticsWidget;
class IrcWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdministratorSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorSettingsWidget() override;

    void loadSettings();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void initialize(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void updateState(bool state);
    LIBRUQOLAWIDGETS_NO_EXPORT void initializeValues(SettingsWidgetBase *widget, const QMap<QString, QVariant> &mapSettings);
    QTabWidget *const mTabWidget;
    AccountSettingsWidget *const mAccountSettingsWidget;
    EncryptionSettingsWidget *const mEncryptionSettingsWidget;
    MessageSettingsWidget *const mMessageSettingsWidget;
    FileUploadSettingsWidget *const mUploadFileSettingsWidget;
    RetentionPolicySettingsWidget *const mRetentionPolicySettingsWidget;
    GeneralSettingsWidget *const mGeneralSettingsWidget;
    RateLimiterWidget *const mRateLimiterWidget;
    PasswordSettingsWidget *const mPasswordSettingsWidget;
    VideoConferenceWidget *const mVideoConferenceWidget;
    IrcFederationWidget *const mIrcFederationWidget;
    WebDavSettingsWidget *const mWebDavSettingsWidget;
    LDapSettingsWidget *const mLDapSettingsWidget;
    LayoutSettingsWidget *const mLayoutSettingsWidget;
    EnterpriseSettingsWidget *const mEnterpriseSettingsWidget;
    UserDataDownloadWidget *const mUserDataDownloadWidget;
    SlackBridgeWidget *const mSlackBridgeWidget;
    LogsSettingsWidget *const mLogsSettingsWidget;
    EmailSettingsWidget *const mEmailSettingsWidget;
    MobileSettingsWidget *const mMobileSettingsWidget;
    TroubleshootSettingsWidget *const mTroubleshootSettingsWidget;
    ConferenceCallSettingsWidget *const mConferenceCallSettingsWidget;
    WebRtcSettingsWidget *const mWebRtcSettingsWidget;
    CasSettingsWidget *const mCasSettingsWidget;
    OauthSettingsWidget *const mOauthSettingsWidget;
    AnalyticsWidget *const mAnalyticsWidget;
    IrcWidget *const mIrcWidget;
    RocketChatAccount *const mRocketChatAccount;
};
