/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QListView>

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageListViewBase : public QListView
{
    Q_OBJECT
public:
    explicit MessageListViewBase(QWidget *parent = nullptr);
    ~MessageListViewBase() override;

    void checkIfAtBottom();
    void updateVerticalPageStep();
    void maybeScrollToBottom();

protected:
    void resizeEvent(QResizeEvent *ev) override;

    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void handleMouseEvent(QMouseEvent *event);
    Q_REQUIRED_RESULT QStyleOptionViewItem listViewOptions() const;
    bool mAtBottom = true;
    QPoint mPressedPosition;
};
