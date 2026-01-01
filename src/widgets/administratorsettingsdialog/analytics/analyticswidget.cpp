/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "analyticswidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>

AnalyticsWidget::AnalyticsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mMessages(new QCheckBox(i18nc("@option:check", "Messages"), this))
    , mRooms(new QCheckBox(i18nc("@option:check", "Rooms"), this))
    , mUsers(new QCheckBox(i18nc("@option:check", "Users"), this))
    , mGoogleEnabled(new QCheckBox(i18nc("@option:check", "Enable"), this))
    , mGoogleTrackingId(new QLineEdit(this))
    , mPiwikEnabled(new QCheckBox(i18nc("@option:check", "Enable"), this))
    , mPiwikUrl(new QLineEdit(this))
    , mPiwikClientID(new QLineEdit(this))
    , mAdditionalPiwikSites(new QPlainTextEdit(this))
    , mPrependDomain(new QCheckBox(i18nc("@option:check", "Prepend Domain"), this))
    , mAllSubdomains(new QCheckBox(i18nc("@option:check", "All Subdomains"), this))
    , mHideOutgoingLinks(new QPlainTextEdit(this))
{
    auto featuresEnabledLabel = createBoldLabel(i18n("Features Enabled"));
    featuresEnabledLabel->setObjectName(u"featuresEnabledLabel"_s);
    mMainLayout->addWidget(featuresEnabledLabel);

    mMessages->setObjectName(u"mMessages"_s);
    mMessages->setToolTip(i18nc("@info:tooltip", "Tracks custom events related to actions a user does on messages."));
    addCheckBox(mMessages, u"Analytics_features_messages"_s);

    mRooms->setObjectName(u"mRooms"_s);
    mRooms->setToolTip(i18nc("@info:tooltip", "Tracks custom events related to actions on a channel or group (create, leave, delete)."));
    addCheckBox(mRooms, u"Analytics_features_rooms"_s);

    mUsers->setObjectName(u"mUsers"_s);
    mUsers->setToolTip(i18nc("@info:tooltip", "Tracks custom events related to actions related to users (password reset times, profile picture change, etc)."));
    addCheckBox(mUsers, u"Analytics_features_users"_s);

    auto googleAnalyticsLabel = createBoldLabel(i18n("Google Analytics"));
    googleAnalyticsLabel->setObjectName(u"googleAnalyticsLabel"_s);
    mMainLayout->addWidget(googleAnalyticsLabel);

    mGoogleEnabled->setObjectName(u"mGoogleEnabled"_s);
    addCheckBox(mGoogleEnabled, u"GoogleAnalytics_enabled"_s);

    mGoogleTrackingId->setObjectName(u"mGoogleTrackingId"_s);
    addLineEdit(i18n("Tracking ID"), mGoogleTrackingId, u"GoogleAnalytics_ID"_s);

    auto piwikAnalyticsLabel = createBoldLabel(i18n("Piwik Analytics"));
    piwikAnalyticsLabel->setObjectName(u"piwikAnalyticsLabel"_s);
    mMainLayout->addWidget(piwikAnalyticsLabel);

    mPiwikEnabled->setObjectName(u"mPiwikEnabled"_s);
    addCheckBox(mPiwikEnabled, u"PiwikAnalytics_enabled"_s);

    mPiwikUrl->setObjectName(u"mPiwikUrl"_s);
    mPiwikUrl->setToolTip(i18nc("@info:tooltip", "The url where the Piwik resides, be sure to include the trailing slash. Example: //piwik.rocket.chat/"));
    addLineEdit(i18n("URL"), mPiwikUrl, u"PiwikAnalytics_url"_s);

    mPiwikClientID->setObjectName(u"mPiwikClientID"_s);
    mPiwikClientID->setToolTip(i18nc("@info:tooltip", "The site id to use for identifying this site. Example: 17"));
    addLineEdit(i18n("Client ID"), mPiwikClientID, u"PiwikAnalytics_siteId"_s);

    mAdditionalPiwikSites->setObjectName(u"mAdditionalPiwikSites"_s);
    addPlainTextEdit(i18n("Additional Piwik Sites"), mAdditionalPiwikSites, u"PiwikAdditionalTrackers"_s);

    mPrependDomain->setObjectName(u"mPrependDomain"_s);
    mPrependDomain->setToolTip(i18nc("@info:tooltip", "Prepend the site domain to the page title when tracking"));
    addCheckBox(mPrependDomain, u"PiwikAnalytics_prependDomain"_s);

    mAllSubdomains->setObjectName(u"mAllSubdomains"_s);
    mAllSubdomains->setToolTip(i18nc("@info:tooltip", "Track visitors across all subdomains"));
    addCheckBox(mAllSubdomains, u"PiwikAnalytics_cookieDomain"_s);

    mHideOutgoingLinks->setObjectName(u"mHideOutgoingLinks"_s);
    addPlainTextEdit(i18n("Hide Outgoing Links"), mHideOutgoingLinks, u"PiwikAnalytics_domains"_s);
}

AnalyticsWidget::~AnalyticsWidget() = default;

void AnalyticsWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
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

#include "moc_analyticswidget.cpp"
