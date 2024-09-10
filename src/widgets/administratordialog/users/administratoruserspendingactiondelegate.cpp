/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratoruserspendingactiondelegate.h"
#include <KLocalizedString>
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

    QStyleOptionButton buttonOpt = buttonOption(option);
    painter->setRenderHint(QPainter::Antialiasing);
    QApplication::style()->drawControl(QStyle::CE_PushButton, &buttonOpt, painter);
    // TODO
}

QStyleOptionButton AdministratorUsersPendingActionDelegate::buttonOption(const QStyleOptionViewItem &option) const
{
    const QString label = i18n("Retrieval Options");
    QStyleOptionButton buttonOpt;
    QRect buttonRect = option.rect;
    const int height = option.rect.height() / 2;
    const int width = 22 + option.fontMetrics.boundingRect(label).width() + 40; // icon size + label size + arrow and padding
    buttonRect.setTop(0);
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
    const QPoint mousePos = me->pos() - option.rect.topLeft();

    QStyleOptionButton buttonOpt = buttonOption(option);

    if (buttonOpt.rect.contains(mousePos)) {
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            return false;
        case QEvent::MouseButtonRelease: {
            Q_EMIT pendingActionActivated();
            /*
            QPoint pos = buttonOpt.rect.bottomLeft() + option.rect.topLeft();
            const QString ident = index.data(Akonadi::AgentInstanceModel::InstanceIdentifierRole).toString();
            Q_EMIT optionsClicked(ident, pos);
            */
            return true;
        }
        default:
            return false;
        }
    }
    return false;
}

#include "moc_administratoruserspendingactiondelegate.cpp"
