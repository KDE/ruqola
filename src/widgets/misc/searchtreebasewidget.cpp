/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchtreebasewidget.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/searchwithdelaylineedit.h"
#include "model/custombasemodel.h"
#include <KLineEditEventHandler>

#include <KLocalizedString>
#include <QHeaderView>
#include <QLabel>
#include <QTreeView>
#include <QVBoxLayout>

SearchTreeBaseWidget::SearchTreeBaseWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mLabelResultSearch(new QLabel(this))
    , mSearchLineEdit(new SearchWithDelayLineEdit(this))
    , mTreeView(new QTreeView(this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mSearchLayout = new QVBoxLayout;
    mainLayout->addLayout(mSearchLayout);

    mSearchLineLayout = new QHBoxLayout;
    mSearchLineLayout->setContentsMargins({});

    mSearchLineEdit->setObjectName(u"mSearchLineEdit"_s);
    mSearchLineLayout->addWidget(mSearchLineEdit);
    mSearchLineEdit->setDelayMs(500ms);
    KLineEditEventHandler::catchReturnKey(mSearchLineEdit);

    mSearchLayout->addLayout(mSearchLineLayout);

    mLabelResultSearch->setObjectName(u"mLabelResultSearch"_s);
    mSearchLayout->addWidget(mLabelResultSearch);
    mLabelResultSearch->setTextFormat(Qt::RichText);
    mLabelResultSearch->setContextMenuPolicy(Qt::NoContextMenu);
    QFont labFont = mLabelResultSearch->font();
    labFont.setBold(true);
    mLabelResultSearch->setFont(labFont);
    connect(mLabelResultSearch, &QLabel::linkActivated, this, &SearchTreeBaseWidget::loadMoreElements);

    auto treeViewLayout = new QVBoxLayout;
    treeViewLayout->setContentsMargins({});
    mainLayout->addLayout(treeViewLayout);

    mTreeView->setObjectName(u"mTreeView"_s);
    mTreeView->setProperty("_breeze_borders_sides", QVariant::fromValue(QFlags{Qt::TopEdge}));

    mTreeView->setRootIsDecorated(false);
    mTreeView->setSortingEnabled(true);
    mTreeView->sortByColumn(0, Qt::AscendingOrder);
    mTreeView->header()->setSectionsClickable(true);
    mTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    treeViewLayout->addWidget(mTreeView);
    connect(mSearchLineEdit, &SearchWithDelayLineEdit::searchCleared, this, &SearchTreeBaseWidget::slotSearchCleared);
    connect(mSearchLineEdit, &SearchWithDelayLineEdit::searchRequested, this, &SearchTreeBaseWidget::slotSearchRequested);
    connect(this, &SearchTreeBaseWidget::loadMoreElements, this, [this]() {
        slotLoadMoreElements();
    });
    connect(mTreeView, &QTreeView::customContextMenuRequested, this, &SearchTreeBaseWidget::slotCustomContextMenuRequested);
    connect(mTreeView, &QTreeView::doubleClicked, this, &SearchTreeBaseWidget::doubleClicked);
}

SearchTreeBaseWidget::~SearchTreeBaseWidget() = default;

QString SearchTreeBaseWidget::clickableStr() const
{
    return u" <a href=\"loadmoreelement\">%1</a>"_s.arg(i18n("(Click here for Loading more…)"));
}

void SearchTreeBaseWidget::refreshLoadElements()
{
    const int offset = mModel->rowCount();
    slotLoadElements(-1, qMin(50, mModel->total() - offset), mSearchLineEdit->text().trimmed());
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

void SearchTreeBaseWidget::hideColumns()
{
    if (mModel) {
        const auto hideColumns = mModel->hideColumns();
        for (const auto col : hideColumns) {
            mTreeView->setColumnHidden(col, true);
        }
    }
}

void SearchTreeBaseWidget::connectModel()
{
    connect(mModel, &CustomBaseModel::hasFullListChanged, this, &SearchTreeBaseWidget::updateLabel);
    connect(mModel, &CustomBaseModel::totalChanged, this, &SearchTreeBaseWidget::updateLabel);
    connect(mModel, &CustomBaseModel::loadingInProgressChanged, this, &SearchTreeBaseWidget::updateLabel);
}

void SearchTreeBaseWidget::finishSearching()
{
    mModel->setLoadMoreInProgress(false);
    resizeColumToContents();
}

void SearchTreeBaseWidget::resizeColumToContents()
{
    for (int i = 0, total = mTreeView->header()->count(); i < total; ++i) {
        if (!mModel->excludeResizeToContentColumns().contains(i)) {
            mTreeView->resizeColumnToContents(i);
        }
    }
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

#include "moc_searchtreebasewidget.cpp"
