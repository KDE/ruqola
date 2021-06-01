/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "searchtreebasewidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "misc/searchwithdelaylineedit.h"

#include <QHeaderView>
#include <QLabel>
#include <QTreeView>
#include <QVBoxLayout>

SearchTreeBaseWidget::SearchTreeBaseWidget(QWidget *parent)
    : QWidget(parent)
    , mLabelResultSearch(new QLabel(this))
    , mSearchLineEdit(new SearchWithDelayLineEdit(this))
    , mTreeView(new QTreeView(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mainLayout->addWidget(mSearchLineEdit);
    mSearchLineEdit->setDelayMs(500);
    new LineEditCatchReturnKey(mSearchLineEdit, this);

    mLabelResultSearch->setObjectName(QStringLiteral("mLabelResultSearch"));
    mainLayout->addWidget(mLabelResultSearch);
    mLabelResultSearch->setTextFormat(Qt::RichText);
    mLabelResultSearch->setContextMenuPolicy(Qt::NoContextMenu);
    QFont labFont = mLabelResultSearch->font();
    labFont.setBold(true);
    mLabelResultSearch->setFont(labFont);
    // connect(mLabelResultSearch, &QLabel::linkActivated, this, &SearchTreeBaseWidget::loadMoreElements);
    mTreeView->setObjectName(QStringLiteral("mTreeView"));
    mTreeView->setRootIsDecorated(false);
    mTreeView->setSortingEnabled(true);
    mTreeView->sortByColumn(0, Qt::AscendingOrder);
    mTreeView->header()->setSectionsClickable(true);
    mTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    // connect(mTreeView, &QTreeView::customContextMenuRequested, this, &DirectoryWidget::slotCustomContextMenuRequested);

    mainLayout->addWidget(mTreeView);
}

SearchTreeBaseWidget::~SearchTreeBaseWidget()
{
}
