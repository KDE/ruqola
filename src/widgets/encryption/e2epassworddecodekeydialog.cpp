/*
  SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2epassworddecodekeydialog.h"
using namespace Qt::Literals::StringLiterals;

#include "e2epassworddecodekeywidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

E2ePasswordDecodeKeyDialog::E2ePasswordDecodeKeyDialog(QWidget *parent)
    : QDialog(parent)
    , mE2ePasswordDecodeKeyWidget(new E2ePasswordDecodeKeyWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Decode Key"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mE2ePasswordDecodeKeyWidget->setObjectName(u"mE2ePasswordDecodeKeyWidget"_s);
    mainLayout->addWidget(mE2ePasswordDecodeKeyWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(u"button"_s);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &E2ePasswordDecodeKeyDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &E2ePasswordDecodeKeyDialog::reject);
    mainLayout->addWidget(buttonBox);
    buttonBox->button(QDialogButtonBox::Ok)->setText(i18nc("@action:button", "Enable Encryption"));
    buttonBox->button(QDialogButtonBox::Cancel)->setText(i18nc("@action:button", "Do it later"));
}

E2ePasswordDecodeKeyDialog::~E2ePasswordDecodeKeyDialog() = default;

QString E2ePasswordDecodeKeyDialog::password() const
{
    return mE2ePasswordDecodeKeyWidget->password();
}

#include "moc_e2epassworddecodekeydialog.cpp"
