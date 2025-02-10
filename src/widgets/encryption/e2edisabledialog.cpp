/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "e2edisabledialog.h"
#include "e2edisablewidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

E2eDisableDialog::E2eDisableDialog(QWidget *parent)
    : QDialog(parent)
    , mE2eDisableWidget(new E2eDisableWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Disable Encryption"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mE2eDisableWidget->setObjectName(QStringLiteral("mE2eDisableWidget"));
    mainLayout->addWidget(mE2eDisableWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    connect(buttonBox, &QDialogButtonBox::accepted, this, &E2eDisableDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &E2eDisableDialog::reject);
    mainLayout->addWidget(buttonBox);
}

E2eDisableDialog::~E2eDisableDialog() = default;

#include "moc_e2edisabledialog.cpp"
