/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#pragma once

#include "common/completionlineedit.h"
#include "libruqolawidgets_private_export.h"
class QJsonObject;
class ChannelCompleterModel;
class ChannelCompleterFilterProxyModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelSearchNameLineEdit : public CompletionLineEdit
{
    Q_OBJECT
public:
    struct ChannelCompletionInfo {
        QString channelName;
        QString channelId;
    };
    explicit ChannelSearchNameLineEdit(QWidget *parent = nullptr);
    ~ChannelSearchNameLineEdit() override;
Q_SIGNALS:
    void newRoomName(const ChannelSearchNameLineEdit::ChannelCompletionInfo &userInfo);

private:
    void slotTextChanged(const QString &text);
    void slotComplete(const QModelIndex &index);
    void slotSearchDone(const QJsonObject &obj);
    ChannelCompleterFilterProxyModel *const mChannelCompleterFilterProxyModel;
    ChannelCompleterModel *const mChannelCompleterModel;
};
