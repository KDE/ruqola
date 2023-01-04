/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "libretranslateengineconfiguredialog.h"
#include "libretranslateengineconfigurewidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

LibreTranslateEngineConfigureDialog::LibreTranslateEngineConfigureDialog(QWidget *parent)
    : QDialog(parent)
    , mConfigureWidget(new LibreTranslateEngineConfigureWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Configure Engine"));

    mConfigureWidget->setObjectName(QStringLiteral("mConfigureWidget"));

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->addWidget(mConfigureWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("buttonBox"));
    mainLayout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &LibreTranslateEngineConfigureDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &LibreTranslateEngineConfigureDialog::reject);
}

LibreTranslateEngineConfigureDialog::~LibreTranslateEngineConfigureDialog() = default;

QString LibreTranslateEngineConfigureDialog::serverUrl() const
{
    return mConfigureWidget->serverUrl();
}

void LibreTranslateEngineConfigureDialog::setServerUrl(const QString &serverUrl)
{
    mConfigureWidget->setServerUrl(serverUrl);
}

QString LibreTranslateEngineConfigureDialog::apiKey() const
{
    return mConfigureWidget->apiKey();
}

void LibreTranslateEngineConfigureDialog::setApiKey(const QString &key)
{
    mConfigureWidget->setApiKey(key);
}

bool LibreTranslateEngineConfigureDialog::serverRequiredApiKey() const
{
    return mConfigureWidget->serverRequiredApiKey();
}

void LibreTranslateEngineConfigureDialog::setServerRequiredApiKey(bool state)
{
    mConfigureWidget->setServerRequiredApiKey(state);
}
