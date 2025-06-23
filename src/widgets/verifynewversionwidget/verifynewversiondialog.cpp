/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "verifynewversiondialog.h"
using namespace Qt::Literals::StringLiterals;

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
    mainLayout->setObjectName(u"mainLayout"_s);

    mVerifyNewVersionWidget->setObjectName(u"mVerifyNewVersionWidget"_s);
    mainLayout->addWidget(mVerifyNewVersionWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(u"buttonBox"_s);
    mainLayout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &VerifyNewVersionDialog::reject);
    resize(300, 200);
}

VerifyNewVersionDialog::~VerifyNewVersionDialog() = default;

void VerifyNewVersionDialog::checkNewVersion()
{
    mVerifyNewVersionWidget->checkNewVersion();
}

#include "moc_verifynewversiondialog.cpp"
