/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

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
    , mOnlyUnReadCheckBox(new QCheckBox(i18n("Show Unread"), this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mSearchLineEdit->setPlaceholderText(i18n("Search..."));
    mSearchLineEdit->setClearButtonEnabled(true);
    mainLayout->addWidget(mSearchLineEdit);
    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &BannerInfoListSearchLineWidget::filterChanged);

    mOnlyUnReadCheckBox->setObjectName(QStringLiteral("mOnlyUnReadCheckBox"));
    mainLayout->addWidget(mOnlyUnReadCheckBox);
    connect(mOnlyUnReadCheckBox, &QCheckBox::clicked, this, &BannerInfoListSearchLineWidget::filterChanged);
}

BannerInfoListSearchLineWidget::~BannerInfoListSearchLineWidget() = default;

QString BannerInfoListSearchLineWidget::searchText() const
{
    return mSearchLineEdit->text();
}

bool BannerInfoListSearchLineWidget::showOnlyUnread() const
{
    return mOnlyUnReadCheckBox->isChecked();
}
