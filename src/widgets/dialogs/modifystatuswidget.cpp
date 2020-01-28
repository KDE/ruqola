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

#include "modifystatuswidget.h"
#include "channellist/statuscombobox.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <KLineEdit>

ModifyStatusWidget::ModifyStatusWidget(QWidget *parent)
    : QWidget(parent)
{
    QFormLayout *mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mStatusCombobox = new StatusCombobox(false, this);
    mStatusCombobox->setObjectName(QStringLiteral("mStatusCombobox"));

    mStatusLineEdit = new KLineEdit(this);
    mStatusLineEdit->setObjectName(QStringLiteral("mStatusLineEdit"));
    mStatusLineEdit->setClearButtonEnabled(true);
    mStatusLineEdit->setTrapReturnKey(true);

    mainLayout->addRow(i18n("Message Status:"), mStatusLineEdit);
    mainLayout->addRow(i18n("Status:"), mStatusCombobox);
}

ModifyStatusWidget::~ModifyStatusWidget()
{
}

void ModifyStatusWidget::setMessageStatus(const QString &statusStr)
{
    mStatusLineEdit->setText(statusStr);
}

QString ModifyStatusWidget::messageStatus() const
{
    return mStatusLineEdit->text();
}

User::PresenceStatus ModifyStatusWidget::status() const
{
    return mStatusCombobox->status();
}

void ModifyStatusWidget::setStatus(User::PresenceStatus status)
{
    mStatusCombobox->setStatus(status);
}
