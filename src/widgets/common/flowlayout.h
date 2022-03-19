/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_export.h"
#include <QLayout>
#include <QVector>

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
    int doFlow(QRect rect, bool effective) const;

private:
    QVector<QLayoutItem *> mItems;
    int mHorizontalSpacing = -1;
    int mVerticalSpacing = -1;
};
