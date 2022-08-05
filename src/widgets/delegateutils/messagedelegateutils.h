/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "messages/messageattachment.h"

#include <QAbstractTextDocumentLayout>
#include <QString>
#include <QTextDocument>
#include <QVector>
#include <memory>

class TextSelection;
class QStyleOptionViewItem;
namespace MessageDelegateUtils
{
Q_REQUIRED_RESULT std::unique_ptr<QTextDocument> createTextDocument(bool useItalic, const QString &text, int width);

Q_REQUIRED_RESULT bool generateToolTip(const QTextDocument *doc, const QPoint &pos, QString &formattedTooltip);

void generateToolTip(const QString &toolTip, const QString &href, QString &formattedTooltip);

Q_REQUIRED_RESULT bool useItalicsForMessage(const QModelIndex &index);

Q_REQUIRED_RESULT bool pendingMessage(const QModelIndex &index);
Q_REQUIRED_RESULT QVector<QAbstractTextDocumentLayout::Selection> selection(TextSelection *selection,
                                                                            QTextDocument *doc,
                                                                            const QModelIndex &index,
                                                                            const QStyleOptionViewItem &option,
                                                                            const MessageAttachment &msgAttach = {},
                                                                            bool isAMessage = true);

void drawSelection(QTextDocument *doc,
                   QRect rect,
                   int top,
                   QPainter *painter,
                   const QModelIndex &index,
                   const QStyleOptionViewItem &option,
                   TextSelection *selection,
                   const MessageAttachment &msgAttach,
                   bool isAMessage = true);

void setClipboardSelection(TextSelection *selection);

Q_REQUIRED_RESULT QSizeF dprAwareSize(const QPixmap &pixmap);

Q_REQUIRED_RESULT qreal basicMargin();
Q_REQUIRED_RESULT QSize timeStampSize(const QString &timeStampText, const QStyleOptionViewItem &option);
Q_REQUIRED_RESULT QSize textSizeHint(QTextDocument *doc, qreal *pBaseLine);
Q_REQUIRED_RESULT bool showIgnoreMessages(const QModelIndex &index);
}
