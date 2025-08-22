/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QStyledItemDelegate>
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreDatabaseRoomIdDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ExploreDatabaseRoomIdDelegate(RocketChatAccount *account, QObject *parent = nullptr);
    ~ExploreDatabaseRoomIdDelegate() override;

    [[nodiscard]] QString displayText(const QVariant &value, const QLocale &locale) const override;

    [[nodiscard]] bool helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    RocketChatAccount *const mRocketChatAccount;
};
