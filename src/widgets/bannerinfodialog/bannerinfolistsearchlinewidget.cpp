/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfolistsearchlinewidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLineEdit>

BannerInfoListSearchLineWidget::BannerInfoListSearchLineWidget(QWidget *parent)
    : QWidget{parent}
    , mSearchLineEdit(new QLineEdit(this))
    , mOnlyUnReadCheckBox(new QCheckBox(i18nc("@option:check", "Show Unread"), this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(u"mainLayout"_s);

    mSearchLineEdit->setObjectName(u"mSearchLineEdit"_s);
    mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search…"));
    mSearchLineEdit->setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(mSearchLineEdit);
    mainLayout->addWidget(mSearchLineEdit);
    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &BannerInfoListSearchLineWidget::filterChanged);

    mOnlyUnReadCheckBox->setObjectName(u"mOnlyUnReadCheckBox"_s);
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

#include "moc_bannerinfolistsearchlinewidget.cpp"
