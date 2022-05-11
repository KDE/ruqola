/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "encryptionsettingswidget.h"

#include <QFormLayout>

EncryptionSettingsWidget::EncryptionSettingsWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
}

EncryptionSettingsWidget::~EncryptionSettingsWidget() = default;

void EncryptionSettingsWidget::initialize()
{
    // TODO
}
