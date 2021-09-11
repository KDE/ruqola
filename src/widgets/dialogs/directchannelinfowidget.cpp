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

#include "directchannelinfowidget.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "user.h"
#include "users/userinfojob.h"

#include <KLocalizedString>
#include <QFormLayout>
#include <QIcon>
#include <QLabel>

DirectChannelInfoWidget::DirectChannelInfoWidget(QWidget *parent)
    : QWidget(parent)
    , mName(new QLabel(this))
    , mUserName(new QLabel(this))
    , mCustomStatus(new QLabel(this))
    , mStatus(new QLabel(this))
    , mTimeZone(new QLabel(this))
    , mAvatar(new QLabel(this))
    , mRoles(new QLabel(this))
    , mCreateAt(new QLabel(this))
    , mLastLogin(new QLabel(this))
    , mEmailsInfo(new QLabel(this))
    , mMainLayout(new QFormLayout(this))
{
    mMainLayout->setObjectName(QStringLiteral("mainLayout"));
    mMainLayout->setContentsMargins({});

    mAvatar->setObjectName(QStringLiteral("mAvatar"));
    mMainLayout->addWidget(mAvatar);

    mName->setObjectName(QStringLiteral("mName"));
    mName->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mMainLayout->addRow(i18n("Name:"), mName);

    mUserName->setObjectName(QStringLiteral("mUserName"));
    mUserName->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mMainLayout->addRow(i18n("Username:"), mUserName);

    mStatus->setObjectName(QStringLiteral("mStatus"));
    mStatus->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mMainLayout->addRow(i18n("Status:"), mStatus);

    mTimeZone->setObjectName(QStringLiteral("mTimeZone"));
    mTimeZone->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mMainLayout->addRow(i18n("Timezone:"), mTimeZone);

    mCustomStatus->setObjectName(QStringLiteral("mCustomStatus"));
    mCustomStatus->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mMainLayout->addRow(i18n("Custom Status:"), mCustomStatus);

    mRoles->setObjectName(QStringLiteral("mRoles"));
    mRoles->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mMainLayout->addRow(i18n("Roles:"), mRoles);

    mCreateAt->setObjectName(QStringLiteral("mCreateAt"));
    mCreateAt->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mMainLayout->addRow(i18n("Created At:"), mCreateAt);

    mLastLogin->setObjectName(QStringLiteral("mLastLogin"));
    mLastLogin->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mMainLayout->addRow(i18n("Last Login:"), mLastLogin);

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
    auto userJob = new RocketChatRestApi::UserInfoJob(this);
    rcAccount->restApi()->initializeRestApiJob(userJob);
    RocketChatRestApi::UserInfoJob::UserInfo info;
    info.userIdentifier = userName;
    info.userInfoType = RocketChatRestApi::UserInfoJob::UserInfoType::UserName;
    userJob->setUserInfo(info);
    connect(userJob, &RocketChatRestApi::UserInfoJob::userInfoDone, this, &DirectChannelInfoWidget::slotUserInfoDone);
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
    const QString name = user.name();
    if (name.isEmpty()) {
        hideWidget(mName);
    } else {
        mName->setText(name);
    }
    mUserName->setText(user.userName());
    if (user.statusText().isEmpty()) {
        hideWidget(mCustomStatus);
    } else {
        mCustomStatus->setText(user.statusText());
    }
    mStatus->setText(Utils::displaytextFromPresenceStatus(user.status()));
    mTimeZone->setText((user.utcOffset() >= 0 ? QStringLiteral("UTC+") : QStringLiteral("UTC")) + QString::number(user.utcOffset()));
    // Download avatar ?
    Utils::AvatarInfo info;
    info.avatarType = Utils::AvatarType::User;
    info.identifier = user.userName();
    const QUrl iconUrlStr = QUrl(Ruqola::self()->rocketChatAccount()->avatarUrl(info));
    mAvatar->setPixmap(QIcon(iconUrlStr.toLocalFile()).pixmap(60, 60)); // TODO hardcoded ?

    const QStringList i18nRoles{user.i18nRoles()};
    if (i18nRoles.isEmpty()) {
        hideWidget(mRoles);
    } else {
        mRoles->setText(i18nRoles.join(QStringLiteral(", ")));
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
        const QString info = i18n("%1 [%2]", user.userEmailsInfo().email, user.userEmailsInfo().verified ? i18n("Verified") : i18n("Not verified"));
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
