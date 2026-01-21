/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentdelegatehelperactions.h"

#include "common/delegatepaintutil.h"
#include "dialogs/showvideodialog.h"
#include "misc/messageattachmentdownloadandsavejob.h"
#include "rocketchataccount.h"

#include <KLocalizedString>

#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionViewItem>

using namespace Qt::Literals::StringLiterals;
MessageAttachmentDelegateHelperActions::MessageAttachmentDelegateHelperActions(RocketChatAccount *account,
                                                                               QListView *view,
                                                                               TextSelectionImpl *textSelectionImpl)
    : MessageDelegateHelperBase(account, view, textSelectionImpl)
{
}

MessageAttachmentDelegateHelperActions::~MessageAttachmentDelegateHelperActions() = default;

void MessageAttachmentDelegateHelperActions::draw(const MessageAttachmentAction &act,
                                                  QPainter *painter,
                                                  QRect messageRect,
                                                  const QModelIndex &index,
                                                  const QStyleOptionViewItem &option) const
{
}

QSize MessageAttachmentDelegateHelperActions::sizeHint(const MessageAttachmentAction &act,
                                                       const QModelIndex &index,
                                                       int maxWidth,
                                                       const QStyleOptionViewItem &option) const
{
    return {};
}

bool MessageAttachmentDelegateHelperActions::handleMouseEvent(const MessageAttachmentAction &act,
                                                              QMouseEvent *mouseEvent,
                                                              QRect attachmentsRect,
                                                              const QStyleOptionViewItem &option,
                                                              const QModelIndex &index)
{
    return false;
}
