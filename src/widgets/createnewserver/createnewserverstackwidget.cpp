/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewserverstackwidget.h"
#include "createnewservercheckurlwidget.h"
#include "createnewserverwidget.h"

CreateNewServerStackWidget::CreateNewServerStackWidget(QWidget *parent)
    : QStackedWidget(parent)
    , mCreateNewServerCheckUrlWidget(new CreateNewServerCheckUrlWidget(this))
    , mCreateNewServerWidget(new CreateNewServerWidget(this))
{
    mCreateNewServerCheckUrlWidget->setObjectName(QStringLiteral("mCreateNewServerCheckUrlWidget"));
    addWidget(mCreateNewServerCheckUrlWidget);

    mCreateNewServerWidget->setObjectName(QStringLiteral("mCreateNewServerWidget"));
    addWidget(mCreateNewServerWidget);
    setCurrentWidget(mCreateNewServerCheckUrlWidget);
}

CreateNewServerStackWidget::~CreateNewServerStackWidget() = default;

#include "moc_createnewserverstackwidget.cpp"
