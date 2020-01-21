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

#include "modifystatusdialog.h"
#include "modifystatuswidget.h"
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <KLocalizedString>


ModifyStatusDialog::ModifyStatusDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Modify Status"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mModifyStatusWidget = new ModifyStatusWidget(this);
    mModifyStatusWidget->setObjectName(QStringLiteral("mModifyStatusWidget"));
    mainLayout->addWidget(mModifyStatusWidget);

    QDialogButtonBox *button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::accepted, this, &ModifyStatusDialog::accept);
    connect(button, &QDialogButtonBox::rejected, this, &ModifyStatusDialog::reject);
}

ModifyStatusDialog::~ModifyStatusDialog()
{

}

User::PresenceStatus ModifyStatusDialog::status() const
{
    return mModifyStatusWidget->status();
}

QString ModifyStatusDialog::messageStatus() const
{
    return mModifyStatusWidget->messageStatus();
}
