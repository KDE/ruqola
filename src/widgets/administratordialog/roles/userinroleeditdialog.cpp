/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userinroleeditdialog.h"

#include "usersinrolewidget.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>
namespace
{
const char myUserInRoleEditDialogGroupName[] = "UserInRoleEditDialog";
}

using namespace Qt::Literals::StringLiterals;
UserInRoleEditDialog::UserInRoleEditDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mUserInRoleEditWidget(new UsersInRoleWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Users in Role"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mUserInRoleEditWidget->setObjectName(u"mUserInRoleEditWidget"_s);
    mainLayout->addWidget(mUserInRoleEditWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &UserInRoleEditDialog::reject);
    readConfig();
}

UserInRoleEditDialog::~UserInRoleEditDialog() = default;

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
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myUserInRoleEditDialogGroupName), QSize(400, 300));
}

#include "moc_userinroleeditdialog.cpp"
