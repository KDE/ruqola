/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfolistsearchlinewidget.h"
#include <QHBoxLayout>
#include <QLineEdit>

BannerInfoListSearchLineWidget::BannerInfoListSearchLineWidget(QWidget *parent)
    : QWidget{parent}
    , mSearchLineEdit(new QLineEdit(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mainLayout->addWidget(mSearchLineEdit);
    // TODO add combobox or checkbox
}

BannerInfoListSearchLineWidget::~BannerInfoListSearchLineWidget() = default;
