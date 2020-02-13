/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#ifndef FLOWLAYOUT_H
#define FLOWLAYOUT_H
#include <QLayout>
#include <QVector>
#include "libruqolawidgets_export.h"

class LIBRUQOLAWIDGETS_EXPORT FlowLayout : public QLayout
{
    Q_OBJECT

public:
    explicit FlowLayout(QWidget *parent = nullptr);
    ~FlowLayout() override;

    Q_REQUIRED_RESULT int horizontalSpacing() const;
    void setHorizontalSpacing(int horizontalSpacing);

    Q_REQUIRED_RESULT int verticalSpacing() const;
    void setVerticalSpacing(int verticalSpacing);

    QSize sizeHint() const override;
    QSize minimumSize() const override;

    void addItem(QLayoutItem *item) override;
    QLayoutItem *itemAt(int index) const override;
    QLayoutItem *takeAt(int index) override;
    Q_REQUIRED_RESULT int count() const override;

    Q_REQUIRED_RESULT Qt::Orientations expandingDirections() const override;
    Q_REQUIRED_RESULT bool hasHeightForWidth() const override;
    Q_REQUIRED_RESULT int heightForWidth(int width) const override;

    void setGeometry(const QRect &rect) override;

    void clear();
    void clearAndDeleteWidgets();

private:
    int doFlow(const QRect &rect, bool effective) const;

private:
    QVector<QLayoutItem *> mItems;
    int mHorizontalSpacing = -1;
    int mVerticalSpacing = -1;
};

#endif
