/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomheaderwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "actionbuttons/actionbuttonsmanager.h"
#include "actionbuttons/actionbuttonutil.h"
#include "actionbuttonsgenerator.h"
#include "channelactionpopupmenu.h"
#include "encryption/e2edisabledialog.h"
#include "misc/avatarcachemanager.h"
#include "room/plugins/plugintool.h"
#include "roomheaderlabel.h"
#include "teamnamelabel.h"
#include "toolspluginmanager.h"
#include <KLocalizedString>
#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>

RoomHeaderWidget::RoomHeaderWidget(QWidget *parent)
    : QWidget(parent)
    , mRoomName(new QLabel(this))
    , mTeamName(new TeamNameLabel(this))
    , mRoomHeaderLabel(new RoomHeaderLabel(this))
    , mDiscussionBackButton(new QToolButton(this))
    , mFavoriteButton(new QToolButton(this))
    , mRoomIcon(new QLabel(this))
    , mCallButton(new QToolButton(this))
    , mChannelInfoButton(new QToolButton(this))
    , mTeamChannelsButton(new QToolButton(this))
    , mListOfUsersButton(new QToolButton(this))
    , mSearchMessageButton(new QToolButton(this))
    , mEncryptedButton(new QToolButton(this))
    , mChannelActionButton(new QToolButton(this))
    , mAIActionButton(new QToolButton(this))
    , mAvatarCacheManager(new AvatarCacheManager(Utils::AvatarType::Room, this))
    , mActionButtonsGenerator(new ActionButtonsGenerator(this))
{
    QList<PluginTool *> plugins = ToolsPluginManager::self()->pluginsList();
    if (plugins.count() > 1) {
        std::sort(plugins.begin(), plugins.end(), [](PluginTool *left, PluginTool *right) {
            return left->order() < right->order();
        });
    }
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto headerLayout = new QHBoxLayout;
    headerLayout->setObjectName(u"headerLayout"_s);
    headerLayout->setContentsMargins({});
    mainLayout->addLayout(headerLayout);

    mFavoriteButton->setAutoRaise(true);
    mFavoriteButton->setObjectName(u"mFavoriteButton"_s);
    mFavoriteButton->setIcon(QIcon::fromTheme(u"favorite"_s));
    mFavoriteButton->setCheckable(true);
    mFavoriteButton->setToolTip(i18nc("@info:tooltip", "Favorite"));
#ifndef QT_NO_ACCESSIBILITY
    mFavoriteButton->setAccessibleName(i18n("Favorite"));
#endif
    headerLayout->addWidget(mFavoriteButton, 0, Qt::AlignTop);
    connect(mFavoriteButton, &QToolButton::clicked, this, &RoomHeaderWidget::favoriteChanged);

    mDiscussionBackButton->setAutoRaise(true);
    mDiscussionBackButton->setObjectName(u"mDiscussionBackButton"_s);
    mDiscussionBackButton->setIcon(QIcon::fromTheme(u"draw-arrow-back"_s));
    mDiscussionBackButton->setCheckable(false);
    headerLayout->addWidget(mDiscussionBackButton, 0, Qt::AlignTop);
    connect(mDiscussionBackButton, &QToolButton::clicked, this, &RoomHeaderWidget::goBackToRoom);
    mDiscussionBackButton->setVisible(false);

    mEncryptedButton->setAutoRaise(true);
    mEncryptedButton->setObjectName(u"mEncryptedButton"_s);
    mEncryptedButton->setIcon(QIcon::fromTheme(u"encrypted"_s));
    mEncryptedButton->setToolTip(i18nc("@info:tooltip", "E2E Enabled"));
#ifndef QT_NO_ACCESSIBILITY
    mEncryptedButton->setAccessibleName(i18n("Encryption"));
#endif

    mEncryptedButton->setCheckable(true);
    mEncryptedButton->setVisible(false);
    headerLayout->addWidget(mEncryptedButton, 0, Qt::AlignTop);
    connect(mEncryptedButton, &QToolButton::clicked, this, [this](bool checked) {
        if (!checked) {
            slotDisabledEncryption();
        } else {
            Q_EMIT encryptedChanged(checked);
        }
    });

    mRoomIcon->setMargin(1);
    headerLayout->addWidget(mRoomIcon, 0, Qt::AlignTop);

    auto infoLayout = new QVBoxLayout;
    infoLayout->setObjectName(u"infoLayout"_s);
    infoLayout->setContentsMargins({});
    infoLayout->setSpacing(0);
    headerLayout->addLayout(infoLayout);

    auto roomNameLayout = new QHBoxLayout;
    roomNameLayout->setObjectName(u"roomNameLayout"_s);
    roomNameLayout->setContentsMargins({});
    roomNameLayout->setSpacing(0);
    infoLayout->addLayout(roomNameLayout);

    mRoomName->setObjectName(u"mRoomName"_s);
    mRoomName->setTextInteractionFlags(Qt::TextBrowserInteraction);
    roomNameLayout->addWidget(mRoomName);
    mRoomName->setVisible(false);

    mTeamName->setObjectName(u"mTeamName"_s);
    roomNameLayout->addWidget(mTeamName);
    mTeamName->setVisible(false);
    roomNameLayout->addStretch(1);

    mRoomHeaderLabel->setObjectName(u"mRoomHeaderLabel"_s);
    infoLayout->addWidget(mRoomHeaderLabel);

    auto buttonLayout = new QHBoxLayout;
    buttonLayout->setObjectName(u"buttonLayout"_s);
    buttonLayout->setContentsMargins({});
    buttonLayout->setSpacing(0);
    headerLayout->addLayout(buttonLayout);

    mAIActionButton->setAutoRaise(true);
    mAIActionButton->setObjectName(u"mAIActionButton"_s);
    mAIActionButton->setToolTip(i18nc("@info:tooltip", "AI Actions"));
    mAIActionButton->setPopupMode(QToolButton::InstantPopup);
    mAIActionButton->setIcon(QIcon::fromTheme(u"irc-operator"_s));
    buttonLayout->addWidget(mAIActionButton, 0, Qt::AlignTop);
    // Disable by default
    mAIActionButton->hide();

    mCallButton->setAutoRaise(true);
    mCallButton->setObjectName(u"mCallButton"_s);
    mCallButton->setIcon(QIcon::fromTheme(u"call-start-symbolic"_s));
    mCallButton->setToolTip(i18nc("@info:tooltip", "Call"));
#ifndef QT_NO_ACCESSIBILITY
    mCallButton->setAccessibleName(i18n("Call"));
#endif

    buttonLayout->addWidget(mCallButton, 0, Qt::AlignTop);
    connect(mCallButton, &QToolButton::clicked, this, &RoomHeaderWidget::callRequested);

    mChannelInfoButton->setAutoRaise(true);
    mChannelInfoButton->setObjectName(u"mChannelInfoButton"_s);
    mChannelInfoButton->setIcon(QIcon::fromTheme(u"documentinfo"_s));
    mChannelInfoButton->setToolTip(i18nc("@info:tooltip", "Channel's Information"));
#ifndef QT_NO_ACCESSIBILITY
    mChannelInfoButton->setAccessibleName(i18n("Channel's Information"));
#endif

    buttonLayout->addWidget(mChannelInfoButton, 0, Qt::AlignTop);
    connect(mChannelInfoButton, &QToolButton::clicked, this, &RoomHeaderWidget::channelInfoRequested);

    mTeamChannelsButton->setAutoRaise(true);
    mTeamChannelsButton->setObjectName(u"mTeamChannelsButton"_s);
    mTeamChannelsButton->setIcon(QIcon::fromTheme(u"irc-channel-active"_s));
    mTeamChannelsButton->setToolTip(i18nc("@info:tooltip", "Team Channels"));
#ifndef QT_NO_ACCESSIBILITY
    mListOfUsersButton->setAccessibleName(i18n("Team Channels"));
#endif

    buttonLayout->addWidget(mTeamChannelsButton, 0, Qt::AlignTop);
    connect(mTeamChannelsButton, &QToolButton::clicked, this, &RoomHeaderWidget::teamChannelsRequested);

    mListOfUsersButton->setAutoRaise(true);
    mListOfUsersButton->setObjectName(u"mListOfUsersButton"_s);
    mListOfUsersButton->setIcon(QIcon::fromTheme(u"system-users"_s));
    mListOfUsersButton->setCheckable(true);
    mListOfUsersButton->setToolTip(i18nc("@info:tooltip", "Show List of Users"));
#ifndef QT_NO_ACCESSIBILITY
    mListOfUsersButton->setAccessibleName(i18n("Show List of Users"));
#endif

    buttonLayout->addWidget(mListOfUsersButton, 0, Qt::AlignTop);
    connect(mListOfUsersButton, &QToolButton::clicked, this, &RoomHeaderWidget::listOfUsersChanged);

    mSearchMessageButton->setAutoRaise(true);
    mSearchMessageButton->setObjectName(u"mSearchMessageButton"_s);
    mSearchMessageButton->setIcon(QIcon::fromTheme(u"edit-find"_s));
    mSearchMessageButton->setCheckable(false);
    mSearchMessageButton->setShortcut(Qt::CTRL | Qt::Key_F);
    mSearchMessageButton->setToolTip(i18nc("@info:tooltip", "Search Messages…"));
#ifndef QT_NO_ACCESSIBILITY
    mSearchMessageButton->setAccessibleName(i18n("Search Messages…"));
#endif
    buttonLayout->addWidget(mSearchMessageButton, 0, Qt::AlignTop);
    connect(mSearchMessageButton, &QToolButton::clicked, this, &RoomHeaderWidget::searchMessageRequested);
    for (PluginTool *plugin : plugins) {
        if (plugin->enabled()) {
            if (plugin->toolType() == PluginTool::ToolType::MessageViewHeaderToolBar) {
                auto pluginButton = new QToolButton(this);
                pluginButton->setAutoRaise(true);
                const QString desc = plugin->description();
                if (desc.isEmpty()) {
                    pluginButton->setText(desc);
                }
                pluginButton->setIcon(QIcon::fromTheme(plugin->iconName()));
                pluginButton->setToolTip(plugin->toolTip());
                auto interface = plugin->createInterface(this);
                mPluginToolInterface.append(interface);
                connect(interface, &PluginToolInterface::activateRequested, this, [this, interface]() {
                    const PluginToolInterface::PluginToolInfo info{
                        .roomId = roomId(),
                        .accountName = mCurrentRocketChatAccount->accountName(),
                        .tmid = {},
                        .msgId = {},
                    };
                    interface->setInfo(info);
                    interface->activateTool();
                });
                if (plugin->hasMenu()) {
                    pluginButton->setMenu(interface->menu(this));
                    pluginButton->setPopupMode(QToolButton::InstantPopup);
                } else {
                    connect(pluginButton, &QToolButton::clicked, interface, &PluginToolInterface::activateRequested);
                }
                buttonLayout->addWidget(pluginButton, 0, Qt::AlignTop);
            }
        }
    }

    mChannelActionButton->setAutoRaise(true);
    mChannelActionButton->setObjectName(u"mChannelAction"_s);
    mChannelActionButton->setPopupMode(QToolButton::InstantPopup);
    mChannelActionButton->setIcon(QIcon::fromTheme(u"irc-operator"_s));
    buttonLayout->addWidget(mChannelActionButton, 0, Qt::AlignTop);

    mChannelActionPopupMenu = new ChannelActionPopupMenu(mChannelActionButton);
    mChannelActionPopupMenu->setObjectName(u"mChannelActionPopupMenu"_s);
    mChannelActionButton->setMenu(mChannelActionPopupMenu->menu());
    connect(mChannelActionPopupMenu, &ChannelActionPopupMenu::actionRequested, this, &RoomHeaderWidget::actionRequested);
    connect(mChannelActionPopupMenu, &ChannelActionPopupMenu::uiInteractionRequested, this, &RoomHeaderWidget::uiInteractionRequested);
    connect(mTeamName, &TeamNameLabel::openTeam, this, &RoomHeaderWidget::openTeam);

    connect(mActionButtonsGenerator, &ActionButtonsGenerator::uiInteractionRequested, this, &RoomHeaderWidget::uiInteractionRequested);
}

