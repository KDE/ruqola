/*
  SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "configurepluginstreewidgetdelegate.h"
#include "configurepluginswidget.h"
#include <QApplication>
#include <QPainter>

ConfigurePluginsTreeWidgetDelegate::ConfigurePluginsTreeWidgetDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
}

ConfigurePluginsTreeWidgetDelegate::~ConfigurePluginsTreeWidgetDelegate() = default;

void ConfigurePluginsTreeWidgetDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) {
        return;
    }

    // Get the text and split it into two lines
    const QString text = index.data(Qt::DisplayRole).toString();
    if (text.isEmpty()) {
        return;
    }

    const QString description = index.data(ConfigurePluginsWidget::PluginItemData::Description).toString();
    if (description.isEmpty()) {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    QStyleOptionViewItem opt(option);
    opt.showDecorationSelected = true;
    QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter);

    painter->save();

    // Check if the item has a CheckStateRole
    const QVariant checkStateData = index.data(Qt::CheckStateRole);
    if (checkStateData.isValid()) {
        // Get the checkbox state
        const Qt::CheckState checkState = static_cast<Qt::CheckState>(checkStateData.toInt());

        // Get the checkbox style options
        QStyleOptionButton checkboxOption;
        checkboxOption.state = QStyle::State_Enabled;
        if (checkState == Qt::Checked) {
            checkboxOption.state |= QStyle::State_On;
        } else if (checkState == Qt::Unchecked) {
            checkboxOption.state |= QStyle::State_Off;
        } else {
            checkboxOption.state |= QStyle::State_NoChange;
        }

        // Center the checkbox in the item's rect
        checkboxOption.rect = QApplication::style()->subElementRect(QStyle::SE_ItemViewItemCheckIndicator, &option);
        checkboxOption.rect.moveCenter(QPoint(option.rect.left(), option.rect.center().y()));

        // Draw the checkbox
        QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkboxOption, painter);
    }

    // Set the painter font
    painter->setFont(option.font);

    // Calculate the text rectangles
    QFontMetrics fontMetrics(option.font);
    const QRect rect = option.rect;

    const QSize checkboxSize = QApplication::style()->sizeFromContents(QStyle::CT_CheckBox, nullptr, QSize(), nullptr);

    // Draw the two lines
    painter->setPen(option.palette.text().color());
    const QRect line1Rect(rect.left() + 5 + checkboxSize.width(), rect.top(), rect.width(), fontMetrics.height());
    painter->drawText(line1Rect, Qt::AlignLeft | Qt::AlignVCenter, text);
    if (!description.isEmpty()) {
        QFont f = option.font;
        f.setItalic(true);
        f.setPointSize(f.pointSize() - 2);
        painter->setFont(f);
        const QRect line2Rect(rect.left() + 5 + checkboxSize.width(), rect.top() + fontMetrics.height(), rect.width(), fontMetrics.height());
        painter->drawText(line2Rect, Qt::AlignLeft | Qt::AlignVCenter, description);
    }
    painter->restore();
}

QSize ConfigurePluginsTreeWidgetDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QFontMetrics fontMetrics(option.font);
    const int height = fontMetrics.height() * 2; // Height for two lines

    const QString description = index.data(ConfigurePluginsWidget::PluginItemData::Description).toString();
    int widthDescription = 0;
    if (!description.isEmpty()) {
        QFont f = option.font;
        f.setItalic(true);
        f.setPointSize(f.pointSize() - 2);
        widthDescription = fontMetrics.horizontalAdvance(description);
    }

    const int width = qMax(fontMetrics.horizontalAdvance(index.data(Qt::DisplayRole).toString()), widthDescription);
    return QSize(width, height);
}

#include "moc_configurepluginstreewidgetdelegate.cpp"
