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

#include "channelpassworddialog.h"
#include "channelpasswordwidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

ChannelPasswordDialog::ChannelPasswordDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Add Password"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mChannelPasswordWidget = new ChannelPasswordWidget(this);
    mChannelPasswordWidget->setObjectName(QStringLiteral("mChannelPasswordWidget"));
    mainLayout->addWidget(mChannelPasswordWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ChannelPasswordDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ChannelPasswordDialog::reject);
    mainLayout->addWidget(buttonBox);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);
    connect(mChannelPasswordWidget, &ChannelPasswordWidget::updateOkButton, this, [=](bool state) {
        okButton->setEnabled(state);
    });
}

ChannelPasswordDialog::~ChannelPasswordDialog()
{
}

QString ChannelPasswordDialog::password() const
{
    return mChannelPasswordWidget->password();
}
