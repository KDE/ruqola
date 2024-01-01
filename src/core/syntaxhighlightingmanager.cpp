/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "syntaxhighlightingmanager.h"
#include "ruqola_debug.h"

namespace
{
bool sortCaseInsensitively(const QString &lhs, const QString &rhs)
{
    return lhs.compare(rhs, Qt::CaseInsensitive) < 0;
}
}

SyntaxHighlightingManager::SyntaxHighlightingManager(QObject *parent)
    : QObject(parent)
{
    initialize();
}

void SyntaxHighlightingManager::initialize()
{
    mDefaultDef = mRepo.definitionForName(QStringLiteral("C++"));
    if (mDefaultDef.isValid()) {
        mSyntaxHighlightingInitialized = true;
        const auto definitions = mRepo.definitions();
        mDefinitions.reserve(definitions.size());
        for (const auto &definition : definitions) {
            mDefinitions.append(definition.name());
        }
        std::sort(mDefinitions.begin(), mDefinitions.end(), sortCaseInsensitively);
    } else {
        qCWarning(RUQOLA_LOG) << "Unable to find definition";
    }
}

KSyntaxHighlighting::Repository &SyntaxHighlightingManager::repo() const
{
    return mRepo;
}

KSyntaxHighlighting::Definition SyntaxHighlightingManager::def(const QString &name) const
{
    auto it = std::lower_bound(mDefinitions.begin(), mDefinitions.end(), name, sortCaseInsensitively);
    if (it != mDefinitions.end() && name.compare(*it, Qt::CaseInsensitive) == 0) {
        return mRepo.definitionForName(*it);
    }
    return {};
}

KSyntaxHighlighting::Definition SyntaxHighlightingManager::defaultDef() const
{
    return mDefaultDef;
}

bool SyntaxHighlightingManager::syntaxHighlightingInitialized() const
{
    return mSyntaxHighlightingInitialized;
}

SyntaxHighlightingManager *SyntaxHighlightingManager::self()
{
    static SyntaxHighlightingManager s_self;
    return &s_self;
}

#include "moc_syntaxhighlightingmanager.cpp"
