/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchmessagedialog.h"
#include "rocketchataccount.h"
#include "room.h"
#include "searchmessagewidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
static const char mySearchMessageDialogGroupName[] = "SearchMessageDialog";
}
SearchMessageDialog::SearchMessageDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mSearchMessageWidget(new SearchMessageWidget(account, this))
    , mRocketChatAccount(account)
{
    setWindowTitle(i18nc("@title:window", "Search Messages"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mSearchMessageWidget->setObjectName(QStringLiteral("mSearchMessageWidget"));
    mainLayout->addWidget(mSearchMessageWidget);
    connect(mSearchMessageWidget, &SearchMessageWidget::goToMessageRequested, this, &SearchMessageDialog::goToMessageRequested);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &SearchMessageDialog::reject);
    readConfig();
}

SearchMessageDialog::~SearchMessageDialog()
{
    writeConfig();
}

void SearchMessageDialog::setRoomId(const QString &roomId)
{
    mSearchMessageWidget->setRoomId(roomId);
}

void SearchMessageDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), mySearchMessageDialogGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void SearchMessageDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), mySearchMessageDialogGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void SearchMessageDialog::setModel(SearchMessageFilterProxyModel *model)
{
    mSearchMessageWidget->setModel(model);
}

void SearchMessageDialog::setRoom(Room *room)
{
    mSearchMessageWidget->setRoom(room);
    setWindowTitle(i18nc("@title:window",
                         "Search Messages in '%1' (%2)",
                         room->displayRoomName(),
                         mRocketChatAccount ? mRocketChatAccount->accountName() : QStringLiteral("account")));
}
