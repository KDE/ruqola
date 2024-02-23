/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokenauthenticationconfigdialog.h"
#include "personalaccesstokenauthenticationconfigwidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

PersonalAccessTokenAuthenticationConfigDialog::PersonalAccessTokenAuthenticationConfigDialog(QWidget *parent)
    : QDialog(parent)
    , mPersonalAccessTokenAuthenticationConfigWidget(new PersonalAccessTokenAuthenticationConfigWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Configure"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mPersonalAccessTokenAuthenticationConfigWidget->setObjectName(QStringLiteral("mPersonalAccessTokenAuthenticationConfigWidget"));
    mainLayout->addWidget(mPersonalAccessTokenAuthenticationConfigWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &PersonalAccessTokenAuthenticationConfigDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &PersonalAccessTokenAuthenticationConfigDialog::accept);
    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    okButton->setObjectName(QStringLiteral("okButton"));
    okButton->setEnabled(false);
    connect(mPersonalAccessTokenAuthenticationConfigWidget, &PersonalAccessTokenAuthenticationConfigWidget::enableOkButton, okButton, &QPushButton::setEnabled);
}

PersonalAccessTokenAuthenticationConfigDialog::~PersonalAccessTokenAuthenticationConfigDialog() = default;

#include "moc_personalaccesstokenauthenticationconfigdialog.cpp"
