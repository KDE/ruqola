/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Repository>

class SyntaxHighlightingManager
{
public:
    ~SyntaxHighlightingManager() = default;

    static SyntaxHighlightingManager *self();

    [[nodiscard]] bool syntaxHighlightingInitialized() const;

    [[nodiscard]] KSyntaxHighlighting::Definition def(const QString &name) const;
    [[nodiscard]] KSyntaxHighlighting::Definition defaultDef() const;

    KSyntaxHighlighting::Repository &repo() const;

private:
    SyntaxHighlightingManager();
    void initialize();
    mutable KSyntaxHighlighting::Repository mRepo;
    KSyntaxHighlighting::Definition mDefaultDef;
    QList<QString> mDefinitions;
    bool mSyntaxHighlightingInitialized = false;
};
