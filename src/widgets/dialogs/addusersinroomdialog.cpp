/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "addusersinroomdialog.h"
#include "addusersinroomwidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
namespace
{
static const char myConfigAddUsersInRoomDialogGroupName[] = "AddUsersInRoomDialog";
}
AddUsersInRoomDialog::AddUsersInRoomDialog(QWidget *parent)
    : QDialog(parent)
    , mAddUsersInRoomWidget(new AddUsersInRoomWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Add Users in Room"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mAddUsersInRoomWidget->setObjectName(QStringLiteral("mAddUsersInRoomWidget"));
    mainLayout->addWidget(mAddUsersInRoomWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
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

QStringList AddUsersInRoomDialog::userIds() const
{
    return mAddUsersInRoomWidget->userIds();
}

QStringList AddUsersInRoomDialog::userNames() const
{
    return mAddUsersInRoomWidget->userNames();
}

void AddUsersInRoomDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigAddUsersInRoomDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void AddUsersInRoomDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigAddUsersInRoomDialogGroupName);
    group.writeEntry("Size", size());
}
