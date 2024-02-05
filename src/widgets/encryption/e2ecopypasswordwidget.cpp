/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2ecopypasswordwidget.h"
#include <KLocalizedString>
#include <QVBoxLayout>

E2eCopyPasswordWidget::E2eCopyPasswordWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
}

E2eCopyPasswordWidget::~E2eCopyPasswordWidget() = default;

#include "moc_e2ecopypasswordwidget.cpp"
