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

#include "ruqolamainwindow.h"

#include "config-ruqola.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "accountmanager.h"
#include "roomwrapper.h"
#include "receivetypingnotificationmanager.h"
#include "ruqolacentralwidget.h"
#include "misc/accountmenu.h"
#include "misc/accountsoverviewwidget.h"
#include "dialogs/serverinfodialog.h"
#include "dialogs/searchchanneldialog.h"
#include "dialogs/createnewchanneldialog.h"
#include "dialogs/createnewaccountdialog.h"
#include "dialogs/channelpassworddialog.h"
#include "dialogs/channelinfodialog.h"
#include "dialogs/directchannelinfodialog.h"
#include "dialogs/addusersinroomdialog.h"
#include "configuredialog/configuresettingsdialog.h"
#include "dialogs/inviteusersdialog.h"
#include <KActionCollection>
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
#include <KStandardAction>
#include <QIcon>
#include <QStatusBar>
#include <QLabel>

#if HAVE_KUSERFEEDBACK
#include "userfeedback/userfeedbackmanager.h"
#include <KUserFeedback/NotificationPopup>
#include <KUserFeedback/Provider>
#endif

namespace {
static const char myConfigGroupName[] = "RuqolaMainWindow";
}

RuqolaMainWindow::RuqolaMainWindow(QWidget *parent)
    : KXmlGuiWindow(parent)
{
    mMainWidget = new RuqolaCentralWidget(this);
    mMainWidget->setObjectName(QStringLiteral("mMainWidget"));
    connect(mMainWidget, &RuqolaCentralWidget::channelSelected, this, [this]() {
        changeActionStatus(true);
    });
    setCentralWidget(mMainWidget);
    setupActions();
    setupStatusBar();
    setupGUI(/*QStringLiteral(":/kxmlgui5/ruqola/ruqolaui.rc")*/);
    readConfig();
    connect(Ruqola::self()->accountManager(), &AccountManager::currentAccountChanged, this, &RuqolaMainWindow::slotAccountChanged);
    slotAccountChanged();
#if HAVE_KUSERFEEDBACK
    KUserFeedback::NotificationPopup *userFeedBackNotificationPopup = new KUserFeedback::NotificationPopup(this);
    userFeedBackNotificationPopup->setFeedbackProvider(UserFeedBackManager::self()->userFeedbackProvider());
#endif
}

RuqolaMainWindow::~RuqolaMainWindow()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group = config->group(myConfigGroupName);
    group.writeEntry("Size", size());

    Ruqola::destroy();
}

void RuqolaMainWindow::setupStatusBar()
{
    mStatusBarTypingMessage = new QLabel(this);
    mStatusBarTypingMessage->setTextFormat(Qt::RichText);
    mStatusBarTypingMessage->setObjectName(QStringLiteral("mStatusBarTypingMessage"));
    statusBar()->addPermanentWidget(mStatusBarTypingMessage);
    mAccountOverviewWidget = new AccountsOverviewWidget(this);
    statusBar()->addPermanentWidget(mAccountOverviewWidget);
}

void RuqolaMainWindow::slotAccountChanged()
{
    if (mCurrentRocketChatAccount) {
        disconnect(mCurrentRocketChatAccount, nullptr, this, nullptr);
    }
    mCurrentRocketChatAccount = Ruqola::self()->rocketChatAccount();
    connect(mCurrentRocketChatAccount->receiveTypingNotificationManager(), &ReceiveTypingNotificationManager::notificationChanged, this, &RuqolaMainWindow::slotTypingNotificationChanged);
    connect(mCurrentRocketChatAccount->receiveTypingNotificationManager(), &ReceiveTypingNotificationManager::clearNotification, this, &RuqolaMainWindow::slotClearNotification);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::missingChannelPassword, this, &RuqolaMainWindow::slotMissingChannelPassword);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::publicSettingChanged, this, &RuqolaMainWindow::updateActions);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::serverVersionChanged, this, &RuqolaMainWindow::updateActions);

    updateActions();
    changeActionStatus(false); //Disable actions when switching.
    slotClearNotification(); //Clear notification when we switch too.
    mMainWidget->setCurrentRocketChatAccount(mCurrentRocketChatAccount);
}

void RuqolaMainWindow::changeActionStatus(bool enabled)
{
    mLoadChannelHistory->setEnabled(enabled);
    mChannelInfo->setEnabled(enabled);
    mStartVideoChat->setEnabled(enabled);
    mSaveAs->setEnabled(enabled);
    RoomWrapper *roomWrapper = mMainWidget->roomWrapper();
    mAddUserInRooms->setEnabled(enabled && roomWrapper && roomWrapper->canBeModify());
    mInviteGenerateUrl->setEnabled(enabled);
}

