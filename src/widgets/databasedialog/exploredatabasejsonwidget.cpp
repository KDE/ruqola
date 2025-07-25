/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabasejsonwidget.h"
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
ExploreDatabaseJsonWidget::ExploreDatabaseJsonWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
}

ExploreDatabaseJsonWidget::~ExploreDatabaseJsonWidget() = default;

#include "moc_exploredatabasejsonwidget.cpp"
