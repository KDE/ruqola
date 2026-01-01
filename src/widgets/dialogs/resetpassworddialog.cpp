/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resetpassworddialog.h"
using namespace Qt::Literals::StringLiterals;

#include "resetpasswordwidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

ResetPasswordDialog::ResetPasswordDialog(QWidget *parent)
    : QDialog(parent)
    , mResetPasswordWidget(new ResetPasswordWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Reset Password"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mResetPasswordWidget->setObjectName(u"mResetPasswordWidget"_s);
    mainLayout->addWidget(mResetPasswordWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::accepted, this, &ResetPasswordDialog::accept);
    connect(button, &QDialogButtonBox::rejected, this, &ResetPasswordDialog::reject);

    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);
    connect(mResetPasswordWidget, &ResetPasswordWidget::updateOkButton, okButton, &QPushButton::setEnabled);
}

ResetPasswordDialog::~ResetPasswordDialog() = default;

QString ResetPasswordDialog::email() const
{
    return mResetPasswordWidget->email();
}

#include "moc_resetpassworddialog.cpp"