void RuqolaMainWindow::updateActions()
{
    mUnreadOnTop->setChecked(mCurrentRocketChatAccount->sortUnreadOnTop());
    mStartVideoChat->setVisible(mCurrentRocketChatAccount->jitsiEnabled());
    mInviteGenerateUrl->setVisible(mCurrentRocketChatAccount->hasInviteUserSupport());
}

void RuqolaMainWindow::readConfig()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group = KConfigGroup(config, myConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void RuqolaMainWindow::slotClearNotification()
{
    mStatusBarTypingMessage->clear();
}

void RuqolaMainWindow::slotTypingNotificationChanged(const QString &roomId, const QString &notificationStr)
{
    if (mMainWidget->roomId() == roomId) {
        mStatusBarTypingMessage->setText(notificationStr);
    }
}

void RuqolaMainWindow::setupActions()
{
    KActionCollection *ac = actionCollection();

    KStandardAction::quit(this, &RuqolaMainWindow::close, ac);
    KStandardAction::preferences(this, &RuqolaMainWindow::slotConfigure, ac);

    mSaveAs = KStandardAction::saveAs(this, &RuqolaMainWindow::slotSaveAs, ac);

    QAction *act = new QAction(i18n("Add Account..."), this);
    connect(act, &QAction::triggered, this, &RuqolaMainWindow::slotAddAccount);
    ac->addAction(QStringLiteral("add_account"), act);

    //Move in specific server widget
    mServerInfo = new QAction(i18n("Server Info..."), this);
    connect(mServerInfo, &QAction::triggered, this, &RuqolaMainWindow::slotServerInfo);
    ac->addAction(QStringLiteral("server_info"), mServerInfo);

    mLogout = new QAction(i18n("Logout"), this);
    connect(mLogout, &QAction::triggered, this, &RuqolaMainWindow::slotLogout);
    ac->addAction(QStringLiteral("logout"), mLogout);

    mSearchChannel = new QAction(i18n("Search Channel..."), this);
    connect(mSearchChannel, &QAction::triggered, this, &RuqolaMainWindow::slotSearchChannel);
    ac->addAction(QStringLiteral("search_channel"), mSearchChannel);

    mCreateNewChannel = new QAction(i18n("Create New Channel..."), this);
    connect(mCreateNewChannel, &QAction::triggered, this, &RuqolaMainWindow::slotCreateNewChannel);
    ac->addAction(QStringLiteral("create_new_channel"), mCreateNewChannel);

    mLoadChannelHistory = new QAction(i18n("Load Recent History"), this);
    connect(mLoadChannelHistory, &QAction::triggered, this, &RuqolaMainWindow::slotLoadRecentHistory);
    ac->addAction(QStringLiteral("load_recent_history"), mLoadChannelHistory);

    mAccountMenu = new AccountMenu(this);
    ac->addAction(QStringLiteral("account_menu"), mAccountMenu);

    mUnreadOnTop = new QAction(i18n("Unread on Top"), this);
    mUnreadOnTop->setCheckable(true);
    connect(mUnreadOnTop, &QAction::triggered, this, &RuqolaMainWindow::slotUnreadOnTop);
    ac->addAction(QStringLiteral("unread_on_top"), mUnreadOnTop);

    mChannelInfo = new QAction(i18n("Channel Info..."), this);
    connect(mChannelInfo, &QAction::triggered, this, &RuqolaMainWindow::slotShowChannelInfo);
    ac->addAction(QStringLiteral("channel_info"), mChannelInfo);

    mAddUserInRooms = new QAction(i18n("Add Users in Channel..."), this);
    connect(mAddUserInRooms, &QAction::triggered, this, &RuqolaMainWindow::slotAddUsersInRoom);
    ac->addAction(QStringLiteral("add_user_in_room"), mAddUserInRooms);

    auto clearAlerts = new QAction(i18n("Mark all channels read"), this);
    ac->setDefaultShortcut(clearAlerts, Qt::SHIFT + Qt::Key_Escape);
    connect(clearAlerts, &QAction::triggered, this, &RuqolaMainWindow::slotClearAccountAlerts);
    ac->addAction(QStringLiteral("mark_all_channels_read"), clearAlerts);

    mStartVideoChat = new QAction(QIcon::fromTheme(QStringLiteral("camera-video")), i18n("Video Chat"), this);
    connect(mStartVideoChat, &QAction::triggered, this, &RuqolaMainWindow::slotStartVideoChat);
    ac->addAction(QStringLiteral("video_chat"), mStartVideoChat);

    mInviteGenerateUrl = new QAction(i18n("Invite Users"), this);
    connect(mInviteGenerateUrl, &QAction::triggered, this, &RuqolaMainWindow::slotGenerateInviteUsers);
    ac->addAction(QStringLiteral("generate_invite"), mInviteGenerateUrl);
}

void RuqolaMainWindow::slotAddUsersInRoom()
{
    QPointer<AddUsersInRoomDialog> dlg = new AddUsersInRoomDialog(this);
    if (dlg->exec()) {
        const QStringList users = dlg->users();
        for (const QString &user : users) {
            Ruqola::self()->rocketChatAccount()->addUserToRoom(user, mMainWidget->roomId(), mMainWidget->roomType());
        }
    }
    delete dlg;
}

void RuqolaMainWindow::slotClearAccountAlerts()
{
    if (auto acct = Ruqola::self()->accountManager()->account()) {
        acct->clearAllUnreadMessages();
    }
}

void RuqolaMainWindow::slotShowChannelInfo()
{
    RoomWrapper *roomWrapper = mMainWidget->roomWrapper();
    if (roomWrapper) {
        const QString roomType = mMainWidget->roomType();
        if (roomType == QLatin1String("d")) {
            QPointer<DirectChannelInfoDialog> dlg = new DirectChannelInfoDialog(this);
            dlg->exec();
            delete dlg;
        } else {
            QPointer<ChannelInfoDialog> dlg = new ChannelInfoDialog(this);
            dlg->setRoomWrapper(roomWrapper);
            dlg->exec();
            delete dlg;
        }
    }
}

void RuqolaMainWindow::slotLoadRecentHistory()
{
    mCurrentRocketChatAccount->loadHistory(mMainWidget->roomId());
}

void RuqolaMainWindow::slotCreateNewChannel()
{
    QPointer<CreateNewChannelDialog> dlg = new CreateNewChannelDialog(this);
    if (dlg->exec()) {
        const CreateNewChannelDialog::NewChannelInfo info = dlg->channelInfo();
        //TODO adapt createNewChannel api for using QStringList
        mCurrentRocketChatAccount->createNewChannel(info.channelName, info.readOnly, info.privateChannel, info.usersName.join(QLatin1Char(',')), info.encryptedRoom, info.password, info.broadCast);
    }
    delete dlg;
}

void RuqolaMainWindow::slotConfigure()
{
    QPointer<ConfigureSettingsDialog> dlg = new ConfigureSettingsDialog(this);
    if (dlg->exec()) {
        mAccountOverviewWidget->updateButtons();
    }
    delete dlg;
}

void RuqolaMainWindow::slotAddAccount()
{
    QPointer<CreateNewAccountDialog> dlg = new CreateNewAccountDialog(this);
    if (dlg->exec()) {
        const CreateNewAccountDialog::AccountInfo info = dlg->accountInfo();
        Ruqola::self()->accountManager()->addAccount(info.accountName, info.userName, info.serverName);
    }
    delete dlg;
}

void RuqolaMainWindow::slotServerInfo()
{
    QPointer<ServerInfoDialog> dlg = new ServerInfoDialog(this);
    dlg->setServerConfigInfo(mCurrentRocketChatAccount->serverConfigInfo());
    dlg->exec();
    delete dlg;
}

void RuqolaMainWindow::slotLogout()
{
    mCurrentRocketChatAccount->logOut();
}

void RuqolaMainWindow::slotSearchChannel()
{
    QPointer<SearchChannelDialog> dlg = new SearchChannelDialog(this);
    dlg->exec();
    delete dlg;
}

void RuqolaMainWindow::slotUnreadOnTop(bool checked)
{
    mCurrentRocketChatAccount->setSortUnreadOnTop(checked);
}

void RuqolaMainWindow::slotMissingChannelPassword(const RocketChatRestApi::ChannelBaseJob::ChannelInfo &channelInfo)
{
    QPointer<ChannelPasswordDialog> dlg = new ChannelPasswordDialog(this);
    //TODO add channel name!
    if (dlg->exec()) {
        //FIXME channelinfo
        mCurrentRocketChatAccount->joinRoom(channelInfo.channelInfoIdentifier, dlg->password());
    }
    delete dlg;
}

void RuqolaMainWindow::slotStartVideoChat()
{
    mCurrentRocketChatAccount->createJitsiConfCall(mMainWidget->roomId());
}

void RuqolaMainWindow::slotSaveAs()
{
    qWarning() << "RuqolaMainWindow::slotSaveAs not implemented yet ";
}

void RuqolaMainWindow::slotGenerateInviteUsers()
{
    QPointer<InviteUsersDialog> dlg = new InviteUsersDialog(this);
    dlg->setRoomId(mMainWidget->roomId());
    dlg->generateLink();
    dlg->exec();
    delete dlg;
}
