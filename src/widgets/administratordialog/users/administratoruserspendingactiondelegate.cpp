/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratoruserspendingactiondelegate.h"
#include "model/adminuserspendingmodel.h"
#include <QApplication>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>

AdministratorUsersPendingActionDelegate::AdministratorUsersPendingActionDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
}

AdministratorUsersPendingActionDelegate::~AdministratorUsersPendingActionDelegate() = default;

void AdministratorUsersPendingActionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) {
        return;
    }

    QStyleOptionViewItem opt(option);
    opt.showDecorationSelected = true;
    QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter);

    painter->save();
    QStyleOptionButton buttonOpt = buttonOption(option, index);
    painter->setRenderHint(QPainter::Antialiasing);
    QApplication::style()->drawControl(QStyle::CE_PushButton, &buttonOpt, painter);

    painter->restore();
    drawFocus(painter, option, option.rect);
}

void AdministratorUsersPendingActionDelegate::drawFocus(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect) const
{
    if (option.state & QStyle::State_HasFocus) {
        QStyleOptionFocusRect o;
        o.QStyleOption::operator=(option);
        o.rect = rect;
        o.state |= QStyle::State_KeyboardFocusChange;
        QPalette::ColorGroup cg = (option.state & QStyle::State_Enabled) ? QPalette::Normal : QPalette::Disabled;
        o.backgroundColor = option.palette.color(cg, (option.state & QStyle::State_Selected) ? QPalette::Highlight : QPalette::Window);
        QApplication::style()->drawPrimitive(QStyle::PE_FrameFocusRect, &o, painter);
    }
}

QStyleOptionButton AdministratorUsersPendingActionDelegate::buttonOption(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QString label = index.model()->index(index.row(), AdminUsersPendingModel::PendingActionButtonText).data().toString();
    QStyleOptionButton buttonOpt;
    QRect buttonRect = option.rect;
    const int height = option.rect.height();
    const int width = 22 + option.fontMetrics.boundingRect(label).width() + 40;
    buttonRect.setHeight(height);
    buttonRect.setLeft(option.rect.right() - width);
    buttonRect.setWidth(width);

    buttonOpt.rect = buttonRect;
    buttonOpt.state = option.state;
    buttonOpt.text = label;
    buttonOpt.palette = option.palette;

    return buttonOpt;
}

QWidget *AdministratorUsersPendingActionDelegate::createEditor(QWidget *, const QStyleOptionViewItem &, const QModelIndex &) const
{
    return nullptr;
}

bool AdministratorUsersPendingActionDelegate::editorEvent(QEvent *event,
                                                          QAbstractItemModel *model,
                                                          const QStyleOptionViewItem &option,
                                                          const QModelIndex &index)
{
    Q_UNUSED(model)
    if (!index.isValid()) {
        return false;
    }
    if (!((event->type() == QEvent::MouseButtonRelease) || (event->type() == QEvent::MouseButtonPress) || (event->type() == QEvent::MouseMove))) {
        return false;
    }

    auto me = static_cast<QMouseEvent *>(event);
    const QPoint mousePos = me->pos();

    const QStyleOptionButton buttonOpt = buttonOption(option, index);
    if (buttonOpt.rect.contains(mousePos)) {
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            return false;
        case QEvent::MouseButtonRelease: {
            Q_EMIT pendingActionActivated(index);
            return true;
        }
        default:
            return false;
        }
    }
    return false;
}

QSize AdministratorUsersPendingActionDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const int textHeight = option.fontMetrics.height() + qMax(option.fontMetrics.height() / 2, 12); // height of text
    const QString label = index.model()->index(index.row(), AdminUsersPendingModel::PendingActionButtonText).data().toString();
    const int width = 22 + option.fontMetrics.boundingRect(label).width() + 40;
    return {width, textHeight}; // any width,the view will give us the whole thing in list mode
}

#include "moc_administratoruserspendingactiondelegate.cpp"
