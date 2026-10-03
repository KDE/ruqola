/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "addusersinroomdialog.h"

#include "addusersinroomwidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>
namespace
{
const char myConfigAddUsersInRoomDialogGroupName[] = "AddUsersInRoomDialog";
}
using namespace Qt::Literals::StringLiterals;
AddUsersInRoomDialog::AddUsersInRoomDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mAddUsersInRoomWidget(new AddUsersInRoomWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Add Users in Room"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mAddUsersInRoomWidget->setObjectName(u"mAddUsersInRoomWidget"_s);
    mainLayout->addWidget(mAddUsersInRoomWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &AddUsersInRoomDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &AddUsersInRoomDialog::accept);
    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    connect(mAddUsersInRoomWidget, &AddUsersInRoomWidget::updateOkButton, okButton, &QPushButton::setEnabled);
    okButton->setEnabled(false);
    readConfig();
}

AddUsersInRoomDialog::~AddUsersInRoomDialog() = default;

QList<QByteArray> AddUsersInRoomDialog::userIds() const
{
    return mAddUsersInRoomWidget->userIds();
}

QStringList AddUsersInRoomDialog::userNames() const
{
    return mAddUsersInRoomWidget->userNames();
}

void AddUsersInRoomDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myConfigAddUsersInRoomDialogGroupName), QSize(800, 300));
}

#include "moc_addusersinroomdialog.cpp"
