/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "administratorroomseditdialog.h"
#include "administratordirectroomseditwidget.h"
#include "administratorroomseditwidget.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

AdministratorRoomsEditDialog::AdministratorRoomsEditDialog(RoomType roomType, QWidget *parent)
    : QDialog(parent)
    , mRoomType(roomType)
{
    setWindowTitle(i18nc("@title:window", "Edit Room"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    switch (mRoomType) {
    case DirectRoom:
        mAdministratorRoomEditWidget = new AdministratorDirectRoomsEditWidget(this);
        break;
    case Unknown:
        Q_ASSERT(false);
        break;
    case Team:
    case Group:
    case Channel:
        mAdministratorRoomEditWidget = new AdministratorRoomsEditWidget(this);
        break;
    }
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

AdministratorRoomsEditBaseWidget::RoomEditInfo AdministratorRoomsEditDialog::roomEditInfo() const
{
    return mAdministratorRoomEditWidget->roomEditInfo();
}

void AdministratorRoomsEditDialog::setRoomEditInfo(const AdministratorRoomsEditBaseWidget::RoomEditInfo &newRoomEditInfo)
{
    mAdministratorRoomEditWidget->setRoomEditInfo(newRoomEditInfo);
}
