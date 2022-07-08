/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfolistsearchlinewidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLineEdit>

BannerInfoListSearchLineWidget::BannerInfoListSearchLineWidget(QWidget *parent)
    : QWidget{parent}
    , mSearchLineEdit(new QLineEdit(this))
    , mUnrealCheckBox(new QCheckBox(i18n("Show Unread"), this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mainLayout->addWidget(mSearchLineEdit);

    mUnrealCheckBox->setObjectName(QStringLiteral("mUnrealCheckBox"));
    mainLayout->addWidget(mUnrealCheckBox);
}

BannerInfoListSearchLineWidget::~BannerInfoListSearchLineWidget() = default;

QString BannerInfoListSearchLineWidget::searchText() const
{
    return mSearchLineEdit->text();
}
