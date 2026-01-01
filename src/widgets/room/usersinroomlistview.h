/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QListView>

class LIBRUQOLAWIDGETS_TESTS_EXPORT UsersInRoomListView : public QListView
{
    Q_OBJECT
public:
    explicit UsersInRoomListView(QWidget *parent = nullptr);
    ~UsersInRoomListView() override;

protected:
    void paintEvent(QPaintEvent *event) override;
    [[nodiscard]] bool event(QEvent *ev) override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void generalPaletteChanged();
    QColor mTextColor;
};
