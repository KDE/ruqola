/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "removeaccountdialog.h"
#include "removeaccountwidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

RemoveAccountDialog::RemoveAccountDialog(QWidget *parent)
    : QDialog(parent)
    , mRemoveAccountWidget(new RemoveAccountWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Remove Account"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mRemoveAccountWidget->setObjectName(QStringLiteral("mRemoveAccountWidget"));
    mainLayout->addWidget(mRemoveAccountWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &RemoveAccountDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &RemoveAccountDialog::accept);
}

RemoveAccountDialog::~RemoveAccountDialog() = default;

bool RemoveAccountDialog::removeLogs() const
{
    return mRemoveAccountWidget->removeLogs();
}

void RemoveAccountDialog::setAccountName(const QString &accountName)
{
    mRemoveAccountWidget->setAccountName(accountName);
}
