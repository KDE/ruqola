/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "forwardmessagewidget.h"
#include <KLocalizedString>
#include <QVBoxLayout>

ForwardMessageWidget::ForwardMessageWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    // TODO
}

ForwardMessageWidget::~ForwardMessageWidget() = default;

#include "moc_forwardmessagewidget.cpp"
