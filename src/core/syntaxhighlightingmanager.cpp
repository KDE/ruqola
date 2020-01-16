/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

SyntaxHighlightingManager::SyntaxHighlightingManager(QObject *parent)
    : QObject(parent)
{
    initialize();
}

void SyntaxHighlightingManager::initialize()
{
    mDef = mRepo.definitionForName(QStringLiteral("C++"));
    if (mDef.isValid()) {
        mSyntaxHighlightingInitialized = true;
    } else {
        qCWarning(RUQOLA_LOG) << "Unable to find definition";
    }
}

KSyntaxHighlighting::Repository &SyntaxHighlightingManager::repo() const
{
    return mRepo;
}

KSyntaxHighlighting::Definition SyntaxHighlightingManager::def() const
{
    return mDef;
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
