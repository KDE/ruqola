/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directchannelinfowidget.h"
#include "common/resizablepixmaplabel.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "room/usersinroommenu.h"
#include "ruqolawidgets_debug.h"

#include "users/userinfojob.h"

#include <KLocalizedString>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>

using namespace Qt::Literals::StringLiterals;
DirectChannelInfoWidget::DirectChannelInfoWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mAvatar(new ResizablePixmapLabel(this))
    , mFormLayout(new QFormLayout)
    , mDirectChannelActionWidget(new DirectChannelActionWidget(account, this))
    , mRocketChatAccount(account)
{
    mFormLayout->setObjectName(u"mFormLayout"_s);
    mFormLayout->setContentsMargins({});

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mAvatar->setObjectName(u"mAvatar"_s);
    mainLayout->addWidget(mAvatar);
    mainLayout->addLayout(mFormLayout);

    mDirectChannelActionWidget->setObjectName(u"mDirectChannelActionWidget"_s);
    mainLayout->addWidget(mDirectChannelActionWidget);
    connect(mDirectChannelActionWidget, &DirectChannelActionWidget::ignoreUser, this, &DirectChannelInfoWidget::slotIgnoreUser);
    connect(mDirectChannelActionWidget, &DirectChannelActionWidget::reportUser, this, &DirectChannelInfoWidget::slotReportUser);
}

DirectChannelInfoWidget::~DirectChannelInfoWidget() = default;

void DirectChannelInfoWidget::setDirectChannelInfo(const DirectChannelInfo &info)
{
    mListRoleInfos = info.roles;
    fetchUserInfo(info.userName);
    mDirectChannelActionWidget->setRoom(info.room);
}

void DirectChannelInfoWidget::slotIgnoreUser()
{
    // TODO
}

