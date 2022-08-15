/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "managedevicedialog.h"
#include "managedevicewidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>

namespace
{
const char myManageDeviceDialogConfigGroupName[] = "ManageDeviceDialog";
}

ManageDeviceDialog::ManageDeviceDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mManageDeviceWidget(new ManageDeviceWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Manage Device"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mManageDeviceWidget->setObjectName(QStringLiteral("mManageDeviceWidget"));
    mainLayout->addWidget(mManageDeviceWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ManageDeviceDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &ManageDeviceDialog::accept);
    readConfig();
}

ManageDeviceDialog::~ManageDeviceDialog()
{
    writeConfig();
}

void ManageDeviceDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myManageDeviceDialogConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(400, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void ManageDeviceDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myManageDeviceDialogConfigGroupName);
    group.writeEntry("Size", size());
}
