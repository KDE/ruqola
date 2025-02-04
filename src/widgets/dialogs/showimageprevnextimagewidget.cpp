/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "showimageprevnextimagewidget.h"
#include <QHBoxLayout>

ShowImagePrevNextImageWidget::ShowImagePrevNextImageWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
}

ShowImagePrevNextImageWidget::~ShowImagePrevNextImageWidget() = default;

#include "moc_showimageprevnextimagewidget.cpp"
