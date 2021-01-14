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

#include "usersinroomdialog.h"
#include "usersinroomwidget.h"

#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
#include <QVBoxLayout>
#include <QDialogButtonBox>

namespace {
static const char myUsersInRoomDialogGroupName[] = "UsersInRoomDialog";
}

UsersInRoomDialog::UsersInRoomDialog(QWidget *parent)
    : QDialog(parent)
    , mUsersInRoomWidget(new UsersInRoomWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Video"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mUsersInRoomWidget->setObjectName(QStringLiteral("mUsersInRoomWidget"));
    mainLayout->addWidget(mUsersInRoomWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    connect(buttonBox, &QDialogButtonBox::rejected, this, &UsersInRoomDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
}

UsersInRoomDialog::~UsersInRoomDialog()
{
    writeConfig();
}

void UsersInRoomDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myUsersInRoomDialogGroupName);
    group.writeEntry("Size", size());
}

void UsersInRoomDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myUsersInRoomDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}
