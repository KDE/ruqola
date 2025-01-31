/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QComboBox>

class ShowTheadsComboBox : public QComboBox
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
