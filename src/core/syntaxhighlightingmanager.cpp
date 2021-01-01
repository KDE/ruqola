/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "syntaxhighlightingmanager.h"
#include "ruqola_debug.h"

namespace {
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
