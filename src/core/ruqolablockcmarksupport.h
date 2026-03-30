/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "config-ruqola.h"
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
#if HAVE_TEXTAUTOGENERATE_NEW_CMARK_SUPPORT
    [[nodiscard]] QString addHighlighter(const QString &str,
                                         const QString &language,
                                         const QString &searchText,
                                         const QByteArray &uuid,
                                         int &blockCodeIndex,
                                         int &numberOfTextSearched,
                                         int hightLightStringIndex,
                                         bool allowInsertText = false) override;
#else
    [[nodiscard]] QString addHighlighter(const QString &str,
                                         const QString &language,
                                         const QString &searchText,
                                         const QByteArray &uuid,
                                         int &blockCodeIndex,
                                         int &numberOfTextSearched,
                                         int hightLightStringIndex) override;
#endif

private:
    TextConverter::ConvertMessageTextSettings *mSettings = nullptr;
};
