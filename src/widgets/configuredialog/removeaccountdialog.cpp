/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "removeaccountdialog.h"
#include "removeaccountwidget.h"
#include <QVBoxLayout>

RemoveAccountDialog::RemoveAccountDialog(QWidget *parent)
    : QDialog(parent)
    , mRemoveAccountWidget(new RemoveAccountWidget(this))
{
}

RemoveAccountDialog::~RemoveAccountDialog() = default;
