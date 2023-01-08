/*
  SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "autocorrectiontreewidget.h"

#include <QKeyEvent>

using namespace TextAutoCorrection;

AutoCorrectionTreeWidget::AutoCorrectionTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
}

AutoCorrectionTreeWidget::~AutoCorrectionTreeWidget() = default;

void AutoCorrectionTreeWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        Q_EMIT deleteSelectedItems();
    }
    QTreeWidget::keyPressEvent(event);
}
