/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serverinfowidget.h"
#include "rocketchataccount.h"
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
{
    auto layout = new QFormLayout(this);
    layout->setObjectName(QStringLiteral("layout"));
    layout->setContentsMargins({});

    mAccountName->setObjectName(QStringLiteral("mAccountName"));
    mAccountName->setTextInteractionFlags(Qt::TextSelectableByMouse);
    mAccountName->setTextFormat(Qt::PlainText);
    layout->addRow(i18n("Account Name:"), mAccountName);

    mUserName->setObjectName(QStringLiteral("mUserName"));
    mUserName->setTextInteractionFlags(Qt::TextSelectableByMouse);
    mUserName->setTextFormat(Qt::PlainText);
    layout->addRow(i18n("User Name:"), mUserName);

    mServerUrl->setObjectName(QStringLiteral("mServerUrl"));
    mServerUrl->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
    mServerUrl->setTextFormat(Qt::RichText);
    mServerUrl->setOpenExternalLinks(true);
    layout->addRow(i18n("Server Url:"), mServerUrl);

    mServerVersion->setObjectName(QStringLiteral("mServerVersion"));
    mServerVersion->setTextInteractionFlags(Qt::TextSelectableByMouse);
    mServerVersion->setTextFormat(Qt::PlainText);
    layout->addRow(i18n("Server Version:"), mServerVersion);

    if (account) {
        if (account->ruqolaServerConfig()->hasAtLeastVersion(5, 0, 0)) {
            mEnterpriseLicense = new QLabel(this);
            mEnterpriseLicense->setObjectName(QStringLiteral("mEnterpriseLicense"));
            mEnterpriseLicense->setTextInteractionFlags(Qt::TextSelectableByMouse);
            mEnterpriseLicense->setTextFormat(Qt::PlainText);
            layout->addRow(i18n("License:"), mEnterpriseLicense);
            mEnterpriseLicense->setText(account->ruqolaServerConfig()->hasEnterpriseSupport() ? i18n("Enterprise") : i18n("None"));
        }
        setServerConfigInfo(account->serverConfigInfo());
    }
}

ServerInfoWidget::~ServerInfoWidget() = default;

void ServerInfoWidget::setServerConfigInfo(ServerConfigInfo *info)
{
    if (info) {
        mAccountName->setText(info->accountName());
        mUserName->setText(info->userName());
        mServerVersion->setText(info->serverVersionStr());
        mServerUrl->setText(QStringLiteral("<a href=\"%1\">%1</a>").arg(info->serverUrl()));
    }
}
