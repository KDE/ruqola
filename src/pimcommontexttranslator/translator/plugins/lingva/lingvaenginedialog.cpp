/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "lingvaenginedialog.h"
#include "lingvaenginewidget.h"

#include <QDialogButtonBox>
#include <QVBoxLayout>

LingvaEngineDialog::LingvaEngineDialog(QWidget *parent)
    : QDialog(parent)
    , mConfigureWidget(new LingvaEngineWidget(this))
{
    mConfigureWidget->setObjectName(QStringLiteral("mConfigureWidget"));

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->addWidget(mConfigureWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("buttonBox"));
    mainLayout->addWidget(buttonBox);
}

LingvaEngineDialog::~LingvaEngineDialog() = default;
