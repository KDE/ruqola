/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelactionpopupmenu.h"
#include "actionbuttons/actionbuttonsmanager.h"
#include "actionbuttons/actionbuttonutil.h"
#include "autogenerateui/autogenerateinteractionutil.h"
#include "connection.h"
#include "misc/appsuiinteractionjob.h"
#include "rocketchataccount.h"
#include "room.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QMenu>

ChannelActionPopupMenu::ChannelActionPopupMenu(QObject *parent)
    : QObject(parent)
    , mMenu(new QMenu)
{
    mMenu->setObjectName(QStringLiteral("mMenu"));
    connect(mMenu, &QMenu::aboutToShow, this, &ChannelActionPopupMenu::slotUpdateMenu);
    createMenu();
}

ChannelActionPopupMenu::~ChannelActionPopupMenu()
{
    delete mMenu;
}

void ChannelActionPopupMenu::createMenu()
{
    mPruneMessages = new QAction(i18nc("@action", "Prune Messages…"), this);
    mMenu->addAction(mPruneMessages);
    connect(mPruneMessages, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::PruneMessages);
    });

    mPruneMessagesSeparator = mMenu->addSeparator();

    mShowMentions = new QAction(i18nc("@action", "Show Mentions…"), this);
    mMenu->addAction(mShowMentions);
    connect(mShowMentions, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowMentions);
    });

    mShowPinnedMessages = new QAction(i18nc("@action", "Show Pinned Messages…"), this);
    mMenu->addAction(mShowPinnedMessages);
    connect(mShowPinnedMessages, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowPinned);
    });

    mShowStarredMessages = new QAction(i18nc("@action", "Show Starred Messages…"), this);
    mMenu->addAction(mShowStarredMessages);
    connect(mShowStarredMessages, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowStarred);
    });

    mShowFileAttachments = new QAction(QIcon::fromTheme(QStringLiteral("download-symbolic")), i18n("Show File Attachments…"), this);
    mMenu->addAction(mShowFileAttachments);
    connect(mShowFileAttachments, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowAttachment);
    });

    mShowDiscussions = new QAction(i18nc("@action", "Show Discussions…"), this);
    mMenu->addAction(mShowDiscussions);
    connect(mShowDiscussions, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowDiscussions);
    });

    mShowThreads = new QAction(i18nc("@action", "Show Threads…"), this);
    mMenu->addAction(mShowThreads);
    connect(mShowThreads, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowThreads);
    });

    mMenu->addSeparator();

    mConfigureNotification = new QAction(QIcon::fromTheme(QStringLiteral("notifications-symbolic")), i18n("Configure Notification…"), this);
    mMenu->addAction(mConfigureNotification);
    connect(mConfigureNotification, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::Notification);
    });

    mAutoTranslateSeparator = mMenu->addSeparator();
    mAutoTranslate = new QAction(i18nc("@action", "Configure Auto-Translate…"), this);
    mMenu->addAction(mAutoTranslate);
    connect(mAutoTranslate, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::AutoTranslate);
    });

    mInviteUsersGenerateUrlSeparator = mMenu->addSeparator();
    mMenu->addAction(mInviteUsersGenerateUrlSeparator);
    mInviteUsersGenerateUrl = new QAction(i18nc("@action", "Invite Users"), this);
    mMenu->addAction(mInviteUsersGenerateUrl);
    connect(mInviteUsersGenerateUrl, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::InviteUsers);
    });

    mAddUserInRoomsSeparator = mMenu->addSeparator();
    mAddUserInRooms = new QAction(i18nc("@action", "Add Users in Channel…"), this);
    mMenu->addAction(mAddUserInRoomsSeparator);
    connect(mAddUserInRooms, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::AddUsersInRoom);
    });
    mMenu->addAction(mAddUserInRooms);

    mMenu->addSeparator();
    mStartVideoChat = new QAction(QIcon::fromTheme(QStringLiteral("camera-video")), i18n("Video Chat"), this);
    mMenu->addAction(mStartVideoChat);
    connect(mStartVideoChat, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::VideoChat);
    });

    mMenu->addSeparator();
    mExportMessages = new QAction(i18nc("@action", "Export Messages…"), this);
    mMenu->addAction(mExportMessages);
    connect(mExportMessages, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ExportMessages);
    });

    mMenu->addSeparator();
    mOffTheRecordMessages = new QAction(i18nc("@action", "OTR"), this);
    mOffTheRecordMessages->setCheckable(true);
    mMenu->addAction(mOffTheRecordMessages);
    connect(mOffTheRecordMessages, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::OtrMessages);
    });
    mMenu->addSeparator();
    mEncryptMessages = new QAction(i18nc("@action", "Encrypt Messages"), this);
    mEncryptMessages->setCheckable(true);
    mMenu->addAction(mEncryptMessages);
    connect(mEncryptMessages, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::EncryptMessages);
    });
}

QMenu *ChannelActionPopupMenu::menu() const
{
    return mMenu;
}

void ChannelActionPopupMenu::setRoom(Room *room)
{
    if (mRoom != room) {
        mRoom = room;
        slotActionButtonChanged();
    }
}

