/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2epasswordwidget.h"
#include <QVBoxLayout>

E2ePasswordWidget::E2ePasswordWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    // TODO
}

E2ePasswordWidget::~E2ePasswordWidget() = default;

#include "moc_e2epasswordwidget.cpp"
