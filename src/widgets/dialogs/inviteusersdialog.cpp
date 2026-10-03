/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "inviteusersdialog.h"

#include "inviteuserswidget.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

namespace
{
const char myInviteUsersDialogGroupName[] = "InviteUsersDialog";
}
using namespace Qt::Literals::StringLiterals;
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

InviteUsersDialog::~InviteUsersDialog() = default;

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
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myInviteUsersDialogGroupName), QSize(300, 300));
}

#include "moc_inviteusersdialog.cpp"
