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

#include "directchannelinfodialog.h"
#include "directchannelinfowidget.h"
#include <KLocalizedString>
#include <QVBoxLayout>
#include <QDialogButtonBox>

DirectChannelInfoDialog::DirectChannelInfoDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Users Info"));
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mDirectChannelInfoWidget = new DirectChannelInfoWidget(this);
    mDirectChannelInfoWidget->setObjectName(QStringLiteral("mDirectChannelInfoWidget"));
    mainLayout->addWidget(mDirectChannelInfoWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(QStringLiteral("buttonBox"));
    connect(buttonBox, &QDialogButtonBox::rejected, this, &DirectChannelInfoDialog::reject);
    mainLayout->addWidget(buttonBox);
}

DirectChannelInfoDialog::~DirectChannelInfoDialog()
{
}
