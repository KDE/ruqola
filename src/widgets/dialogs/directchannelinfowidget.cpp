/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

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
#include <QScreen>

DirectChannelInfoWidget::DirectChannelInfoWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mAvatar(new QLabel(this))
    , mMainLayout(new QFormLayout(this))
    , mRocketChatAccount(account)
{
    mMainLayout->setObjectName(QStringLiteral("mainLayout"));
    mMainLayout->setContentsMargins({});

    mAvatar->setObjectName(QStringLiteral("mAvatar"));
    mMainLayout->addWidget(mAvatar);
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
    // Download avatar ?
    Utils::AvatarInfo info;
    info.avatarType = Utils::AvatarType::User;
    info.identifier = user.userName();
    const QUrl iconUrlStr = QUrl(mRocketChatAccount->avatarUrl(info));
    if (!iconUrlStr.isEmpty()) {
        const QSize pixmapAvatarSize = QSize(80, 80) * screen()->devicePixelRatio();
        mAvatar->setPixmap(QIcon(iconUrlStr.toLocalFile()).pixmap(pixmapAvatarSize));
    }

    const QString name = user.name();
    if (!name.isEmpty()) {
        auto nameLabel = new QLabel(this);
        nameLabel->setText(name);
        nameLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
        mMainLayout->addRow(i18n("Name:"), nameLabel);
    }

    const QString userName = user.userName();
    if (!userName.isEmpty()) {
        auto userNameLabel = new QLabel(userName, this);
        userNameLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
        mMainLayout->addRow(i18n("Username:"), userNameLabel);
    }

    auto statusLabel = new QLabel(Utils::displaytextFromPresenceStatus(user.status()), this);
    statusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mMainLayout->addRow(i18n("Status:"), statusLabel);

    const QString statusText = user.statusText();
    if (!statusText.isEmpty()) {
        auto customStatusLabel = new QLabel(statusText, this);
        customStatusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
        mMainLayout->addRow(i18n("Custom Status:"), customStatusLabel);
    }

    auto timeZoneLabel = new QLabel(this);
    timeZoneLabel->setText((user.utcOffset() >= 0 ? QStringLiteral("UTC+") : QStringLiteral("UTC")) + QString::number(user.utcOffset()));
    timeZoneLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mMainLayout->addRow(i18n("Timezone:"), timeZoneLabel);

    const QStringList roles{user.roles()};

    if (!roles.isEmpty()) {
        auto rolesLabel = new QLabel(this);
        QStringList newRolesList;
        for (const QString &rolestr : roles) {
            for (const RoleInfo &roleInfo : std::as_const(mListRoleInfos)) {
                if (roleInfo.identifier() == rolestr) {
                    newRolesList.append(User::roleI18n(roleInfo.name(), mListRoleInfos));
                    break;
                }
            }
        }
        rolesLabel->setText(newRolesList.join(QStringLiteral(", ")));
        rolesLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
        mMainLayout->addRow(i18n("Roles:"), rolesLabel);
    }

    const auto createdAt = user.createdAt();
    if (createdAt.isValid()) {
        auto createAtLabel = new QLabel(createdAt.date().toString(), this);
        createAtLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
        mMainLayout->addRow(i18n("Created At:"), createAtLabel);
    }

    const auto lastLogin = user.lastLogin();
    if (lastLogin.isValid()) {
        auto lastLoginLabel = new QLabel(lastLogin.toString(), this);
        lastLoginLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
        mMainLayout->addRow(i18n("Last Login:"), lastLoginLabel);
    }

    const auto userEmailsInfo = user.userEmailsInfo();
    if (userEmailsInfo.isValid()) {
        const QString generateEmail = QStringLiteral("<a href=\'mailto:%1\'>%1</a>").arg(user.userEmailsInfo().email);
        const QString infoStr = i18n("%1 [%2]", generateEmail, user.userEmailsInfo().verified ? i18n("Verified") : i18n("Not verified"));
        auto emailsInfoLabel = new QLabel(infoStr, this);
        emailsInfoLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
        emailsInfoLabel->setTextFormat(Qt::RichText);
        emailsInfoLabel->setOpenExternalLinks(true);
        mMainLayout->addRow(i18n("Email:"), emailsInfoLabel);
    }
}

#include "moc_directchannelinfowidget.cpp"
