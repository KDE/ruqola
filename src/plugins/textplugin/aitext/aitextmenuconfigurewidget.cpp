/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "aitextmenuconfigurewidget.h"
#include <QVBoxLayout>

AiTextMenuConfigureWidget::AiTextMenuConfigureWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));
    mainLayout->setContentsMargins({});
}

AiTextMenuConfigureWidget::~AiTextMenuConfigureWidget() = default;

#include "moc_aitextmenuconfigurewidget.cpp"
