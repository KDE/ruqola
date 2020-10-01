/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "directchannelinfowidget.h"
#include "user.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "restapirequest.h"
#include "ruqolawidgets_debug.h"
#include "users/userinfojob.h"

#include <QFormLayout>
#include <KLocalizedString>
#include <QLabel>
#include <QIcon>

DirectChannelInfoWidget::DirectChannelInfoWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mAvatar = new QLabel(this);
    mAvatar->setObjectName(QStringLiteral("mAvatar"));
    mainLayout->addWidget(mAvatar);

    mName = new QLabel(this);
    mName->setObjectName(QStringLiteral("mName"));
    mName->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mainLayout->addRow(i18n("Name:"), mName);

    mUserName = new QLabel(this);
    mUserName->setObjectName(QStringLiteral("mUserName"));
    mUserName->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mainLayout->addRow(i18n("UserName:"), mUserName);

    mStatus = new QLabel(this);
    mStatus->setObjectName(QStringLiteral("mStatus"));
    mStatus->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mainLayout->addRow(i18n("Status:"), mStatus);

    mTimeZone = new QLabel(this);
    mTimeZone->setObjectName(QStringLiteral("mTimeZone"));
    mTimeZone->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mainLayout->addRow(i18n("TimeZone:"), mTimeZone);

    mCustomStatus = new QLabel(this);
    mCustomStatus->setObjectName(QStringLiteral("mCustomStatus"));
    mCustomStatus->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mainLayout->addRow(i18n("Custom Status:"), mCustomStatus);
}

DirectChannelInfoWidget::~DirectChannelInfoWidget()
{
}

void DirectChannelInfoWidget::setUserName(const QString &userName)
{
    fetchUserInfo(userName);
}

void DirectChannelInfoWidget::fetchUserInfo(const QString &userName)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    RocketChatRestApi::UserInfoJob *userJob = new RocketChatRestApi::UserInfoJob(this);
    rcAccount->restApi()->initializeRestApiJob(userJob);
    RocketChatRestApi::UserInfoJob::UserInfo info;
    info.userIdentifier = userName;
    info.userInfoType = RocketChatRestApi::UserInfoJob::UserInfoType::UserName;
    userJob->setUserInfo(info);
    connect(userJob, &RocketChatRestApi::UserInfoJob::userInfoDone,
            this, &DirectChannelInfoWidget::slotUserInfoDone);
    if (!userJob->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start UserInfoJob";
    }
}

void DirectChannelInfoWidget::slotUserInfoDone(const QJsonObject &obj)
{
    const QJsonObject userJson = obj.value(QStringLiteral("user")).toObject();
    User user;
    user.parseUserRestApi(userJson);
    if (user.isValid()) {
        setUser(user);
    } else {
        qCDebug(RUQOLAWIDGETS_LOG) << "Invalid user parsing" << obj;
    }
}

void DirectChannelInfoWidget::setUser(const User &user)
{
    mName->setText(user.name());
    mUserName->setText(user.userName());
    mCustomStatus->setText(user.statusText());
    mStatus->setText(user.status());
    mTimeZone->setText((user.utcOffset() >= 0 ? QStringLiteral("UTC+") : QStringLiteral("UTC")) + QString::number(user.utcOffset()));
    //Download avatar ?
    const QUrl iconUrlStr = QUrl(Ruqola::self()->rocketChatAccount()->avatarUrl(user.userName()));
    mAvatar->setPixmap(QIcon(iconUrlStr.toLocalFile()).pixmap(60, 60)); //TODO hardcoded ?
}
