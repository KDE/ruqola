/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "featurepreviewmessagewidget.h"
#include <KLocalizedString>

FeaturePreviewMessageWidget::FeaturePreviewMessageWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setMessageType(KMessageWidget::Information);
    setPosition(KMessageWidget::Footer);
    hide();
}

FeaturePreviewMessageWidget::~FeaturePreviewMessageWidget() = default;
#include "moc_featurepreviewmessagewidget.cpp"