RoomHeaderWidget::~RoomHeaderWidget()
{
    qDeleteAll(mPluginToolInterface);
}

void RoomHeaderWidget::setCallEnabled(bool b)
{
    mCallButton->setVisible(b);
}

void RoomHeaderWidget::setIsDirectGroup(bool state)
{
    mChannelInfoButton->setVisible(!state);
}

void RoomHeaderWidget::setTeamRoomInfo(const Room::TeamRoomInfo &info)
{
    mTeamName->setTeamRoomInfo(info);
    mTeamName->setVisible(!info.teamName.isEmpty());
}

void RoomHeaderWidget::setRoomName(const QString &name)
{
    mRoomName->setText(name);
    mRoomName->setVisible(!name.isEmpty());
}

QString RoomHeaderWidget::roomName() const
{
    return mRoomName->text();
}

void RoomHeaderWidget::setRoomAnnouncement(const QString &name)
{
    mRoomHeaderLabel->setRoomAnnouncement(name);
}

void RoomHeaderWidget::setRoomTopic(const QString &name)
{
    mRoomHeaderLabel->setRoomTopic(name);
}

void RoomHeaderWidget::setFavoriteStatus(bool b)
{
    mFavoriteButton->setChecked(b);
}

void RoomHeaderWidget::setEncypted(bool b)
{
    mEncryptedButton->setChecked(b);
    mEncryptedButton->setVisible(b);
}

