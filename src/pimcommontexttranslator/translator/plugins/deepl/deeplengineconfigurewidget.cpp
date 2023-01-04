/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "deeplengineconfigurewidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>

DeeplEngineConfigureWidget::DeeplEngineConfigureWidget(QWidget *parent)
    : QWidget{parent}
    , mUseFreeLicense(new QCheckBox(i18n("Use Free License Key"), this))
    , mApiKey(new QLineEdit(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mUseFreeLicense->setObjectName(QStringLiteral("mUseFreeLicense"));
    mainLayout->addWidget(mUseFreeLicense);

    mApiKey->setObjectName(QStringLiteral("mApiKey"));
    mApiKey->setClearButtonEnabled(true);
    mainLayout->addRow(i18n("Api Key:"), mApiKey);
}

DeeplEngineConfigureWidget::~DeeplEngineConfigureWidget() = default;

void DeeplEngineConfigureWidget::setUseFreeLicenceKey(bool b)
{
    mUseFreeLicense->setChecked(b);
}

bool DeeplEngineConfigureWidget::useFreeLicenceKey() const
{
    return mUseFreeLicense->isChecked();
}

QString DeeplEngineConfigureWidget::apiKey() const
{
    return mApiKey->text();
}

void DeeplEngineConfigureWidget::setApiKey(const QString &key)
{
    mApiKey->setText(key);
}
