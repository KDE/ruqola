/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "config-ruqola.h"
#if HAVE_TEXTADDONSWIDGETS_COMPLETIONLINEEDIT
#include <TextAddonsWidgets/CompletionLineEdit>
#else
#include "common/completionlineedit.h"
#endif
#include "libruqolawidgets_private_export.h"
class QJsonObject;
class InputCompleterModel;
class ChannelCompleterFilterProxyModel;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelSearchNameLineEdit : public
#if HAVE_TEXTADDONSWIDGETS_COMPLETIONLINEEDIT
                                                                TextAddonsWidgets::
#endif
                                                                    CompletionLineEdit
{
    Q_OBJECT
public:
    struct ChannelCompletionInfo {
        QString channelName;
        QByteArray channelId;
    };
    explicit ChannelSearchNameLineEdit(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ChannelSearchNameLineEdit() override;
Q_SIGNALS:
    void newRoomName(const ChannelSearchNameLineEdit::ChannelCompletionInfo &userInfo);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTextChanged(const QString &text);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotComplete(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchDone(const QJsonObject &obj);
    ChannelCompleterFilterProxyModel *const mChannelCompleterFilterProxyModel;
    InputCompleterModel *const mChannelCompleterModel;
    RocketChatAccount *const mRocketChatAccount;
};
