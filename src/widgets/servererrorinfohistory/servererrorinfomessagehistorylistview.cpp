/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfomessagehistorylistview.h"
using namespace Qt::Literals::StringLiterals;

#include "model/servererrorinfohistorymodel.h"
#include "servererrorinfohistorydelegate.h"
#include "servererrorinfohistorymanager.h"

#include <KLocalizedString>
#include <QMenu>
#include <QMouseEvent>

#include "config-ruqola.h"

ServerErrorInfoMessageHistoryListView::ServerErrorInfoMessageHistoryListView(QWidget *parent)
    : MessageListViewBase(parent)
    , mListServerErrorInfosDelegate(new ServerErrorInfoHistoryDelegate(this, this))
{
    setContextMenuPolicy(Qt::CustomContextMenu);

    mListServerErrorInfosDelegate->setObjectName(u"mListServerErrorInfosDelegate"_s);
    setItemDelegate(mListServerErrorInfosDelegate);

    connect(mListServerErrorInfosDelegate, &ServerErrorInfoHistoryDelegate::updateView, this, [this](const QModelIndex &index) {
        update(index);
    });
    connect(this, &QListView::customContextMenuRequested, this, &ServerErrorInfoMessageHistoryListView::slotCustomContextMenuRequested);
    connect(this,
            &ServerErrorInfoMessageHistoryListView::needToClearSizeHintCache,
            mListServerErrorInfosDelegate,
            &ServerErrorInfoHistoryDelegate::clearSizeHintCache);
}

ServerErrorInfoMessageHistoryListView::~ServerErrorInfoMessageHistoryListView() = default;

void ServerErrorInfoMessageHistoryListView::slotCustomContextMenuRequested(const QPoint &pos)
{
    if (model()->rowCount() > 0) {
        QMenu menu(this);
        menu.addAction(QIcon::fromTheme(u"edit-clear-history"_s), i18nc("@action", "Clear"), this, &ServerErrorInfoMessageHistoryListView::slotClearList);
        const QModelIndex index = indexAt(pos);
        if (index.isValid()) {
            menu.addSeparator();
            auto copyAction = new QAction(&menu);
            copyAction->setIcon(QIcon::fromTheme(u"edit-copy"_s));
            if (hasSelection()) {
                copyAction->setText(i18nc("@action", "Copy Selection"));
            } else {
                copyAction->setText(i18nc("@action", "Copy Message"));
            }
            copyAction->setShortcut(QKeySequence::Copy);
            connect(copyAction, &QAction::triggered, this, [this, index]() {
                copyMessageToClipboard(index);
            });
            menu.addAction(copyAction);
            if (mListServerErrorInfosDelegate->hasSelection()) {
                addTextPlugins(&menu, mListServerErrorInfosDelegate->selectedText());
            }

#if HAVE_TEXT_TO_SPEECH
            menu.addSeparator();
            auto speakAction = menu.addAction(QIcon::fromTheme(u"text-speak-symbolic"_s), i18nc("@action", "Speak Text"));
            connect(speakAction, &QAction::triggered, this, [this, index]() {
                slotTextToSpeech(index);
            });
#endif

            menu.addSeparator();
            menu.addAction(QIcon::fromTheme(u"edit-select-all"_s), i18nc("@action", "Select All"), this, [this, index]() {
                slotSelectAll(index);
            });
        }
        menu.exec(viewport()->mapToGlobal(pos));
    }
}

bool ServerErrorInfoMessageHistoryListView::maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return mListServerErrorInfosDelegate->maybeStartDrag(event, option, index);
}

bool ServerErrorInfoMessageHistoryListView::mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return mListServerErrorInfosDelegate->mouseEvent(event, option, index);
}

void ServerErrorInfoMessageHistoryListView::clearCache()
{
    mListServerErrorInfosDelegate->clearCache();
}

void ServerErrorInfoMessageHistoryListView::slotSelectAll(const QModelIndex &index)
{
    mListServerErrorInfosDelegate->selectAll(listViewOptions(), index);
}

const QString &ServerErrorInfoMessageHistoryListView::searchText() const
{
    return mListServerErrorInfosDelegate->searchText();
}

void ServerErrorInfoMessageHistoryListView::setSearchText(const QString &newSearchText)
{
    mListServerErrorInfosDelegate->setSearchText(newSearchText);
}

QString ServerErrorInfoMessageHistoryListView::selectedText() const
{
    return mListServerErrorInfosDelegate->selectedText();
}

void ServerErrorInfoMessageHistoryListView::slotTextToSpeech(const QModelIndex &index)
{
    const QString messageText = selectedText(index);
    if (!messageText.isEmpty()) {
        Q_EMIT textToSpeech(messageText);
    }
}

QString ServerErrorInfoMessageHistoryListView::selectedText(const QModelIndex &index)
{
    QString messageText = selectedText();
    if (messageText.isEmpty()) {
        if (!index.isValid()) {
            return {};
        }
        messageText = index.data(ServerErrorInfoHistoryModel::MessageStr).toString();
    }
    return messageText;
}

bool ServerErrorInfoMessageHistoryListView::hasSelection() const
{
    return mListServerErrorInfosDelegate->hasSelection();
}

void ServerErrorInfoMessageHistoryListView::slotClearList()
{
    clearCache();
    ServerErrorInfoHistoryManager::self()->serverErrorInfoHistoryModel()->clear();
}

#include "moc_servererrorinfomessagehistorylistview.cpp"
