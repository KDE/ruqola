/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorycontainerwidget.h"
#include <QVBoxLayout>
DirectoryContainerWidget::DirectoryContainerWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
}

DirectoryContainerWidget::~DirectoryContainerWidget() = default;
