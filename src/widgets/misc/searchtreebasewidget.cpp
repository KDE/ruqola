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
#include "model/directorybasemodel.h"

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
    connect(mLabelResultSearch, &QLabel::linkActivated, this, &SearchTreeBaseWidget::loadMoreElements);
    mTreeView->setObjectName(QStringLiteral("mTreeView"));
    mTreeView->setRootIsDecorated(false);
    mTreeView->setSortingEnabled(true);
    mTreeView->sortByColumn(0, Qt::AscendingOrder);
    mTreeView->header()->setSectionsClickable(true);
    mTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    mainLayout->addWidget(mTreeView);
    connect(mSearchLineEdit, &SearchWithDelayLineEdit::searchCleared, this, &SearchTreeBaseWidget::slotSearchCleared);
    connect(mSearchLineEdit, &SearchWithDelayLineEdit::searchRequested, this, &SearchTreeBaseWidget::slotSearchRequested);
    connect(this, &SearchTreeBaseWidget::loadMoreElements, this, [this]() {
        slotLoadMoreElements();
    });
    connect(mTreeView, &QTreeView::customContextMenuRequested, this, &SearchTreeBaseWidget::slotCustomContextMenuRequested);
}

SearchTreeBaseWidget::~SearchTreeBaseWidget()
{
}

void SearchTreeBaseWidget::slotLoadMoreElements()
{
    if (!mModel->loadMoreInProgress()) {
        const int offset = mModel->rowCount();
        if (offset < mModel->total()) {
            mModel->setLoadMoreInProgress(true);
            slotLoadElements(offset, qMin(50, mModel->total() - offset), mSearchLineEdit->text().trimmed());
        }
    }
}

void SearchTreeBaseWidget::connectModel()
{
    connect(mModel, &DirectoryBaseModel::hasFullListChanged, this, &SearchTreeBaseWidget::updateLabel);
    connect(mModel, &DirectoryBaseModel::totalChanged, this, &SearchTreeBaseWidget::updateLabel);
    connect(mModel, &DirectoryBaseModel::loadingInProgressChanged, this, &SearchTreeBaseWidget::updateLabel);
}

void SearchTreeBaseWidget::finishSearching()
{
    mModel->setLoadMoreInProgress(false);
    mTreeView->header()->resizeSections(QHeaderView::ResizeToContents);
}

void SearchTreeBaseWidget::slotLoadMoreElementDone(const QJsonObject &obj)
{
    mModel->addMoreElements(obj);
    finishSearching();
}

void SearchTreeBaseWidget::slotSearchDone(const QJsonObject &obj)
{
    mModel->parseElements(obj);
    finishSearching();
}

void SearchTreeBaseWidget::slotSearchCleared()
{
    slotLoadElements();
}

void SearchTreeBaseWidget::slotSearchRequested(const QString &str)
{
    slotLoadElements(-1, -1, str);
}

void SearchTreeBaseWidget::initialize()
{
    mModel->initialize();
    slotLoadElements();
}
