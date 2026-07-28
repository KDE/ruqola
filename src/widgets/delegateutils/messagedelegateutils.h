/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "messages/messageattachment.h"
#include "messages/messageurl.h"

#include <QAbstractTextDocumentLayout>
#include <QList>
#include <QModelIndex>
#include <QString>
#include <QStyleOptionViewItem>
#include <memory>

class TextSelection;
class QTextDocument;
namespace MessageDelegateUtils
{
[[nodiscard]] std::unique_ptr<QTextDocument> createTextDocument(bool useItalic, const QString &text, int width);

[[nodiscard]] bool generateToolTip(const QTextDocument *doc, const QPoint &pos, QString &formattedTooltip);

void generateToolTip(const QString &toolTip, const QString &href, QString &formattedTooltip);

[[nodiscard]] bool useItalicsForMessage(const QModelIndex &index);

[[nodiscard]] bool pendingMessage(const QModelIndex &index);
[[nodiscard]] QList<QAbstractTextDocumentLayout::Selection> selection(TextSelection *selection,
                                                                      QTextDocument *doc,
                                                                      const QModelIndex &index,
                                                                      const QStyleOptionViewItem &option,
                                                                      const MessageAttachment &msgAttach = {},
                                                                      const MessageUrl &msgUrl = {},
                                                                      bool isAMessage = true);

void drawSelection(QTextDocument *doc,
                   QRect rect,
                   int top,
                   QPainter *painter,
                   const QModelIndex &index,
                   const QStyleOptionViewItem &option,
                   TextSelection *selection,
                   const MessageAttachment &msgAttach,
                   const MessageUrl &msgUrl,
                   bool isAMessage = true);

void setClipboardSelection(TextSelection *selection);

[[nodiscard]] QSizeF dprAwareSize(const QPixmap &pixmap);

[[nodiscard]] qreal basicMargin();
// Vertical spacing scale for the message list. Empty space is placed above each message block:
// senderBlockSpacing() when the author changes (so a new speaker reads as a new block) and the
// smaller groupedMessageSpacing() for a consecutive message from the same author. Both are
// deliberately larger than the name-to-text gap, so proximity groups each author with their text.
[[nodiscard]] int senderBlockSpacing();
[[nodiscard]] int groupedMessageSpacing();
[[nodiscard]] QSize timeStampSize(const QString &timeStampText, const QStyleOptionViewItem &option);
[[nodiscard]] QSize textSizeHint(QTextDocument *doc, qreal *pBaseLine);
[[nodiscard]] bool showIgnoreMessages(const QModelIndex &index);
}
