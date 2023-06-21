/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelpassworddialog.h"
#include "channelpasswordwidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

ChannelPasswordDialog::ChannelPasswordDialog(QWidget *parent)
    : QDialog(parent)
    , mChannelPasswordWidget(new ChannelPasswordWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Add Password"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mChannelPasswordWidget->setObjectName(QStringLiteral("mChannelPasswordWidget"));
    mainLayout->addWidget(mChannelPasswordWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
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

ChannelPasswordDialog::~ChannelPasswordDialog() = default;

QString ChannelPasswordDialog::password() const
{
    return mChannelPasswordWidget->password();
}

#include "moc_channelpassworddialog.cpp"
