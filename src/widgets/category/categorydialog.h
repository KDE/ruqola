/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class CategoryWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CategoryDialog : public QDialog
{
    Q_OBJECT
public:
    enum class Mode : uint8_t {
        Add,
        Rename,
    };
    explicit CategoryDialog(Mode mode, QWidget *parent = nullptr);
    ~CategoryDialog() override;

    [[nodiscard]] QString categoryName() const;
    void setCategoryName(const QString &name);

private:
    CategoryWidget *const mCategoryWidget;
};
