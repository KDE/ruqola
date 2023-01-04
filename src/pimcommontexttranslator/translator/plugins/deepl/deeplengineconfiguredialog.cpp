/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "deeplengineconfiguredialog.h"
#include "deeplengineconfigurewidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

DeeplEngineConfigureDialog::DeeplEngineConfigureDialog(QWidget *parent)
    : QDialog(parent)
    , mConfigureWidget(new DeeplEngineConfigureWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Configure Engine"));
    mConfigureWidget->setObjectName(QStringLiteral("mConfigureWidget"));

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->addWidget(mConfigureWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("buttonBox"));
    mainLayout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &DeeplEngineConfigureDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &DeeplEngineConfigureDialog::reject);
}

DeeplEngineConfigureDialog::~DeeplEngineConfigureDialog() = default;

void DeeplEngineConfigureDialog::setUseFreeLicenceKey(bool b)
{
    mConfigureWidget->setUseFreeLicenceKey(b);
}

bool DeeplEngineConfigureDialog::useFreeLicenceKey() const
{
    return mConfigureWidget->useFreeLicenceKey();
}

QString DeeplEngineConfigureDialog::apiKey() const
{
    return mConfigureWidget->apiKey();
}

void DeeplEngineConfigureDialog::setApiKey(const QString &key)
{
    mConfigureWidget->setApiKey(key);
}
