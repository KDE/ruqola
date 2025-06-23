/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfolistview.h"
using namespace Qt::Literals::StringLiterals;

#include "bannerinfolistviewdelegate.h"
#include "model/bannerinfosmodel.h"

#include <KLocalizedString>

#include <QMenu>
#include <QMouseEvent>
#include <QScrollBar>

BannerInfoListView::BannerInfoListView(RocketChatAccount *account, QWidget *parent)
    : MessageListViewBase(parent)
    , mBannerInfoListViewDelegate(new BannerInfoListViewDelegate(this, account, this))
{
    setItemDelegate(mBannerInfoListViewDelegate);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mBannerInfoListViewDelegate, &BannerInfoListViewDelegate::updateView, this, [this](const QModelIndex &index) {
        update(index);
    });
    const auto lineHeight = fontMetrics().height() + 10;
    verticalScrollBar()->setSingleStep(lineHeight);
    connect(this, &QListView::customContextMenuRequested, this, &BannerInfoListView::slotCustomContextMenuRequested);
    connect(this, &BannerInfoListView::needToClearSizeHintCache, mBannerInfoListViewDelegate, &BannerInfoListViewDelegate::clearSizeHintCache);
}

BannerInfoListView::~BannerInfoListView() = default;

bool BannerInfoListView::maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return mBannerInfoListViewDelegate->maybeStartDrag(event, option, index);
}

bool BannerInfoListView::mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return mBannerInfoListViewDelegate->mouseEvent(event, option, index);
}

void BannerInfoListView::slotCustomContextMenuRequested(const QPoint &pos)
{
    if (model()->rowCount() > 0) {
        const QModelIndex index = indexAt(pos);
        if (index.isValid()) {
            QMenu menu(this);
            menu.addSeparator();
            menu.addAction(QIcon::fromTheme(u"edit-select-all"_s), i18nc("@action", "Select All"), this, [this, index]() {
                slotSelectAll(index);
            });
            menu.addSeparator();
            auto copyAction = new QAction(QIcon::fromTheme(u"edit-copy"_s), i18nc("@action", "Copy Text"), &menu);
            copyAction->setShortcut(QKeySequence::Copy);
            connect(copyAction, &QAction::triggered, this, [this, index]() {
                copyMessageToClipboard(index);
            });
            menu.addAction(copyAction);
            menu.exec(viewport()->mapToGlobal(pos));
        }
    }
}

void BannerInfoListView::slotSelectAll(const QModelIndex &index)
{
    mBannerInfoListViewDelegate->selectAll(listViewOptions(), index);
}

QString BannerInfoListView::selectedText(const QModelIndex &index)
{
    QString messageText = selectedText();
    if (messageText.isEmpty()) {
        if (!index.isValid()) {
            return {};
        }
        messageText = index.data(BannerInfosModel::Text).toString();
    }
    return messageText;
}

QString BannerInfoListView::selectedText() const
{
    return mBannerInfoListViewDelegate->selectedText();
}

bool BannerInfoListView::hasSelection() const
{
    return mBannerInfoListViewDelegate->hasSelection();
}

#include "moc_bannerinfolistview.cpp"
