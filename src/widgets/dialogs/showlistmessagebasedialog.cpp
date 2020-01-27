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

#include "showlistmessagebasedialog.h"
#include "showlistmessagebasewidget.h"
#include <QVBoxLayout>
#include <KLocalizedString>
#include <QDialogButtonBox>

ShowListMessageBaseDialog::ShowListMessageBaseDialog(QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mShowListMessage = new ShowListMessageBaseWidget(this);
    mShowListMessage->setObjectName(QStringLiteral("mShowListMessage"));
    mainLayout->addWidget(mShowListMessage);

    QDialogButtonBox *button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ShowListMessageBaseDialog::reject);
    connect(mShowListMessage, &ShowListMessageBaseWidget::loadMoreElements, this, &ShowListMessageBaseDialog::loadMoreElements);
}

ShowListMessageBaseDialog::~ShowListMessageBaseDialog()
{
}

void ShowListMessageBaseDialog::setModel(ListMessagesModelFilterProxyModel *model)
{
    mShowListMessage->setModel(model);
}

void ShowListMessageBaseDialog::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QString ShowListMessageBaseDialog::roomId() const
{
    return mRoomId;
}
