/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resetpassworddialog.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

ResetPasswordDialog::ResetPasswordDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Reset Password"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    // mReportMessageWidget->setObjectName(QStringLiteral("mReportMessageWidget"));
    // mainLayout->addWidget(mReportMessageWidget);
    // mainLayout->addStretch(1);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::accepted, this, &ResetPasswordDialog::accept);
    connect(button, &QDialogButtonBox::rejected, this, &ResetPasswordDialog::reject);

    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);
    // connect(mReportMessageWidget, &ReportMessageWidget::updateOkButton, okButton, &QPushButton::setEnabled);
}

ResetPasswordDialog::~ResetPasswordDialog() = default;

#include "moc_resetpassworddialog.cpp"
