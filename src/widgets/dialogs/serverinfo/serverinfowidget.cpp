/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serverinfowidget.h"
using namespace Qt::Literals::StringLiterals;

#include "rocketchataccount.h"
#include "ruqolaserverconfig.h"
#include "serverconfiginfo.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QLabel>

ServerInfoWidget::ServerInfoWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mAccountName(new QLabel(this))
    , mUserName(new QLabel(this))
    , mServerVersion(new QLabel(this))
    , mServerUrl(new QLabel(this))
    , mLogo(new QLabel(this))
    , mRocketChatAccount(account)
{
    auto layout = new QFormLayout(this);
    layout->setObjectName(u"layout"_s);
    layout->setContentsMargins({});

    mLogo->setObjectName(u"mLogo"_s);
    mLogo->hide(); // Hide by default
    layout->addWidget(mLogo);

    mAccountName->setObjectName(u"mAccountName"_s);
    mAccountName->setTextInteractionFlags(Qt::TextSelectableByMouse);
    mAccountName->setTextFormat(Qt::PlainText);
    layout->addRow(i18n("Account name:"), mAccountName);

    mUserName->setObjectName(u"mUserName"_s);
    mUserName->setTextInteractionFlags(Qt::TextSelectableByMouse);
    mUserName->setTextFormat(Qt::PlainText);
    layout->addRow(i18n("Username:"), mUserName);

    mServerUrl->setObjectName(u"mServerUrl"_s);
    mServerUrl->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
    mServerUrl->setTextFormat(Qt::RichText);
    layout->addRow(i18n("Server URL:"), mServerUrl);

    mServerVersion->setObjectName(u"mServerVersion"_s);
    mServerVersion->setTextInteractionFlags(Qt::TextSelectableByMouse);
    mServerVersion->setTextFormat(Qt::PlainText);
    layout->addRow(i18n("Server version:"), mServerVersion);

    if (mRocketChatAccount) {
        mEnterpriseLicense = new QLabel(this);
        mEnterpriseLicense->setObjectName(u"mEnterpriseLicense"_s);
        mEnterpriseLicense->setTextInteractionFlags(Qt::TextSelectableByMouse);
        mEnterpriseLicense->setTextFormat(Qt::PlainText);
        layout->addRow(i18n("License:"), mEnterpriseLicense);
        mEnterpriseLicense->setText(mRocketChatAccount->ruqolaServerConfig()->hasEnterpriseSupport() ? i18n("Enterprise") : i18nc("No license", "None"));
        setServerConfigInfo(mRocketChatAccount->serverConfigInfo());
    }
}

ServerInfoWidget::~ServerInfoWidget() = default;

void ServerInfoWidget::setServerConfigInfo(ServerConfigInfo *info)
{
    if (info) {
        mAccountName->setText(info->accountName());
        mUserName->setText(info->userName());
        mServerVersion->setText(info->serverVersionStr());
        mServerUrl->setText(u"<a href=\"%1\">%1</a>"_s.arg(info->serverUrl()));
        const QString logoLocalUrl{mRocketChatAccount->attachmentUrlFromLocalCache(info->logoUrl()).toLocalFile()};
        if (!logoLocalUrl.isEmpty()) {
            const QPixmap pix{logoLocalUrl};
            if (!pix.isNull()) {
                mLogo->show();
                const QPixmap scaledPixmap = pix.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                mLogo->setPixmap(scaledPixmap);
            }
        }
    }
}

#include "moc_serverinfowidget.cpp"
