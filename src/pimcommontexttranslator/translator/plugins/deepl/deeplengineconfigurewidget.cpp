/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "deeplengineconfigurewidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>

DeeplEngineConfigureWidget::DeeplEngineConfigureWidget(QWidget *parent)
    : QWidget{parent}
    , mUseFreeLicense(new QCheckBox(i18n("Use Free License Key"), this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mUseFreeLicense->setObjectName(QStringLiteral("mUseFreeLicense"));
    mainLayout->addWidget(mUseFreeLicense);
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
