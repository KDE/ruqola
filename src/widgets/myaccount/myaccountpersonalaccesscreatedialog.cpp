/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountpersonalaccesscreatedialog.h"

#include "myaccountpersonalaccesscreatewidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
MyAccountPersonalAccessCreateDialog::MyAccountPersonalAccessCreateDialog(QWidget *parent)
    : QDialog(parent)
    , mCreateWidget(new MyAccountPersonalAccessCreateWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Create Personal Token"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mCreateWidget->setObjectName(u"mCreateWidget"_s);
    mainLayout->addWidget(mCreateWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &MyAccountPersonalAccessCreateDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &MyAccountPersonalAccessCreateDialog::accept);

    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    okButton->setText(i18nc("@action:button", "Create"));
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

void MyAccountPersonalAccessCreateDialog::setExistingTokenNames(const QStringList &lst)
{
    mCreateWidget->setExistingTokenNames(lst);
}

#include "moc_myaccountpersonalaccesscreatedialog.cpp"
