/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QHBoxLayout;
class QLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SearchMessageLineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchMessageLineWidget(QWidget *parent = nullptr);
    ~SearchMessageLineWidget() override;

    void addMessageLineWidget(QWidget *w);

Q_SIGNALS:
    void textChanged(const QString &str);

private:
    QHBoxLayout *const mMainLayout;
    QLineEdit *const mSearchMessageLineEdit;
};
