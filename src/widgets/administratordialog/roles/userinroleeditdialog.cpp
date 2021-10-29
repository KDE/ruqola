/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "userinroleeditdialog.h"
#include "userinroleeditwidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace
{
const char myUserInRoleEditDialogGroupName[] = "UserInRoleEditDialog";
}

UserInRoleEditDialog::UserInRoleEditDialog(QWidget *parent)
    : QDialog(parent)
    , mUserInRoleEditWidget(new UserInRoleEditWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Add User in Role"));
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

void UserInRoleEditDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myUserInRoleEditDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(400, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void UserInRoleEditDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myUserInRoleEditDialogGroupName);
    group.writeEntry("Size", size());
}
