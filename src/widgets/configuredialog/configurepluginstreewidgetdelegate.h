/*
  SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once
#include <QStyledItemDelegate>
class ConfigurePluginsTreeWidgetDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ConfigurePluginsTreeWidgetDelegate(QObject *parent = nullptr);
    ~ConfigurePluginsTreeWidgetDelegate() override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};
