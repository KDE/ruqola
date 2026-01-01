/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "confirmpassworddialog.h"
using namespace Qt::Literals::StringLiterals;

#include "confirmpasswordwidget.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

ConfirmPasswordDialog::ConfirmPasswordDialog(QWidget *parent)
    : QDialog(parent)
    , mConfirmPasswordWidget(new ConfirmPasswordWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Confirm Password"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mConfirmPasswordWidget->setObjectName(u"mConfirmPasswordWidget"_s);
    mainLayout->addWidget(mConfirmPasswordWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(u"buttonBox"_s);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ConfirmPasswordDialog::reject);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ConfirmPasswordDialog::accept);
    mainLayout->addWidget(buttonBox);
    auto okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);
    connect(mConfirmPasswordWidget, &ConfirmPasswordWidget::enabledOkButton, okButton, &QPushButton::setEnabled);
}

ConfirmPasswordDialog::~ConfirmPasswordDialog() = default;

QString ConfirmPasswordDialog::password() const
{
    return mConfirmPasswordWidget->password();
}

#include "moc_confirmpassworddialog.cpp"
