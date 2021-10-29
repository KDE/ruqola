/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "createdirectmessagesdialog.h"
#include "createdirectmessageswidget.h"
#include "rocketchataccount.h"

#include <KLocalizedString>

#include <KConfigGroup>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace
{
static const char myConfigCreateDirectMessagesDialogGroupName[] = "CreateDirectMessagesDialog";
}
CreateDirectMessagesDialog::CreateDirectMessagesDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mCreateDirectMessagesWidget(new CreateDirectMessagesWidget(this))
    , mCurrentRocketChatAccount(account)
{
    setWindowTitle(i18nc("@title:window", "Create Direct Messages"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mCreateDirectMessagesWidget->setObjectName(QStringLiteral("mCreateDirectMessagesWidget"));
    mainLayout->addWidget(mCreateDirectMessagesWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    connect(buttonBox, &QDialogButtonBox::accepted, this, &CreateDirectMessagesDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &CreateDirectMessagesDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
    auto okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);
    connect(mCreateDirectMessagesWidget, &CreateDirectMessagesWidget::updateOkButton, this, [okButton](bool state) {
        okButton->setEnabled(state);
    });
    connect(buttonBox, &QDialogButtonBox::accepted, this, &CreateDirectMessagesDialog::slotAccepted);
}
CreateDirectMessagesDialog::~CreateDirectMessagesDialog()
{
    writeConfig();
}

void CreateDirectMessagesDialog::slotAccepted()
{
    const QStringList usernames = userNames();
    mCurrentRocketChatAccount->createDirectMessages(usernames);
}

QStringList CreateDirectMessagesDialog::userNames() const
{
    return mCreateDirectMessagesWidget->userNames();
}

void CreateDirectMessagesDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigCreateDirectMessagesDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void CreateDirectMessagesDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigCreateDirectMessagesDialogGroupName);
    group.writeEntry("Size", size());
}
