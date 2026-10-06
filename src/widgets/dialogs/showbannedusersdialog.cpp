/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showbannedusersdialog.h"
#include "rocketchataccount.h"
#include "showbanneduserswidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>
namespace
{
const char myShowBannedUsersDialogGroupName[] = "ShowBannedUsersDialog";
}

using namespace Qt::Literals::StringLiterals;
ShowBannedUsersDialog::ShowBannedUsersDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mShowBannedUsersWidget(new ShowBannedUsersWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Show Banned Users - %1", account ? account->accountName() : u"account"_s));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mShowBannedUsersWidget->setObjectName(u"mShowBannedUsersWidget"_s);
    mainLayout->addWidget(mShowBannedUsersWidget);

    setAttribute(Qt::WA_DeleteOnClose);
    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ShowBannedUsersDialog::reject);

    readConfig();
}

ShowBannedUsersDialog::~ShowBannedUsersDialog() = default;

void ShowBannedUsersDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myShowBannedUsersDialogGroupName), QSize(800, 600));
}

void ShowBannedUsersDialog::setRoomId(const QByteArray &roomId)
{
    mShowBannedUsersWidget->setRoomId(roomId);
}

#include "moc_showbannedusersdialog.cpp"
