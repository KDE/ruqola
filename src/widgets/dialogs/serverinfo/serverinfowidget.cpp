/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

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
    , mLogo(new QLabel(this))
    , mRocketChatAccount(account)
{
    auto layout = new QFormLayout(this);
    layout->setObjectName(QStringLiteral("layout"));
    layout->setContentsMargins({});

    mLogo->setObjectName(QStringLiteral("mLogo"));
    mLogo->hide(); // Hide by default
    layout->addWidget(mLogo);

    mAccountName->setObjectName(QStringLiteral("mAccountName"));
    mAccountName->setTextInteractionFlags(Qt::TextSelectableByMouse);
    mAccountName->setTextFormat(Qt::PlainText);
    layout->addRow(i18n("Account name:"), mAccountName);

    mUserName->setObjectName(QStringLiteral("mUserName"));
    mUserName->setTextInteractionFlags(Qt::TextSelectableByMouse);
    mUserName->setTextFormat(Qt::PlainText);
    layout->addRow(i18n("Username:"), mUserName);

    mServerUrl->setObjectName(QStringLiteral("mServerUrl"));
    mServerUrl->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
    mServerUrl->setTextFormat(Qt::RichText);
    layout->addRow(i18n("Server URL:"), mServerUrl);

    mServerVersion->setObjectName(QStringLiteral("mServerVersion"));
    mServerVersion->setTextInteractionFlags(Qt::TextSelectableByMouse);
    mServerVersion->setTextFormat(Qt::PlainText);
    layout->addRow(i18n("Server version:"), mServerVersion);

    if (mRocketChatAccount) {
        if (mRocketChatAccount->ruqolaServerConfig()->hasAtLeastVersion(5, 0, 0)) {
            mEnterpriseLicense = new QLabel(this);
            mEnterpriseLicense->setObjectName(QStringLiteral("mEnterpriseLicense"));
            mEnterpriseLicense->setTextInteractionFlags(Qt::TextSelectableByMouse);
            mEnterpriseLicense->setTextFormat(Qt::PlainText);
            layout->addRow(i18n("License:"), mEnterpriseLicense);
            mEnterpriseLicense->setText(mRocketChatAccount->ruqolaServerConfig()->hasEnterpriseSupport() ? i18n("Enterprise") : i18n("None"));
        }
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
        mServerUrl->setText(QStringLiteral("<a href=\"%1\">%1</a>").arg(info->serverUrl()));
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
