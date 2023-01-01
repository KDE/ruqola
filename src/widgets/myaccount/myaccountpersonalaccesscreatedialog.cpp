/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountpersonalaccesscreatedialog.h"
#include "myaccountpersonalaccesscreatewidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

MyAccountPersonalAccessCreateDialog::MyAccountPersonalAccessCreateDialog(QWidget *parent)
    : QDialog(parent)
    , mCreateWidget(new MyAccountPersonalAccessCreateWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Create Personal Token"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mCreateWidget->setObjectName(QStringLiteral("mCreateWidget"));
    mainLayout->addWidget(mCreateWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &MyAccountPersonalAccessCreateDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &MyAccountPersonalAccessCreateDialog::accept);

    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    okButton->setText(i18n("Create"));
    okButton->setEnabled(false);
    connect(mCreateWidget, &MyAccountPersonalAccessCreateWidget::updateOkButton, okButton, &QPushButton::setEnabled);
}

MyAccountPersonalAccessCreateDialog::~MyAccountPersonalAccessCreateDialog() = default;

bool MyAccountPersonalAccessCreateDialog::bypassTwoFactor() const
{
    return mCreateWidget->bypassTwoFactor();
}

QString MyAccountPersonalAccessCreateDialog::tokenName() const
{
    return mCreateWidget->tokenName();
}
