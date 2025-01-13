/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
  code based on kdenlive
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QObject>

class LIBRUQOLAWIDGETS_TESTS_EXPORT NeedUpdateParseHtmlJob : public QObject
{
    Q_OBJECT
public:
    explicit NeedUpdateParseHtmlJob(QObject *parent = nullptr);
    ~NeedUpdateParseHtmlJob() override;

    [[nodiscard]] bool canStart() const;
};
