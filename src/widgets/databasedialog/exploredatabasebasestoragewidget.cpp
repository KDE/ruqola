/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "exploredatabasebasestoragewidget.h"
#include "ruqolawidgets_debug.h"
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <KSyntaxHighlighting/Theme>
#include <QPlainTextEdit>
#include <QSplitter>
#include <QTableView>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ExploreDatabaseBaseStorageWidget::ExploreDatabaseBaseStorageWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mTableView(new QTableView(this))
    , mRocketChatAccount(account)
    , mTextEdit(new QPlainTextEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto splitter = new QSplitter(this);
    splitter->setObjectName(u"splitter"_s);
    mainLayout->addWidget(splitter);

    mTableView->setObjectName(u"mTableView"_s);
    mTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mTableView->setSortingEnabled(true);

    mTextEdit->setReadOnly(true);

    splitter->addWidget(mTableView);
    splitter->addWidget(mTextEdit);
    splitter->setChildrenCollapsible(false);

    const KSyntaxHighlighting::Definition def = mRepo.definitionForName(u"Json"_s);
    if (!def.isValid()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Invalid definition name";
    }

    auto hl = new KSyntaxHighlighting::SyntaxHighlighter(mTextEdit->document());
    hl->setTheme((palette().color(QPalette::Base).lightness() < 128) ? mRepo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                                                                     : mRepo.defaultTheme(KSyntaxHighlighting::Repository::LightTheme));
    hl->setDefinition(def);

    connect(mTableView, &QTableView::clicked, this, &ExploreDatabaseBaseStorageWidget::slotCellClicked);
}

ExploreDatabaseBaseStorageWidget::~ExploreDatabaseBaseStorageWidget() = default;

void ExploreDatabaseBaseStorageWidget::slotCellClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        mTextEdit->setPlainText(index.data().toString());
    } else {
        mTextEdit->clear();
    }
}

#include "moc_exploredatabasebasestoragewidget.cpp"
