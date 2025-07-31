/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchmessagedialog.h"
using namespace Qt::Literals::StringLiterals;

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
{
    setWindowTitle(i18nc("@title:window", "Search Messages"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mSearchMessageWidget->setObjectName(u"mSearchMessageWidget"_s);
    mainLayout->addWidget(mSearchMessageWidget);
    connect(mSearchMessageWidget, &SearchMessageWidget::goToMessageRequested, this, &SearchMessageDialog::goToMessageRequested);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &SearchMessageDialog::reject);
    readConfig();
}

SearchMessageDialog::~SearchMessageDialog()
{
    writeConfig();
}

void SearchMessageDialog::setRoomId(const QByteArray &roomId)
{
    mSearchMessageWidget->setRoomId(roomId);
}

void SearchMessageDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySearchMessageDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void SearchMessageDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(mySearchMessageDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void SearchMessageDialog::setRoom(Room *room)
{
    mSearchMessageWidget->setRoom(room);
    setWindowTitle(i18nc("@title:window",
                         "Search Messages in '%1' (%2)",
                         room->displayRoomName(),
                         mSearchMessageWidget->currentRocketChatAccount() ? mSearchMessageWidget->currentRocketChatAccount()->accountName() : u"account"_s));
}

#include "moc_searchmessagedialog.cpp"
