/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2ecopypassworddialog.h"
#include "e2ecopypasswordwidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

E2eCopyPasswordDialog::E2eCopyPasswordDialog(QWidget *parent)
    : QDialog(parent)
    , mE2eCopyPasswordWidget(new E2eCopyPasswordWidget(this))

{
    setWindowTitle(i18nc("@title:window", "Copy Password"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mE2eCopyPasswordWidget->setObjectName(QStringLiteral("mE2eCopyPasswordWidget"));
    mainLayout->addWidget(mE2eCopyPasswordWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    connect(buttonBox, &QDialogButtonBox::accepted, this, &E2eCopyPasswordDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &E2eCopyPasswordDialog::reject);
    mainLayout->addWidget(buttonBox);
}

E2eCopyPasswordDialog::~E2eCopyPasswordDialog() = default;

#include "moc_e2ecopypassworddialog.cpp"
