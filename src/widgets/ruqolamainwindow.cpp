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

#include "ruqola.h"
#include "rocketchataccount.h"
#include "accountmanager.h"
#include "ruqolamainwindow.h"
#include "ruqolacentralwidget.h"
#include "dialogs/serverinfodialog.h"
#include "dialogs/searchchanneldialog.h"
#include "dialogs/createnewchanneldialog.h"
#include "dialogs/createnewaccountdialog.h"
#include "dialogs/showpinnedmessagesdialog.h"
#include "dialogs/showstarredmessagesdialog.h"
#include "dialogs/showmentionsmessagesdialog.h"
#include "dialogs/showsnipperedmessagesdialog.h"
#include "dialogs/searchmessagedialog.h"
#include "dialogs/configurenotificationdialog.h"
#include <KActionCollection>
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
#include <QPointer>
#include <QIcon>

namespace {
static const char myConfigGroupName[] = "RuqolaMainWindow";
}

RuqolaMainWindow::RuqolaMainWindow(QWidget *parent)
    : KXmlGuiWindow(parent)
{
    mMainWidget = new RuqolaCentralWidget(this);
    mMainWidget->setObjectName(QStringLiteral("mMainWidget"));
    setCentralWidget(mMainWidget);
    setupActions();
    setupGUI(KXmlGuiWindow::Default, QStringLiteral(":/kxmlgui5/ruqola/ruqolaui.rc"));
    readConfig();
}

RuqolaMainWindow::~RuqolaMainWindow()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group = config->group(myConfigGroupName);
    group.writeEntry("Size", size());

    Ruqola::destroy();
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

void RuqolaMainWindow::setupActions()
{
    KActionCollection *ac = actionCollection();

    KStandardAction::quit(this, &RuqolaMainWindow::close, ac);
    KStandardAction::preferences(this, &RuqolaMainWindow::slotConfigure, ac);

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

    mShowMentions = new QAction(i18n("Show Mentions..."), this);
    connect(mShowMentions, &QAction::triggered, this, &RuqolaMainWindow::slotShowMentions);
    ac->addAction(QStringLiteral("show_mentions"), mShowMentions);

    mShowPinnedMessages = new QAction(i18n("Show Pinned Messages..."), this);
    connect(mShowPinnedMessages, &QAction::triggered, this, &RuqolaMainWindow::slotPinnedMessages);
    ac->addAction(QStringLiteral("show_pinned_messages"), mShowPinnedMessages);

    mShowStarredMessages = new QAction(i18n("Show Starred Messages..."), this);
    connect(mShowStarredMessages, &QAction::triggered, this, &RuqolaMainWindow::slotStarredMessages);
    ac->addAction(QStringLiteral("show_starred_messages"), mShowStarredMessages);

    mShowSnipperedMessages = new QAction(i18n("Show Snippered Messages..."), this);
    connect(mShowSnipperedMessages, &QAction::triggered, this, &RuqolaMainWindow::slotSnipperedMessages);
    ac->addAction(QStringLiteral("show_snippered_messages"), mShowSnipperedMessages);

    mSearchMessages = new QAction(QIcon::fromTheme(QStringLiteral("edit-find")), i18n("Search Messages..."), this);
    connect(mSearchMessages, &QAction::triggered, this, &RuqolaMainWindow::slotSearchMessages);
    ac->addAction(QStringLiteral("search_messages"), mSearchMessages);

    mConfigureNotification = new QAction(QIcon::fromTheme(QStringLiteral("preferences-desktop-notification")), i18n("Configure Notification..."), this);
    connect(mConfigureNotification, &QAction::triggered, this, &RuqolaMainWindow::slotConfigureNotification);
    ac->addAction(QStringLiteral("configure_notification"), mConfigureNotification);
}

void RuqolaMainWindow::slotConfigureNotification()
{
    QPointer<ConfigureNotificationDialog> dlg = new ConfigureNotificationDialog(this);
    dlg->exec();
    delete dlg;
}

void RuqolaMainWindow::slotSearchMessages()
{
    QPointer<SearchMessageDialog> dlg = new SearchMessageDialog(this);
    dlg->setRoomId(mMainWidget->roomId());
    dlg->exec();
    delete dlg;
}

void RuqolaMainWindow::slotStarredMessages()
{
    QPointer<ShowStarredMessagesDialog> dlg = new ShowStarredMessagesDialog(this);
    dlg->setModel(Ruqola::self()->rocketChatAccount()->listMessagesFilterProxyModel());
    Ruqola::self()->rocketChatAccount()->getListMessages(mMainWidget->roomId(), ListMessagesModel::StarredMessages);
    dlg->exec();
    delete dlg;
}

void RuqolaMainWindow::slotPinnedMessages()
{
    QPointer<ShowPinnedMessagesDialog> dlg = new ShowPinnedMessagesDialog(this);
    dlg->setModel(Ruqola::self()->rocketChatAccount()->listMessagesFilterProxyModel());
    Ruqola::self()->rocketChatAccount()->getListMessages(mMainWidget->roomId(), ListMessagesModel::PinnedMessages);
    dlg->exec();
    delete dlg;
}

void RuqolaMainWindow::slotShowMentions()
{
//    QPointer<ShowMentionsMessagesDialog> dlg = new ShowMentionsMessagesDialog(this);
//    dlg->setModel(Ruqola::self()->rocketChatAccount()->listMessagesFilterProxyModel());
//    Ruqola::self()->rocketChatAccount()->getListMessages(mMainWidget->roomId(), ListMessagesModel::);
//    dlg->exec();
//    delete dlg;
}

void RuqolaMainWindow::slotSnipperedMessages()
{
    QPointer<ShowSnipperedMessagesDialog> dlg = new ShowSnipperedMessagesDialog(this);
    dlg->setModel(Ruqola::self()->rocketChatAccount()->listMessagesFilterProxyModel());
    Ruqola::self()->rocketChatAccount()->getListMessages(mMainWidget->roomId(), ListMessagesModel::SnipperedMessages);
    dlg->exec();
    delete dlg;
}

void RuqolaMainWindow::slotCreateNewChannel()
{
    QPointer<CreateNewChannelDialog> dlg = new CreateNewChannelDialog(this);
    if (dlg->exec()) {
        const CreateNewChannelDialog::NewChannelInfo info = dlg->channelInfo();
        Ruqola::self()->rocketChatAccount()->createNewChannel(info.channelName, info.readOnly, info.privateChannel, info.usersName, info.encryptedRoom, info.password, info.broadCast);
    }
    delete dlg;
}

void RuqolaMainWindow::slotConfigure()
{
    //TODO
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
    dlg->setServerConfigInfo(Ruqola::self()->rocketChatAccount()->serverConfigInfo());
    dlg->exec();
    delete dlg;
}

void RuqolaMainWindow::slotLogout()
{
    Ruqola::self()->rocketChatAccount()->logOut();
}

void RuqolaMainWindow::slotSearchChannel()
{
    QPointer<SearchChannelDialog> dlg = new SearchChannelDialog(this);
    dlg->exec();
    delete dlg;
}
