/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CategoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CategoryWidget(QWidget *parent = nullptr);
    ~CategoryWidget() override;

    [[nodiscard]] QString categoryName() const;
    void setCategoryName(const QString &name);

Q_SIGNALS:
    void updateOkButton(bool enabled);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTextChanged(const QString &str);
    QLineEdit *const mCategoryLineEdit;
};
