/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <TextUtils/TextUtilsBlockCMarkSupport>

class RuqolaBlockCMarkSupport : public TextUtils::TextUtilsBlockCMarkSupport
{
public:
    RuqolaBlockCMarkSupport();
    ~RuqolaBlockCMarkSupport() override;

protected:
    [[nodiscard]] QString
    addHighlighter(const QString &str, const QString &language, const QString &searchText, const QByteArray &uuid, int &blockCodeIndex) override;
};
