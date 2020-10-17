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
    mMainLayout = new QFormLayout(this);
    mMainLayout->setObjectName(QStringLiteral("mainLayout"));
    mMainLayout->setContentsMargins({});

    mAvatar = new QLabel(this);
    mAvatar->setObjectName(QStringLiteral("mAvatar"));
    mMainLayout->addWidget(mAvatar);

    mName = new QLabel(this);
    mName->setObjectName(QStringLiteral("mName"));
    mName->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mMainLayout->addRow(i18n("Name:"), mName);

    mUserName = new QLabel(this);
    mUserName->setObjectName(QStringLiteral("mUserName"));
    mUserName->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mMainLayout->addRow(i18n("UserName:"), mUserName);

    mStatus = new QLabel(this);
    mStatus->setObjectName(QStringLiteral("mStatus"));
    mStatus->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mMainLayout->addRow(i18n("Status:"), mStatus);

    mTimeZone = new QLabel(this);
    mTimeZone->setObjectName(QStringLiteral("mTimeZone"));
    mTimeZone->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mMainLayout->addRow(i18n("TimeZone:"), mTimeZone);

    mCustomStatus = new QLabel(this);
    mCustomStatus->setObjectName(QStringLiteral("mCustomStatus"));
    mCustomStatus->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mMainLayout->addRow(i18n("Custom Status:"), mCustomStatus);

    mRoles = new QLabel(this);
    mRoles->setObjectName(QStringLiteral("mRoles"));
    mRoles->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mMainLayout->addRow(i18n("Roles:"), mRoles);

    mCreateAt = new QLabel(this);
    mCreateAt->setObjectName(QStringLiteral("mCreateAt"));
    mCreateAt->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mMainLayout->addRow(i18n("Created At:"), mCreateAt);

    mLastLogin = new QLabel(this);
    mLastLogin->setObjectName(QStringLiteral("mLastLogin"));
    mLastLogin->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mMainLayout->addRow(i18n("Last Login:"), mLastLogin);

    mEmailsInfo = new QLabel(this);
    mEmailsInfo->setObjectName(QStringLiteral("mEmailsInfo"));
    mEmailsInfo->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mMainLayout->addRow(i18n("Email:"), mEmailsInfo);
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
    if (user.statusText().isEmpty()) {
        mCustomStatus->setVisible(false);
        mMainLayout->labelForField(mCustomStatus)->setVisible(false);
    } else {
        mCustomStatus->setText(user.statusText());
    }
    mStatus->setText(user.status());
    mTimeZone->setText((user.utcOffset() >= 0 ? QStringLiteral("UTC+") : QStringLiteral("UTC")) + QString::number(user.utcOffset()));
    //Download avatar ?
    Utils::AvatarInfo info;
    info.avatarType = Utils::AvatarType::User;
    info.identifier = user.userName();
    const QUrl iconUrlStr = QUrl(Ruqola::self()->rocketChatAccount()->avatarUrl(info));
    mAvatar->setPixmap(QIcon(iconUrlStr.toLocalFile()).pixmap(60, 60)); //TODO hardcoded ?
    //TODO use i18n ?
    if (user.roles().isEmpty()) {
        hideWidget(mRoles);
    } else {
        mRoles->setText(user.roles().join(QStringLiteral(", ")));
    }

    if (user.createdAt().isValid()) {
        mCreateAt->setText(user.createdAt().date().toString());
    } else {
        hideWidget(mCreateAt);
    }

    if (user.lastLogin().isValid()) {
        mLastLogin->setText(user.lastLogin().date().toString());
    } else {
        hideWidget(mLastLogin);
    }

    if (user.userEmailsInfo().isValid()) {
        const QString info = i18n("%1 %2", user.userEmailsInfo().email, user.userEmailsInfo().verified ? i18n("Verified") : i18n("Not verified"));
        mEmailsInfo->setText(info);
    } else {
        hideWidget(mEmailsInfo);
    }

}


void DirectChannelInfoWidget::hideWidget(QLabel *label)
{
    label->setVisible(false);
    mMainLayout->labelForField(label)->setVisible(false);
}
