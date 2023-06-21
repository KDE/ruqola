/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "modifystatusdialog.h"
#include "modifystatuswidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

ModifyStatusDialog::ModifyStatusDialog(QWidget *parent)
    : QDialog(parent)
    , mModifyStatusWidget(new ModifyStatusWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Modify Status"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mModifyStatusWidget->setObjectName(QStringLiteral("mModifyStatusWidget"));
    mainLayout->addWidget(mModifyStatusWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::accepted, this, &ModifyStatusDialog::accept);
    connect(button, &QDialogButtonBox::rejected, this, &ModifyStatusDialog::reject);
}

ModifyStatusDialog::~ModifyStatusDialog() = default;

User::PresenceStatus ModifyStatusDialog::status() const
{
    return mModifyStatusWidget->status();
}

void ModifyStatusDialog::setStatus(User::PresenceStatus status)
{
    mModifyStatusWidget->setStatus(status);
}

QString ModifyStatusDialog::messageStatus() const
{
    return mModifyStatusWidget->messageStatus();
}

void ModifyStatusDialog::setMessageStatus(const QString &statusStr)
{
    mModifyStatusWidget->setMessageStatus(statusStr);
}

#include "moc_modifystatusdialog.cpp"
