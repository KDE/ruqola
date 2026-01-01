/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "directoryusersmodel.h"

#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT AdminUsersBaseModel : public DirectoryUsersModel
{
    Q_OBJECT
public:
    explicit AdminUsersBaseModel(QObject *parent = nullptr);
    ~AdminUsersBaseModel() override;

protected:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT Users::ParseType parseType() const override;
    LIBRUQOLACORE_NO_EXPORT void removeElement(const QByteArray &identifier) override;
    LIBRUQOLACORE_NO_EXPORT void updateElement(const QJsonObject &obj) override;
    LIBRUQOLACORE_NO_EXPORT void insertElement(const QJsonObject &obj) override;
};
