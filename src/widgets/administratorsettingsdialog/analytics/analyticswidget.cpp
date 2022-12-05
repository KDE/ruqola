/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "analyticswidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>

AnalyticsWidget::AnalyticsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mMessages(new QCheckBox(i18n("Messages"), this))
    , mRooms(new QCheckBox(i18n("Rooms"), this))
    , mUsers(new QCheckBox(i18n("Users"), this))
    , mGoogleEnabled(new QCheckBox(i18n("Enable"), this))
    , mPiwikEnabled(new QCheckBox(i18n("Enable"), this))
    , mGoogleTrackingId(new QLineEdit(this))
    , mPiwikUrl(new QLineEdit(this))
    , mPiwikClientID(new QLineEdit(this))
    , mPrependDomain(new QCheckBox(i18n("Prepend Domain"), this))
    , mAllSubdomains(new QCheckBox(i18n("All Subdomains"), this))
    , mAdditionalPiwikSites(new QPlainTextEdit(this))
    , mHideOutgoingLinks(new QPlainTextEdit(this))
{
    auto featuresEnabledLabel = createBoldLabel(i18n("Features Enabled"));
    featuresEnabledLabel->setObjectName(QStringLiteral("featuresEnabledLabel"));
    mMainLayout->addWidget(featuresEnabledLabel);

    mMessages->setObjectName(QStringLiteral("mMessages"));
    mMainLayout->addWidget(mMessages);
    mMessages->setToolTip(i18n("Tracks custom events related to actions a user does on messages."));
    connectCheckBox(mMessages, QStringLiteral("Analytics_features_messages"));

    mRooms->setObjectName(QStringLiteral("mRooms"));
    mMainLayout->addWidget(mRooms);
    mRooms->setToolTip(i18n("Tracks custom events related to actions on a channel or group (create, leave, delete)."));
    connectCheckBox(mRooms, QStringLiteral("Analytics_features_rooms"));

    mUsers->setObjectName(QStringLiteral("mUsers"));
    mMainLayout->addWidget(mUsers);
    mUsers->setToolTip(i18n("Tracks custom events related to actions related to users (password reset times, profile picture change, etc)."));
    connectCheckBox(mUsers, QStringLiteral("Analytics_features_users"));

    auto googleAnalyticsLabel = createBoldLabel(i18n("Google Analytics"));
    googleAnalyticsLabel->setObjectName(QStringLiteral("googleAnalyticsLabel"));
    mMainLayout->addWidget(googleAnalyticsLabel);

    mGoogleEnabled->setObjectName(QStringLiteral("mGoogleEnabled"));
    mMainLayout->addWidget(mGoogleEnabled);
    connectCheckBox(mGoogleEnabled, QStringLiteral("GoogleAnalytics_enabled"));

    mGoogleTrackingId->setObjectName(QStringLiteral("mGoogleTrackingId"));
    addLineEdit(i18n("Tracking ID"), mGoogleTrackingId, QStringLiteral("GoogleAnalytics_ID"));

    auto piwikAnalyticsLabel = createBoldLabel(i18n("Piwik Analytics"));
    piwikAnalyticsLabel->setObjectName(QStringLiteral("piwikAnalyticsLabel"));
    mMainLayout->addWidget(piwikAnalyticsLabel);

    mPiwikEnabled->setObjectName(QStringLiteral("mPiwikEnabled"));
    mMainLayout->addWidget(mPiwikEnabled);
    connectCheckBox(mPiwikEnabled, QStringLiteral("PiwikAnalytics_enabled"));

    mPiwikUrl->setObjectName(QStringLiteral("mPiwikUrl"));
    mPiwikUrl->setToolTip(i18n("The url where the Piwik resides, be sure to include the trailing slash. Example: //piwik.rocket.chat/"));
    addLineEdit(i18n("URL"), mPiwikUrl, QStringLiteral("PiwikAnalytics_url"));

    mPiwikClientID->setObjectName(QStringLiteral("mPiwikClientID"));
    mPiwikClientID->setToolTip(i18n("The site id to use for identifying this site. Example: 17"));
    addLineEdit(i18n("Client ID"), mPiwikClientID, QStringLiteral("PiwikAnalytics_siteId"));

    mAdditionalPiwikSites->setObjectName(QStringLiteral("mAdditionalPiwikSites"));
    addPlainTextEdit(i18n("Additional Piwik Sites"), mAdditionalPiwikSites, QStringLiteral("PiwikAdditionalTrackers"));

    mPrependDomain->setObjectName(QStringLiteral("mPrependDomain"));
    mPrependDomain->setToolTip(i18n("Prepend the site domain to the page title when tracking"));
    mMainLayout->addWidget(mPrependDomain);
    connectCheckBox(mPrependDomain, QStringLiteral("PiwikAnalytics_prependDomain"));

    mAllSubdomains->setObjectName(QStringLiteral("mAllSubdomains"));
    mAllSubdomains->setToolTip(i18n("Track visitors across all subdomains"));
    mMainLayout->addWidget(mAllSubdomains);
    connectCheckBox(mAllSubdomains, QStringLiteral("PiwikAnalytics_cookieDomain"));

    mHideOutgoingLinks->setObjectName(QStringLiteral("mHideOutgoingLinks"));
    addPlainTextEdit(i18n("Hide Outgoing Links"), mHideOutgoingLinks, QStringLiteral("PiwikAnalytics_domains"));
}

AnalyticsWidget::~AnalyticsWidget() = default;

void AnalyticsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mMessages, mapSettings, true);
    initializeWidget(mRooms, mapSettings, true);
    initializeWidget(mUsers, mapSettings, true);
    initializeWidget(mGoogleEnabled, mapSettings, false);
    initializeWidget(mGoogleTrackingId, mapSettings, {});
    initializeWidget(mPiwikEnabled, mapSettings, false);
    initializeWidget(mPiwikUrl, mapSettings, {});
    initializeWidget(mPiwikClientID, mapSettings, {});
    initializeWidget(mPrependDomain, mapSettings, false);
    initializeWidget(mAllSubdomains, mapSettings, false);
    initializeWidget(mAdditionalPiwikSites, mapSettings, {});
    initializeWidget(mHideOutgoingLinks, mapSettings, {});
}
