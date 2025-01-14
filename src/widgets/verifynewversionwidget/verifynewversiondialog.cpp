/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "verifynewversiondialog.h"
#include "verifynewversionwidget/verifynewversionwidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

VerifyNewVersionDialog::VerifyNewVersionDialog(QWidget *parent)
    : QDialog(parent)
    , mVerifyNewVersionWidget(new VerifyNewVersionWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Check New Version"));

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mVerifyNewVersionWidget->setObjectName(QStringLiteral("mVerifyNewVersionWidget"));
    mainLayout->addWidget(mVerifyNewVersionWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(QStringLiteral("buttonBox"));
    mainLayout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &VerifyNewVersionDialog::reject);
}

VerifyNewVersionDialog::~VerifyNewVersionDialog() = default;

#include "moc_verifynewversiondialog.cpp"
