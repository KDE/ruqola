/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorroomseditdialog.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    switch (mRoomType) {
    case RoomType::DirectRoom:
        setWindowTitle(i18nc("@title:window", "Edit Direct Room"));
        mAdministratorRoomEditWidget = new AdministratorDirectRoomsEditWidget(this);
        break;
    case RoomType::Team:
    case RoomType::Group:
    case RoomType::Channel:
        setWindowTitle(i18nc("@title:window", "Edit Room"));
        mAdministratorRoomEditWidget = new AdministratorRoomsEditWidget(this);
        break;
    case RoomType::Unknown:
        Q_ASSERT(false);
        break;
    }
    mAdministratorRoomEditWidget->setObjectName(u"mAdministratorRoomEditWidget"_s);
    mainLayout->addWidget(mAdministratorRoomEditWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);

    connect(button, &QDialogButtonBox::rejected, this, &AdministratorRoomsEditDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &AdministratorRoomsEditDialog::accept);
    resize(350, 50);
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

#include "moc_administratorroomseditdialog.cpp"
