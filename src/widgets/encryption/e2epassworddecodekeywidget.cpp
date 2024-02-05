/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2epassworddecodekeywidget.h"
#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>

E2ePasswordDecodeKeyWidget::E2ePasswordDecodeKeyWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
}

E2ePasswordDecodeKeyWidget::~E2ePasswordDecodeKeyWidget() = default;

#include "moc_e2epassworddecodekeywidget.cpp"
