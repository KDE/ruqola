/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "textconverter.h"
#include <TextUtils/TextUtilsBlockCMarkSupport>
class RuqolaBlockCMarkSupport : public TextUtils::TextUtilsBlockCMarkSupport
{
public:
    RuqolaBlockCMarkSupport();
    ~RuqolaBlockCMarkSupport() override;

    [[nodiscard]] TextConverter::ConvertMessageTextSettings *settings() const;
    void setSettings(TextConverter::ConvertMessageTextSettings *newSettings);

protected:
    [[nodiscard]] QString addHighlighter(const QString &str,
                                         const QString &language,
                                         const QString &searchText,
                                         const QByteArray &uuid,
                                         int &blockCodeIndex,
                                         int &numberOfTextSearched) override;

private:
    TextConverter::ConvertMessageTextSettings *mSettings = nullptr;
};
