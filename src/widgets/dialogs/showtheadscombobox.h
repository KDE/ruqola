/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QComboBox>

class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowTheadsComboBox : public QComboBox
{
    Q_OBJECT
public:
    enum ThreadType : uint8_t {
        All = 0,
        Following,
        Unread,
    };
    Q_ENUM(ThreadType);
    explicit ShowTheadsComboBox(QWidget *parent = nullptr);
    ~ShowTheadsComboBox() override;

    [[nodiscard]] ShowTheadsComboBox::ThreadType currentThreadType() const;
};