void RoomHeaderWidget::setIsMainTeam(bool isMainTeam)
{
    mTeamChannelsButton->setVisible(isMainTeam);
    mChannelInfoButton->setToolTip(isMainTeam ? i18n("Team's Information") : i18n("Channel's Information"));
}

void RoomHeaderWidget::setIsDiscussion(bool isDiscussion)
{
    mFavoriteButton->setVisible(!isDiscussion);
    mDiscussionBackButton->setVisible(isDiscussion);
}

void RoomHeaderWidget::setRoom(Room *room)
{
    mRoom = room;
    mChannelActionPopupMenu->setRoom(room);
    const auto avatarInfo = room->avatarInfo();
    if (avatarInfo.isValid()) {
        const QPixmap pix = mAvatarCacheManager->makeRoundedAvatarPixmap(this, avatarInfo, mFavoriteButton->size().height() - 4);
        mRoomIcon->setPixmap(pix);
    } else {
        mRoomIcon->setPixmap({});
    }
}

void RoomHeaderWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    if (mCurrentRocketChatAccount) {
        disconnect(mCurrentRocketChatAccount->actionButtonsManager(),
                   &ActionButtonsManager::actionButtonsChanged,
                   this,
                   &RoomHeaderWidget::slotActionButtonChanged);
    }
    mCurrentRocketChatAccount = account;
    mChannelActionPopupMenu->setCurrentRocketChatAccount(account);
    mAvatarCacheManager->setCurrentRocketChatAccount(account);
    mActionButtonsGenerator->setCurrentRocketChatAccount(account);
    if (mCurrentRocketChatAccount) {
        connect(mCurrentRocketChatAccount->actionButtonsManager(),
                &ActionButtonsManager::actionButtonsChanged,
                this,
                &RoomHeaderWidget::slotActionButtonChanged);
    }
}

QByteArray RoomHeaderWidget::roomId() const
{
    return mRoom->roomId();
}

void RoomHeaderWidget::slotActionButtonChanged()
{
    if (mCurrentRocketChatAccount && mRoom) {
        ActionButton::FilterActionInfo filterInfo;
        filterInfo.buttonContext = ActionButton::ButtonContext::RoomAction;
        filterInfo.roomTypeFilter = ActionButtonUtil::convertRoomTypeToActionButtonRoomTypeFilter(mRoom);
        filterInfo.category = ActionButton::Category::AI;
        const QList<ActionButton> actionButtons = mCurrentRocketChatAccount->actionButtonsManager()->actionButtonsFromFilterActionInfo(filterInfo);
        mAIActionButton->setVisible(!actionButtons.isEmpty());
        const QByteArray roomId = mRoom->roomId();
        mActionButtonsGenerator->generateActionButtons(actionButtons, mAIActionButton->menu(), roomId);
    }
}

void RoomHeaderWidget::slotDisabledEncryption()
{
    E2eDisableDialog d(this);
    if (d.exec()) {
        Q_EMIT encryptedChanged(false);
    } else {
        mEncryptedButton->setChecked(true);
    }
}

#include "moc_roomheaderwidget.cpp"
