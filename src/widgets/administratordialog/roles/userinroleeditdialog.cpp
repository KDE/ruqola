/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userinroleeditdialog.h"
#include "usersinrolewidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>
namespace
{
const char myUserInRoleEditDialogGroupName[] = "UserInRoleEditDialog";
}

UserInRoleEditDialog::UserInRoleEditDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mUserInRoleEditWidget(new UsersInRoleWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Users in Role"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mUserInRoleEditWidget->setObjectName(QStringLiteral("mUserInRoleEditWidget"));
    mainLayout->addWidget(mUserInRoleEditWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &UserInRoleEditDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &UserInRoleEditDialog::accept);
    readConfig();
}

UserInRoleEditDialog::~UserInRoleEditDialog()
{
    writeConfig();
}

const QString &UserInRoleEditDialog::roleId() const
{
    return mUserInRoleEditWidget->roleId();
}

void UserInRoleEditDialog::setRoleId(const QString &newRoleId)
{
    mUserInRoleEditWidget->setRoleId(newRoleId);
}

const QString &UserInRoleEditDialog::roleName() const
{
    return mUserInRoleEditWidget->roleName();
}

void UserInRoleEditDialog::setRoleName(const QString &newRoleName)
{
    mUserInRoleEditWidget->setRoleName(newRoleName);
}

void UserInRoleEditDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myUserInRoleEditDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void UserInRoleEditDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myUserInRoleEditDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_userinroleeditdialog.cpp"
