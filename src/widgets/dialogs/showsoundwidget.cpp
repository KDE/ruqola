/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "showsoundwidget.h"
#include <QHBoxLayout>

ShowSoundWidget::ShowSoundWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    // TODO
}

ShowSoundWidget::~ShowSoundWidget()
{
}
