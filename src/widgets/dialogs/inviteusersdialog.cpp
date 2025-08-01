/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "inviteusersdialog.h"
using namespace Qt::Literals::StringLiterals;

#include "inviteuserswidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
const char myInviteUsersDialogGroupName[] = "InviteUsersDialog";
}
InviteUsersDialog::InviteUsersDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mInviteUsersWidget(new InviteUsersWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Invite Users"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mInviteUsersWidget->setObjectName(u"mInviteUsersWidget"_s);
    mainLayout->addWidget(mInviteUsersWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(u"button"_s);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &InviteUsersDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
}

InviteUsersDialog::~InviteUsersDialog()
{
    writeConfig();
}

QByteArray InviteUsersDialog::roomId() const
{
    return mInviteUsersWidget->roomId();
}

void InviteUsersDialog::setRoomId(const QByteArray &roomId)
{
    mInviteUsersWidget->setRoomId(roomId);
}

void InviteUsersDialog::generateLink()
{
    mInviteUsersWidget->generateLink();
}

void InviteUsersDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(300, 300));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myInviteUsersDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void InviteUsersDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myInviteUsersDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_inviteusersdialog.cpp"
