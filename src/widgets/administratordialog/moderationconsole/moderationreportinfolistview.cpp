/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportinfolistview.h"
using namespace Qt::Literals::StringLiterals;

#include "model/moderationreportinfomodel.h"
#include "moderationreportinfodelegate.h"

#include <KLocalizedString>

#include <QMenu>
#include <QMouseEvent>

#include "config-ruqola.h"

ModerationReportInfoListView::ModerationReportInfoListView(RocketChatAccount *account, QWidget *parent)
    : MessageListViewBase(parent)
    , mModerationReportInfoDelegate(new ModerationReportInfoDelegate(account, this, this))
{
    mModerationReportInfoDelegate->setObjectName(u"mModerationReportInfoDelegate"_s);
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
    if (model()->rowCount() > 0) {
        QMenu menu(this);
        const QModelIndex index = indexAt(pos);
        if (index.isValid()) {
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
            if (mModerationReportInfoDelegate->hasSelection()) {
                addTextPlugins(&menu, mModerationReportInfoDelegate->selectedText());
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

bool ModerationReportInfoListView::hasSelection() const
{
    return mModerationReportInfoDelegate->hasSelection();
}

#include "moc_moderationreportinfolistview.cpp"
