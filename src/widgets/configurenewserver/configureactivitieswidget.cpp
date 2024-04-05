/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureactivitieswidget.h"
#include <QListView>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;

ConfigureActivitiesWidget::ConfigureActivitiesWidget(QWidget *parent)
    : QWidget{parent}
    , mListView(new QListView(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);

    mListView->setObjectName("mListView"_L1);
    mainLayout->addWidget(mListView);
}

ConfigureActivitiesWidget::~ConfigureActivitiesWidget() = default;

QStringList ConfigureActivitiesWidget::activities() const
{
    // TODO
    return {};
}
