/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabasedialog.h"
#include "databasedialog/exploredatabaseroomspendingtypedinfostoragewidget.h"
#include "databasedialog/exploredatabaseroomsubscriptionswidget.h"
#include "exploredatabaseaccountstoragewidget.h"
#include "exploredatabasee2estoragewidget.h"
#include "exploredatabaseglobalstoragewidget.h"
#include "exploredatabasejsonwidget.h"
#include "exploredatabasemessagesstoragewidget.h"
#include "exploredatabaseroomsstoragewidget.h"

#include "exploredatabaseviewmessageswidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWindow>

using namespace Qt::Literals::StringLiterals;
namespace
{
const char myExploreDatabaseDialogConfigGroupName[] = "ExploreDatabaseDialog";
}
ExploreDatabaseDialog::ExploreDatabaseDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mExploreDatabaseWidget(new ExploreDatabaseViewMessagesWidget(account, this))
    , mExploreDatabaseJsonWidget(new ExploreDatabaseJsonWidget(this))
    , mExploreDatabaseMessagesStorageWidget(new ExploreDatabaseMessagesStorageWidget(account, this))
    , mExploreDatabaseAccountStorageWidget(new ExploreDatabaseAccountStorageWidget(account, this))
    , mExploreDatabaseRoomsStorageWidget(new ExploreDatabaseRoomsStorageWidget(account, this))
    , mExploreDatabaseGlobalStorageWidget(new ExploreDatabaseGlobalStorageWidget(account, this))
    , mExploreDatabaseE2EStorageWidget(new ExploreDatabaseE2EStorageWidget(account, this))
    , mExploreDatabaseRoomsPendingTypedInfoStorageWidget(new ExploreDatabaseRoomsPendingTypedInfoStorageWidget(account, this))
    , mExploreDatabaseRoomSubscriptionsWidget(new ExploreDatabaseRoomSubscriptionsWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Database Info"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    auto tabWidget = new QTabWidget(this);
    tabWidget->setObjectName(u"tabWidget"_s);
    mainLayout->addWidget(tabWidget);

    mExploreDatabaseWidget->setObjectName(u"mExploreDatabaseWidget"_s);
    tabWidget->addTab(mExploreDatabaseWidget, i18n("Messages"));

    mExploreDatabaseJsonWidget->setObjectName(u"mExploreDatabaseJsonWidget"_s);
    tabWidget->addTab(mExploreDatabaseJsonWidget, i18n("Json"));

    mExploreDatabaseMessagesStorageWidget->setObjectName(u"mExploreDatabaseStorageWidget"_s);
    tabWidget->addTab(mExploreDatabaseMessagesStorageWidget, i18n("DataBase Messages"));

    mExploreDatabaseAccountStorageWidget->setObjectName(u"mExploreDatabaseAccountStorageWidget"_s);
    tabWidget->addTab(mExploreDatabaseAccountStorageWidget, i18n("DataBase Accounts"));

    mExploreDatabaseRoomsStorageWidget->setObjectName(u"mExploreDatabaseRoomsStorageWidget"_s);
    tabWidget->addTab(mExploreDatabaseRoomsStorageWidget, i18n("DataBase Rooms"));

    mExploreDatabaseGlobalStorageWidget->setObjectName(u"mExploreDatabaseGlobalStorageWidget"_s);
    tabWidget->addTab(mExploreDatabaseGlobalStorageWidget, i18n("DataBase Global"));

    mExploreDatabaseE2EStorageWidget->setObjectName(u"mExploreDatabaseE2EStorageWidget"_s);
    tabWidget->addTab(mExploreDatabaseE2EStorageWidget, i18n("DataBase E2E"));

    mExploreDatabaseRoomsPendingTypedInfoStorageWidget->setObjectName(u"mExploreDatabaseRoomsPendingTypedInfoStorageWidget"_s);
    tabWidget->addTab(mExploreDatabaseRoomsPendingTypedInfoStorageWidget, i18n("DataBase Room Pending Typed Info"));

    mExploreDatabaseRoomSubscriptionsWidget->setObjectName(u"mExploreDatabaseRoomSubscriptionsWidget"_s);
    tabWidget->addTab(mExploreDatabaseRoomSubscriptionsWidget, i18n("DataBase Room Subscriptions"));

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ExploreDatabaseDialog::reject);

    connect(mExploreDatabaseWidget,
            &ExploreDatabaseViewMessagesWidget::messagesLoaded,
            mExploreDatabaseJsonWidget,
            &ExploreDatabaseJsonWidget::slotLoadedMessages);

    connect(mExploreDatabaseWidget,
            &ExploreDatabaseViewMessagesWidget::loadModelFromDataBase,
            mExploreDatabaseMessagesStorageWidget,
            &ExploreDatabaseMessagesStorageWidget::slotLoadModelFromDataBase);

    readConfig();
}

ExploreDatabaseDialog::~ExploreDatabaseDialog()
{
    writeConfig();
}

void ExploreDatabaseDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myExploreDatabaseDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ExploreDatabaseDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myExploreDatabaseDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_exploredatabasedialog.cpp"
