/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QListView>
class LIBRUQOLAWIDGETS_TESTS_EXPORT EmoticonListViewBase : public QListView
{
    Q_OBJECT
public:
    explicit EmoticonListViewBase(QWidget *parent = nullptr);
    ~EmoticonListViewBase() override;
    void setFontSize(int newFontSize);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *e) override;

Q_SIGNALS:
    void fontSizeChanged(int size);

private:
    void applyFontSize();
    int mFontSize = 18;
};
