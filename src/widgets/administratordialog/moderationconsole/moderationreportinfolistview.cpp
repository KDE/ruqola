/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportinfolistview.h"
#include "model/moderationreportinfomodel.h"
#include "moderationreportinfodelegate.h"

#include <KLocalizedString>

#include <QMenu>
#include <QMouseEvent>

#include <config-ruqola.h>

ModerationReportInfoListView::ModerationReportInfoListView(QWidget *parent)
    : MessageListViewBase(parent)
    , mModerationReportInfoDelegate(new ModerationReportInfoDelegate(this, this))
{
    mModerationReportInfoDelegate->setObjectName(QStringLiteral("mModerationReportInfoDelegate"));
    setItemDelegate(mModerationReportInfoDelegate);
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(mModerationReportInfoDelegate, &ModerationReportInfoDelegate::updateView, this, [this](const QModelIndex &index) {
        update(index);
    });
    connect(this, &QListView::customContextMenuRequested, this, &ModerationReportInfoListView::slotCustomContextMenuRequested);
    connect(this, &ModerationReportInfoListView::needToClearSizeHintCache, mModerationReportInfoDelegate, &ModerationReportInfoDelegate::clearSizeHintCache);
}

ModerationReportInfoListView::~ModerationReportInfoListView() = default;

bool ModerationReportInfoListView::maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return mModerationReportInfoDelegate->maybeStartDrag(event, option, index);
}

bool ModerationReportInfoListView::mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return mModerationReportInfoDelegate->mouseEvent(event, option, index);
}

void ModerationReportInfoListView::clearCache()
{
    mModerationReportInfoDelegate->clearCache();
}

void ModerationReportInfoListView::slotSelectAll(const QModelIndex &index)
{
    mModerationReportInfoDelegate->selectAll(listViewOptions(), index);
}

const QString &ModerationReportInfoListView::searchText() const
{
    return mModerationReportInfoDelegate->searchText();
}

void ModerationReportInfoListView::setSearchText(const QString &newSearchText)
{
    mModerationReportInfoDelegate->setSearchText(newSearchText);
}

QString ModerationReportInfoListView::selectedText() const
{
    return mModerationReportInfoDelegate->selectedText();
}

void ModerationReportInfoListView::slotCustomContextMenuRequested(const QPoint &pos)
{
#if 0 // TODO
    if (model()->rowCount() > 0) {
        QMenu menu(this);
        menu.addAction(QIcon::fromTheme(QStringLiteral("edit-clear-history")), i18n("Clear"), this, &ModerationReportInfoListView::slotClearList);
        const QModelIndex index = indexAt(pos);
        if (index.isValid()) {
            menu.addSeparator();
            menu.addAction(i18n("Go to Message"), this, [this, index]() {
                Q_EMIT showMessage(index);
            });
            menu.addSeparator();
            auto copyAction = new QAction(QIcon::fromTheme(QStringLiteral("edit-copy")), i18n("Copy Message"), &menu);
            copyAction->setShortcut(QKeySequence::Copy);
            connect(copyAction, &QAction::triggered, this, [=]() {
                copyMessageToClipboard(index);
            });
            menu.addAction(copyAction);
            if (mModerationReportInfoDelegate->hasSelection()) {
                addTextPlugins(&menu, mModerationReportInfoDelegate->selectedText());
            }
#if HAVE_TEXT_TO_SPEECH
            menu.addSeparator();
            auto speakAction = menu.addAction(QIcon::fromTheme(QStringLiteral("preferences-desktop-text-to-speech")), i18n("Speak Text"));
            connect(speakAction, &QAction::triggered, this, [=]() {
                slotTextToSpeech(index);
            });
#endif
            menu.addSeparator();
            menu.addAction(i18n("Select All"), this, [this, index]() {
                slotSelectAll(index);
            });
        }
        menu.exec(viewport()->mapToGlobal(pos));
    }
#endif
}

void ModerationReportInfoListView::slotTextToSpeech(const QModelIndex &index)
{
    const QString messageText = selectedText(index);
    if (!messageText.isEmpty()) {
        Q_EMIT textToSpeech(messageText);
    }
}

QString ModerationReportInfoListView::selectedText(const QModelIndex &index)
{
    QString messageText = selectedText();
    if (messageText.isEmpty()) {
        if (!index.isValid()) {
            return {};
        }
        messageText = index.data(ModerationReportInfoModel::Message).toString();
    }
    return messageText;
}

#include "moc_moderationreportinfolistview.cpp"
