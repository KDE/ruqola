/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QComboBox>

#include "libruqolawidgets_private_export.h"
class SystemMessagesModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SystemMessagesComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit SystemMessagesComboBox(QWidget *parent = nullptr);
    ~SystemMessagesComboBox() override;
    void setMessagesSystem(const QStringList &lst);
    [[nodiscard]] QStringList systemMessagesSelected() const;

Q_SIGNALS:
    void settingsChanged();

private:
    SystemMessagesModel *const mSystemMessagesModel;
};
