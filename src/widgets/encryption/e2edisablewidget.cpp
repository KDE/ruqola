/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2edisablewidget.h"
#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>

E2eDisableWidget::E2eDisableWidget(QWidget *parent)
    : QWidget{parent}
{
}

E2eDisableWidget::~E2eDisableWidget() = default;

#include "moc_e2edisablewidget.cpp"
