/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinroomdialog.h"
using namespace Qt::Literals::StringLiterals;

#include "usersinroomwidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
const char myUsersInRoomDialogGroupName[] = "UsersInRoomDialog";
}

UsersInRoomDialog::UsersInRoomDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mUsersInRoomWidget(new UsersInRoomWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Users"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mUsersInRoomWidget->setObjectName(u"mUsersInRoomWidget"_s);
    mainLayout->addWidget(mUsersInRoomWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(u"button"_s);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &UsersInRoomDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
}

UsersInRoomDialog::~UsersInRoomDialog()
{
    writeConfig();
}

void UsersInRoomDialog::setRoom(Room *room)
{
    mUsersInRoomWidget->setRoom(room);
}

void UsersInRoomDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myUsersInRoomDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void UsersInRoomDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myUsersInRoomDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

#include "moc_usersinroomdialog.cpp"
