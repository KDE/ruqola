/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "serverinfowidget.h"
#include "serverconfiginfo.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QLabel>

ServerInfoWidget::ServerInfoWidget(QWidget *parent)
    : QWidget(parent)
{
    auto layout = new QFormLayout(this);
    layout->setObjectName(QStringLiteral("layout"));
    layout->setContentsMargins({});

    mAccountName = new QLabel(this);
    mAccountName->setObjectName(QStringLiteral("mAccountName"));
    mAccountName->setTextInteractionFlags(Qt::TextSelectableByMouse);
    mAccountName->setTextFormat(Qt::PlainText);
    layout->addRow(i18n("Account Name:"), mAccountName);

    mUserName = new QLabel(this);
    mUserName->setObjectName(QStringLiteral("mUserName"));
    mUserName->setTextInteractionFlags(Qt::TextSelectableByMouse);
    mUserName->setTextFormat(Qt::PlainText);
    layout->addRow(i18n("User Name:"), mUserName);

    mServerUrl = new QLabel(this);
    mServerUrl->setObjectName(QStringLiteral("mServerUrl"));
    mServerUrl->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
    mServerUrl->setTextFormat(Qt::RichText);
    mServerUrl->setOpenExternalLinks(true);
    layout->addRow(i18n("Server Url:"), mServerUrl);

    mServerVersion = new QLabel(this);
    mServerVersion->setObjectName(QStringLiteral("mServerVersion"));
    mServerVersion->setTextInteractionFlags(Qt::TextSelectableByMouse);
    mServerVersion->setTextFormat(Qt::PlainText);
    layout->addRow(i18n("Server Version:"), mServerVersion);
}

ServerInfoWidget::~ServerInfoWidget()
{
}

void ServerInfoWidget::setServerConfigInfo(ServerConfigInfo *info)
{
    if (info) {
        mAccountName->setText(info->serverName());
        mUserName->setText(info->userName());
        mServerVersion->setText(info->serverVersionStr());
        mServerUrl->setText(QStringLiteral("<a href=\"%1\">%1</a>").arg(info->serverUrl()));
    }
}
