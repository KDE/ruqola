/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QStyledItemDelegate>

class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreDatabaseTimeStampDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ExploreDatabaseTimeStampDelegate(QObject *parent = nullptr);
    ~ExploreDatabaseTimeStampDelegate() override;

    [[nodiscard]] QString displayText(const QVariant &value, const QLocale &locale) const override;
};
