/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "removeaccountdialog.h"
using namespace Qt::Literals::StringLiterals;

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
    mainLayout->setObjectName(u"mainLayout"_s);

    mRemoveAccountWidget->setObjectName(u"mRemoveAccountWidget"_s);
    mainLayout->addWidget(mRemoveAccountWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
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

#include "moc_removeaccountdialog.cpp"
