/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "featurepreviewwidget.h"

#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
FeaturePreviewWidget::FeaturePreviewWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);
}

FeaturePreviewWidget::~FeaturePreviewWidget() = default;

void FeaturePreviewWidget::initialize()
{
    // TODO
}

#include "moc_featurepreviewwidget.cpp"
