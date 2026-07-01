/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLineEdit;
class ReadReceiptsModel;
class RocketChatAccount;
class ReadReceiptsFilterProxyModel;
class QListView;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowReadReceiptsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowReadReceiptsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowReadReceiptsWidget() override;

    void setMessageId(const QByteArray &roomId);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchReadReceiptsChanged(const QString &str);
    RocketChatAccount *const mCurrentRocketChatAccount;
    QLineEdit *const mShowReadReceiptsLineEdit;
    QListView *const mListReadReceipts;
    ReadReceiptsModel *const mModel;
    ReadReceiptsFilterProxyModel *const mReadReceiptsFilterProxyModel;
};
