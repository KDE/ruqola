/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directchannelinfowidget.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "user.h"
#include "users/userinfojob.h"

#include <KLocalizedString>
#include <QFormLayout>
#include <QIcon>
#include <QLabel>

DirectChannelInfoWidget::DirectChannelInfoWidget(RocketChatAccount *account, QWidget *parent)
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
    , mRocketChatAccount(account)
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

    mCustomStatus->setObjectName(QStringLiteral("mCustomStatus"));
    mCustomStatus->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mMainLayout->addRow(i18n("Custom Status:"), mCustomStatus);

    mTimeZone->setObjectName(QStringLiteral("mTimeZone"));
    mTimeZone->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mMainLayout->addRow(i18n("Timezone:"), mTimeZone);

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
    mEmailsInfo->setTextFormat(Qt::RichText);
    mEmailsInfo->setOpenExternalLinks(true);
    mMainLayout->addRow(i18n("Email:"), mEmailsInfo);
}

DirectChannelInfoWidget::~DirectChannelInfoWidget() = default;

void DirectChannelInfoWidget::setUserName(const QString &userName)
{
    fetchUserInfo(userName);
}

void DirectChannelInfoWidget::setRoles(const QVector<RoleInfo> &newRoles)
{
    mListRoleInfos = newRoles;
}

void DirectChannelInfoWidget::fetchUserInfo(const QString &userName)
{
    auto userJob = new RocketChatRestApi::UserInfoJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(userJob);
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
    user.parseUserRestApi(userJson, mListRoleInfos);
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
    const QUrl iconUrlStr = QUrl(mRocketChatAccount->avatarUrl(info));
    mAvatar->setPixmap(QIcon(iconUrlStr.toLocalFile()).pixmap(60, 60)); // TODO hardcoded ?

    const QStringList roles{user.roles()};
    if (roles.isEmpty()) {
        hideWidget(mRoles);
    } else {
        QStringList newRolesList;
        for (const QString &rolestr : roles) {
            for (const RoleInfo &roleInfo : std::as_const(mListRoleInfos)) {
                if (roleInfo.identifier() == rolestr) {
                    newRolesList.append(User::roleI18n(roleInfo.name(), mListRoleInfos));
                    break;
                }
            }
        }
        mRoles->setText(newRolesList.join(QStringLiteral(", ")));
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
        const QString generateEmail = QStringLiteral("<a href=\'mailto:%1\'>%1</a>").arg(user.userEmailsInfo().email);
        const QString infoStr = i18n("%1 [%2]", generateEmail, user.userEmailsInfo().verified ? i18n("Verified") : i18n("Not verified"));
        mEmailsInfo->setText(infoStr);
    } else {
        hideWidget(mEmailsInfo);
    }
}

void DirectChannelInfoWidget::hideWidget(QLabel *label)
{
    label->setVisible(false);
    mMainLayout->labelForField(label)->setVisible(false);
}