void ChannelActionPopupMenu::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    if (mCurrentRocketChatAccount) {
        disconnect(mCurrentRocketChatAccount->actionButtonsManager(),
                   &ActionButtonsManager::actionButtonsChanged,
                   this,
                   &ChannelActionPopupMenu::slotActionButtonChanged);
    }
    mCurrentRocketChatAccount = account;
    if (mCurrentRocketChatAccount) {
        connect(mCurrentRocketChatAccount->actionButtonsManager(),
                &ActionButtonsManager::actionButtonsChanged,
                this,
                &ChannelActionPopupMenu::slotActionButtonChanged);
    }
}

void ChannelActionPopupMenu::slotActionButtonChanged()
{
    // Check list of apps action
    qDeleteAll(mListActionButton);
    mListActionButton.clear();
    if (mCurrentRocketChatAccount && mRoom) {
        ActionButton::FilterActionInfo filterInfo;
        filterInfo.buttonContext = ActionButton::ButtonContext::RoomAction;
        filterInfo.roomTypeFilter = ActionButtonUtil::convertRoomTypeToActionButtonRoomTypeFilter(mRoom);
        const QList<ActionButton> actionButtons = mCurrentRocketChatAccount->actionButtonsManager()->actionButtonsFromFilterActionInfo(filterInfo);
        if (!actionButtons.isEmpty()) {
            const QString lang = QLocale().name();
            auto actSeparator = new QAction(this);
            actSeparator->setSeparator(true);
            mListActionButton.append(actSeparator);
            mMenu->addAction(actSeparator);
            for (const auto &actionButton : actionButtons) {
                auto act = new QAction(this);
                const QString translateIdentifier = ActionButtonUtil::generateTranslateIdentifier(actionButton);
                const QString appId = QString::fromLatin1(actionButton.appId());
                act->setText(mCurrentRocketChatAccount->getTranslatedIdentifier(lang, translateIdentifier));
                const QByteArray roomId = mRoom->roomId();
                connect(act, &QAction::triggered, this, [this, actionButton, appId, roomId]() {
                    auto job = new RocketChatRestApi::AppsUiInteractionJob(this);
                    RocketChatRestApi::AppsUiInteractionJob::AppsUiInteractionJobInfo info;
                    info.methodName = appId;
                    AutoGenerateInteractionUtil::ActionButtonInfo actionButtonInfo;
                    actionButtonInfo.actionId = actionButton.actionId();
                    actionButtonInfo.triggerId = QUuid::createUuid().toByteArray(QUuid::Id128);
                    actionButtonInfo.roomId = roomId;
                    info.messageObj = AutoGenerateInteractionUtil::createRoomActionButton(actionButtonInfo);
                    job->setAppsUiInteractionJobInfo(info);

                    mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
                    connect(job, &RocketChatRestApi::AppsUiInteractionJob::appsUiInteractionDone, this, [this](const QJsonObject &replyObject) {
                        Q_EMIT uiInteractionRequested(replyObject);
                    });
                    if (!job->start()) {
                        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start AppsUiInteractionJob job";
                    }
                });
                mListActionButton.append(act);
                mMenu->addAction(act);
            }
        }
    }
}

void ChannelActionPopupMenu::slotUpdateMenu()
{
    if (mCurrentRocketChatAccount) {
        mShowPinnedMessages->setVisible(mCurrentRocketChatAccount->ruqolaServerConfig()->allowMessagePinningEnabled());
        mShowStarredMessages->setVisible(mCurrentRocketChatAccount->ruqolaServerConfig()->allowMessageStarringEnabled());
        mAutoTranslate->setVisible(mCurrentRocketChatAccount->hasAutotranslateSupport());
        mAutoTranslateSeparator->setVisible(mCurrentRocketChatAccount->ruqolaServerConfig()->autoTranslateEnabled());

        const bool hasPermissionInviteUserSupport = mRoom && mRoom->hasPermission(QStringLiteral("create-invite-links"));
        mInviteUsersGenerateUrl->setVisible(hasPermissionInviteUserSupport);
        mInviteUsersGenerateUrlSeparator->setVisible(hasPermissionInviteUserSupport);
        mStartVideoChat->setVisible(mCurrentRocketChatAccount->ruqolaServerConfig()->jitsiEnabled());

        mAddUserInRoomsSeparator->setVisible(mRoom && mRoom->canBeModify());
        mAddUserInRooms->setVisible(mRoom && mRoom->canBeModify());

        const bool showPruneMessage = mCurrentRocketChatAccount->hasPermission(QStringLiteral("clean-channel-history"));
        mPruneMessages->setVisible(showPruneMessage);
        mPruneMessagesSeparator->setVisible(showPruneMessage);

        mExportMessages->setVisible(mCurrentRocketChatAccount->hasPermission(QStringLiteral("mail-messages")));

        // FIXME Disable for the moment
        mOffTheRecordMessages->setVisible(false && mCurrentRocketChatAccount->ruqolaServerConfig()->otrEnabled()
                                          && mRoom->channelType() == Room::RoomType::Direct);

        // FIXME Disable for the moment
        // TODO
        mEncryptMessages->setVisible(false);
    }
}

#include "moc_channelactionpopupmenu.cpp"
