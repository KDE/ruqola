/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appcountinfowidget.h"
#include <KLocalizedString>
#include <QHBoxLayout>
using namespace Qt::Literals::StringLiterals;
AppCountInfoWidget::AppCountInfoWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});
}

void AppCountInfoWidget::setAppCountInfo(const AppsCountInfo &info)
{
    // TODO
}

AppCountInfoWidget::~AppCountInfoWidget() = default;

#include "moc_appcountinfowidget.cpp"
