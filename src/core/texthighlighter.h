/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <KSyntaxHighlighting/AbstractHighlighter>

class QTextStream;

class TextHighlighter : public KSyntaxHighlighting::AbstractHighlighter
{
public:
    explicit TextHighlighter(QTextStream *stream);
    ~TextHighlighter() override = default;

    void highlight(const QString &str);

protected:
    void applyFormat(int offset, int length, const KSyntaxHighlighting::Format &format) override;

private:
    QString mCurrentLine;
    QTextStream *const mStream;
};
