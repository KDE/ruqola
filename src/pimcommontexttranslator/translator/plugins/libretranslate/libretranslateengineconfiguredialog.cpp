/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

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
    mConfigureWidget->setObjectName(QStringLiteral("mConfigureWidget"));

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->addWidget(mConfigureWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("buttonBox"));
    mainLayout->addWidget(buttonBox);
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
