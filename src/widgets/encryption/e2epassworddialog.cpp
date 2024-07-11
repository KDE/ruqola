/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2epassworddialog.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

E2ePasswordDialog::E2ePasswordDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Copy Password"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    // mE2eCopyPasswordWidget->setObjectName(QStringLiteral("mE2eCopyPasswordWidget"));
    // mainLayout->addWidget(mE2eCopyPasswordWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    connect(buttonBox, &QDialogButtonBox::rejected, this, &E2ePasswordDialog::reject);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &E2ePasswordDialog::accept);
    mainLayout->addWidget(buttonBox);
}

E2ePasswordDialog::~E2ePasswordDialog() = default;
