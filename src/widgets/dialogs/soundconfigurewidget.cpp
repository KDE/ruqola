/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "soundconfigurewidget.h"
#include <QHBoxLayout>

SoundConfigureWidget::SoundConfigureWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});
}

SoundConfigureWidget::~SoundConfigureWidget() = default;
