/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showbannedusersdialog.h"
#include "rocketchataccount.h"
#include "showbanneduserswidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>
namespace
{
const char myShowBannedUsersDialogGroupName[] = "ShowBannedUsersDialog";
}

using namespace Qt::Literals::StringLiterals;
ShowBannedUsersDialog::ShowBannedUsersDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mShowBannedUsersWidget(new ShowBannedUsersWidget(account, this))
    , mRocketChatAccount(account)
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

ShowBannedUsersDialog::~ShowBannedUsersDialog()
{
    writeConfig();
}

void ShowBannedUsersDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myShowBannedUsersDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ShowBannedUsersDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myShowBannedUsersDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_showbannedusersdialog.cpp"
