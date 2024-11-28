/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appscountinfowidget.h"
#include <KLocalizedString>
#include <QHBoxLayout>
using namespace Qt::Literals::StringLiterals;
AppsCountInfoWidget::AppsCountInfoWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});
}

void AppsCountInfoWidget::setAppCountInfo(const AppsCountInfo &info)
{
    // TODO
}

AppsCountInfoWidget::~AppsCountInfoWidget() = default;

#include "moc_appscountinfowidget.cpp"