void DirectChannelInfoWidget::slotReportUser()
{
    // TODO
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
    const QJsonObject userJson = obj.value("user"_L1).toObject();
    // qDebug() << " userJson " << userJson;
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
        mAvatar->setCurrentPixmap(QPixmap(iconUrlStr.toLocalFile()));
    }

    const QString name = user.name();
    QFont f = font();
    f.setBold(true);
    if (!name.isEmpty()) {
        auto nameLabel = new QLabel(this);

        nameLabel->setText(name);
        nameLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
        auto label = new QLabel(i18n("Name:"), this);
        label->setFont(f);
        mFormLayout->addRow(label, nameLabel);
    }

    const QString userName = user.userName();
    if (!userName.isEmpty()) {
        auto userNameLabel = new QLabel(userName, this);
        userNameLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
        auto label = new QLabel(i18n("Username:"), this);
        label->setFont(f);
        mFormLayout->addRow(label, userNameLabel);
    }

    auto statusLabel = new QLabel(Utils::displaytextFromPresenceStatus(user.status()), this);
    statusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    auto label = new QLabel(i18n("Status:"), this);
    label->setFont(f);
    mFormLayout->addRow(label, statusLabel);

    const QString statusText = user.statusText();
    if (!statusText.isEmpty()) {
        auto customStatusLabel = new QLabel(statusText, this);
        customStatusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
        auto customStatuslabel = new QLabel(i18n("Custom Status:"), this);
        customStatuslabel->setFont(f);
        mFormLayout->addRow(customStatuslabel, customStatusLabel);
    }

    if (!user.bio().isEmpty()) {
        auto bioUserLabel = new QLabel(user.bio(), this);
        bioUserLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
        auto bioLabel = new QLabel(i18n("Bio:"), this);
        bioLabel->setFont(f);
        mFormLayout->addRow(bioLabel, bioUserLabel);
    }

    auto timeZoneLabel = new QLabel(this);
    QString urlStr = u"UTC"_s;
    if (user.utcOffset() > 0) {
        urlStr += u'+' + QString::number(user.utcOffset());
    } else if (user.utcOffset() < 0) {
        urlStr += QString::number(user.utcOffset());
    }
    timeZoneLabel->setText(urlStr);
    timeZoneLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    label = new QLabel(i18n("Timezone:"), this);
    label->setFont(f);
    mFormLayout->addRow(label, timeZoneLabel);

    QDateTime dt = QDateTime::currentDateTimeUtc();
    constexpr int hours = 60 * 60;
    dt = dt.addSecs(hours * user.utcOffset());

    auto localTimeLabel = new QLabel(this);
    localTimeLabel->setText(dt.time().toString());
    label = new QLabel(i18n("Local Time:"), this);
    label->setFont(f);
    mFormLayout->addRow(label, localTimeLabel);

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
        rolesLabel->setText(newRolesList.join(u", "_s));
        rolesLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
        label = new QLabel(i18n("Roles:"), this);
        label->setFont(f);

        mFormLayout->addRow(label, rolesLabel);
    }

    const auto createdAt = user.createdAt();
    if (createdAt.isValid()) {
        auto createAtLabel = new QLabel(createdAt.date().toString(), this);
        createAtLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
        label = new QLabel(i18n("Created At:"), this);
        label->setFont(f);
        mFormLayout->addRow(label, createAtLabel);
    }

    const auto lastLogin = user.lastLogin();
    if (lastLogin.isValid()) {
        auto lastLoginLabel = new QLabel(lastLogin.toString(), this);
        lastLoginLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
        label = new QLabel(i18n("Last Login:"), this);
        label->setFont(f);
        mFormLayout->addRow(label, lastLoginLabel);
    }

    const auto userEmailsInfo = user.userEmailsInfo();
    if (userEmailsInfo.isValid()) {
        const QString generateEmail = u"<a href=\'mailto:%1\'>%1</a>"_s.arg(user.userEmailsInfo().email);
        const QString infoStr = i18n("%1 [%2]", generateEmail, user.userEmailsInfo().verified ? i18n("Verified") : i18n("Not verified"));
        auto emailsInfoLabel = new QLabel(infoStr, this);
        emailsInfoLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
        emailsInfoLabel->setTextFormat(Qt::RichText);
        emailsInfoLabel->setOpenExternalLinks(true);
        label = new QLabel(i18n("Email:"), this);
        label->setFont(f);
        mFormLayout->addRow(label, emailsInfoLabel);
    }
    // mDirectChannelActionWidget->setVisible(user.userId() != mRocketChatAccount->userId());
    mDirectChannelActionWidget->setVisible(false); // TODO
    mDirectChannelActionWidget->generateRoomMenu(user);
}

DirectChannelActionWidget::DirectChannelActionWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mToolButton(new QToolButton(this))
    , mRocketChatAccount(account)
{
    auto actionLayout = new QHBoxLayout(this);
    actionLayout->setObjectName(u"mainLayout"_s);
    actionLayout->setContentsMargins({});

    auto ignoreButton = new QPushButton(i18nc("@action:button", "Ignore"), this);
    ignoreButton->setObjectName(u"ignoreButton"_s);
    actionLayout->addWidget(ignoreButton);
    connect(ignoreButton, &QPushButton::clicked, this, &DirectChannelActionWidget::ignoreUser);

    auto reportButton = new QPushButton(i18nc("@action:button", "Report"), this);
    reportButton->setObjectName(u"reportButton"_s);
    actionLayout->addWidget(reportButton);
    connect(reportButton, &QPushButton::clicked, this, &DirectChannelActionWidget::reportUser);

    mToolButton->setObjectName(u"mToolButton"_s);
    mToolButton->setPopupMode(QToolButton::InstantPopup);
}

DirectChannelActionWidget::~DirectChannelActionWidget() = default;

void DirectChannelActionWidget::setRoom(Room *room)
{
    mRoom = room;
}

void DirectChannelActionWidget::generateRoomMenu(const User &user)
{
    auto menu = new UsersInRoomMenu(mRocketChatAccount, this);
    menu->setParentWidget(this);
    menu->setUserId(user.userId());
    menu->setUserName(user.userName());
    menu->setRoom(mRoom);
    mToolButton->setMenu(menu->createMenu());
}

#include "moc_directchannelinfowidget.cpp"
