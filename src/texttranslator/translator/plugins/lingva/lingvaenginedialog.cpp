/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "lingvaenginedialog.h"
#include "lingvaenginewidget.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

LingvaEngineDialog::LingvaEngineDialog(QWidget *parent)
    : QDialog(parent)
    , mConfigureWidget(new LingvaEngineWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Configure Engine"));
    mConfigureWidget->setObjectName(QStringLiteral("mConfigureWidget"));

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->addWidget(mConfigureWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("buttonBox"));
    mainLayout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &LingvaEngineDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &LingvaEngineDialog::reject);
}

LingvaEngineDialog::~LingvaEngineDialog() = default;

QString LingvaEngineDialog::serverUrl() const
{
    return mConfigureWidget->serverUrl();
}

void LingvaEngineDialog::setServerUrl(const QString &serverUrl)
{
    mConfigureWidget->setServerUrl(serverUrl);
}
