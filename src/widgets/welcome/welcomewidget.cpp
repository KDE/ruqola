/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "welcomewidget.h"
#include <KLocalizedString>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
WelcomeWidget::WelcomeWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});
}

WelcomeWidget::~WelcomeWidget() = default;
