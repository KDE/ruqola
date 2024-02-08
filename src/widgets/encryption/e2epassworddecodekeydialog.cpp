/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2epassworddecodekeydialog.h"
#include "e2epassworddecodekeywidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

E2ePasswordDecodeKeyDialog::E2ePasswordDecodeKeyDialog(QWidget *parent)
    : QDialog(parent)
    , mE2ePasswordDecodeKeyWidget(new E2ePasswordDecodeKeyWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Decode Key"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mE2ePasswordDecodeKeyWidget->setObjectName(QStringLiteral("mE2ePasswordDecodeKeyWidget"));
    mainLayout->addWidget(mE2ePasswordDecodeKeyWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    connect(buttonBox, &QDialogButtonBox::accepted, this, &E2ePasswordDecodeKeyDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &E2ePasswordDecodeKeyDialog::reject);
    mainLayout->addWidget(buttonBox);
}

E2ePasswordDecodeKeyDialog::~E2ePasswordDecodeKeyDialog() = default;

QString E2ePasswordDecodeKeyDialog::password() const
{
    return mE2ePasswordDecodeKeyWidget->password();
}

#include "moc_e2epassworddecodekeydialog.cpp"
