/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistdelegatebase.h"
#include "delegateutils/textselection.h"
#include "delegateutils/textselectionimpl.h"

#include <QAbstractTextDocumentLayout>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>

MessageListDelegateBase::MessageListDelegateBase(QObject *parent)
    : QItemDelegate{parent}
    , mTextSelectionImpl(new TextSelectionImpl)
{
    auto textSelection = mTextSelectionImpl->textSelection();
    textSelection->setTextHelperFactory(this);
    connect(textSelection, &TextSelection::repaintNeeded, this, &MessageListDelegateBase::updateView);
}

MessageListDelegateBase::~MessageListDelegateBase()
{
    delete mTextSelectionImpl;
}

void MessageListDelegateBase::clearCache()
{
    mDocumentCache.clear();
}

bool MessageListDelegateBase::maybeStartDrag(QMouseEvent *mouseEvent, QRect messageRect, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (!mTextSelectionImpl->mightStartDrag()) {
        return false;
    }
    const QPoint pos = mouseEvent->pos() - messageRect.topLeft();
    if (mTextSelectionImpl->textSelection()->hasSelection()) {
        const auto *doc = documentForModelIndex(index, messageRect.width());
        const int charPos = doc->documentLayout()->hitTest(pos, Qt::FuzzyHit);
        if (charPos != -1 && mTextSelectionImpl->textSelection()->contains(index, charPos)) {
            auto mimeData = new QMimeData;
            mimeData->setHtml(mTextSelectionImpl->textSelection()->selectedText(TextSelection::Html));
            mimeData->setText(mTextSelectionImpl->textSelection()->selectedText(TextSelection::Text));
            auto drag = new QDrag(const_cast<QWidget *>(option.widget));
            drag->setMimeData(mimeData);
            drag->exec(Qt::CopyAction);
            mTextSelectionImpl->setMightStartDrag(false); // don't clear selection on release
            return true;
        }
    }
    return false;
}
