/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "exploredatabasebasestoragewidget.h"
#include "databasedialog/exploredatabasejsonplaintexteditwidget.h"
#include <KLineEditEventHandler>
#include <QLineEdit>
#include <QSplitter>
#include <QTableView>
#include <QVBoxLayout>
#include <qsortfilterproxymodel.h>

using namespace Qt::Literals::StringLiterals;
ExploreDatabaseBaseStorageWidget::ExploreDatabaseBaseStorageWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mTableView(new QTableView(this))
    , mRocketChatAccount(account)
    , mTextEdit(new ExploreDatabaseJsonPlainTextEditWidget(this))
    , mFilterLineEdit(new QLineEdit(this))
    , mSortFilterProxyModel(new QSortFilterProxyModel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto splitter = new QSplitter(this);
    splitter->setObjectName(u"splitter"_s);
    mainLayout->addWidget(splitter);

    auto tableWidget = new QWidget(this);
    auto tableLayout = new QVBoxLayout(tableWidget);
    tableLayout->setObjectName(u"tableLayout"_s);
    tableLayout->setContentsMargins({});

    KLineEditEventHandler::catchReturnKey(mTextEdit);
    mTableView->setObjectName(u"mTableView"_s);
    mTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mTableView->setSortingEnabled(true);
    tableLayout->addWidget(mFilterLineEdit);
    tableLayout->addWidget(mTableView);
    mTableView->setModel(mSortFilterProxyModel);
    mSortFilterProxyModel->setFilterKeyColumn(-1); // don't select specific column
    mSortFilterProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    mFilterLineEdit->setClearButtonEnabled(true);
    connect(mFilterLineEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        mSortFilterProxyModel->setFilterFixedString(text);
    });

    splitter->addWidget(tableWidget);
    splitter->addWidget(mTextEdit);
    splitter->setChildrenCollapsible(false);

    connect(mTableView, &QTableView::clicked, this, &ExploreDatabaseBaseStorageWidget::slotCellClicked);
}

ExploreDatabaseBaseStorageWidget::~ExploreDatabaseBaseStorageWidget() = default;

void ExploreDatabaseBaseStorageWidget::slotCellClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        mTextEdit->setPlainText(index.data().toString());
    } else {
        mTextEdit->setPlainText({});
    }
}

void ExploreDatabaseBaseStorageWidget::setModel(QAbstractItemModel *model)
{
    mSortFilterProxyModel->setSourceModel(model);
}

#include "moc_exploredatabasebasestoragewidget.cpp"
