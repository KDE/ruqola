/*
  SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2ecopypassworddialog.h"
using namespace Qt::Literals::StringLiterals;

#include "e2ecopypasswordwidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

E2eCopyPasswordDialog::E2eCopyPasswordDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mE2eCopyPasswordWidget(new E2eCopyPasswordWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Copy Password"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mE2eCopyPasswordWidget->setObjectName(u"mE2eCopyPasswordWidget"_s);
    mainLayout->addWidget(mE2eCopyPasswordWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, this);
    buttonBox->setObjectName(u"button"_s);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &E2eCopyPasswordDialog::reject);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &E2eCopyPasswordDialog::accept);
    mainLayout->addWidget(buttonBox);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setText(i18nc("@action:button", "I saved my Password"));

    QPushButton *cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
    cancelButton->setText(i18nc("@action:button", "Do it later"));
}

E2eCopyPasswordDialog::~E2eCopyPasswordDialog() = default;

#include "moc_e2ecopypassworddialog.cpp"
