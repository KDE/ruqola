/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2edisablewidget.h"
#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
E2eDisableWidget::E2eDisableWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});
}

E2eDisableWidget::~E2eDisableWidget() = default;

#include "moc_e2edisablewidget.cpp"
