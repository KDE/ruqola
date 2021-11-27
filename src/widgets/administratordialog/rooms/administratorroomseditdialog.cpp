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

#include "administratorroomseditdialog.h"
#include "administratorroomseditwidget.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

AdministratorRoomsEditDialog::AdministratorRoomsEditDialog(RoomType roomType, QWidget *parent)
    : QDialog(parent)
    , mAdministratorRoomEditWidget(new AdministratorRoomsEditWidget(this))
    , mRoomType(roomType)
{
    setWindowTitle(i18nc("@title:window", "Edit Room"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mAdministratorRoomEditWidget->setObjectName(QStringLiteral("mAdministratorRoomEditWidget"));
    mainLayout->addWidget(mAdministratorRoomEditWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    auto okButton = button->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);

    connect(button, &QDialogButtonBox::rejected, this, &AdministratorRoomsEditDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &AdministratorRoomsEditDialog::accept);
    resize(350, 50);
    //    connect(mRoleEditWidget, &RoleEditWidget::updateOkButton, this, [okButton](bool b) {
    //        okButton->setEnabled(b);
    //    });
}

AdministratorRoomsEditDialog::~AdministratorRoomsEditDialog() = default;

AdministratorRoomsEditDialog::RoomType AdministratorRoomsEditDialog::roomType() const
{
    return mRoomType;
}

const AdministratorRoomsEditBaseWidget::RoomEditInfo &AdministratorRoomsEditDialog::roomEditInfo() const
{
    return mAdministratorRoomEditWidget->roomEditInfo();
}

void AdministratorRoomsEditDialog::setRoomEditInfo(const AdministratorRoomsEditBaseWidget::RoomEditInfo &newRoomEditInfo)
{
    mAdministratorRoomEditWidget->setRoomEditInfo(newRoomEditInfo);
}
