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

#include "administratorcustomemojicreatedialog.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>

namespace
{
static const char myConfigAdministratorCustomEmojiCreateDialogGroupName[] = "AdministratorCustomEmojiCreateDialog";
}
AdministratorCustomEmojiCreateDialog::AdministratorCustomEmojiCreateDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Create New User Status"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    //    mCreateWidget->setObjectName(QStringLiteral("mCreateWidget"));
    //    mainLayout->addWidget(mCreateWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &AdministratorCustomEmojiCreateDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &AdministratorCustomEmojiCreateDialog::accept);
    readConfig();
}

AdministratorCustomEmojiCreateDialog::~AdministratorCustomEmojiCreateDialog()
{
    writeConfig();
}

void AdministratorCustomEmojiCreateDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigAdministratorCustomEmojiCreateDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void AdministratorCustomEmojiCreateDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigAdministratorCustomEmojiCreateDialogGroupName);
    group.writeEntry("Size", size());
}
