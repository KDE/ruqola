/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "textconverter.h"
#include <QRegularExpression>
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
                                         int &numberOfTextSearched,
                                         int hightLightStringIndex) override;

private:
    void regenerateSearchText();
    TextConverter::ConvertMessageTextSettings *mSettings = nullptr;
    QRegularExpression mSearchRegularExpression;
    QString mSearchText;
};
