/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QStyledItemDelegate>

class AdministratorUsersPendingActionDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit AdministratorUsersPendingActionDelegate(QObject *parent = nullptr);
    ~AdministratorUsersPendingActionDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *, const QStyleOptionViewItem &, const QModelIndex &) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &) const override;
Q_SIGNALS:
    void pendingActionActivated(const QModelIndex &index);

private:
    [[nodiscard]] QStyleOptionButton buttonOption(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawFocus(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect) const;
};
