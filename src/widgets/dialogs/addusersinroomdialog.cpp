/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "addusersinroomdialog.h"
#include "addusersinroomwidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
namespace {
static const char myConfigGroupName[] = "AddUsersInRoomDialog";
}
AddUsersInRoomDialog::AddUsersInRoomDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Add Users in Room"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    //TODO add label
    mAddUsersInRoomWidget = new AddUsersInRoomWidget(this);
    mAddUsersInRoomWidget->setObjectName(QStringLiteral("mAddUsersInRoomWidget"));
    mainLayout->addWidget(mAddUsersInRoomWidget);

    QDialogButtonBox *button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &AddUsersInRoomDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &AddUsersInRoomDialog::accept);
    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    connect(mAddUsersInRoomWidget, &AddUsersInRoomWidget::updateOkButton, okButton, &QPushButton::setEnabled);
    okButton->setEnabled(false);
    readConfig();
}

AddUsersInRoomDialog::~AddUsersInRoomDialog()
{
    writeConfig();
}

void AddUsersInRoomDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void AddUsersInRoomDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    group.writeEntry("Size", size());
}
