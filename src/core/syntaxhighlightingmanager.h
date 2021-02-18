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

#pragma once

#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Repository>
#include <QObject>

class SyntaxHighlightingManager : public QObject
{
    Q_OBJECT
public:
    explicit SyntaxHighlightingManager(QObject *parent = nullptr);
    ~SyntaxHighlightingManager() override = default;

    static SyntaxHighlightingManager *self();

    Q_REQUIRED_RESULT bool syntaxHighlightingInitialized() const;

    Q_REQUIRED_RESULT KSyntaxHighlighting::Definition def(const QString &name) const;
    Q_REQUIRED_RESULT KSyntaxHighlighting::Definition defaultDef() const;

    KSyntaxHighlighting::Repository &repo() const;

private:
    Q_DISABLE_COPY(SyntaxHighlightingManager)
    void initialize();
    mutable KSyntaxHighlighting::Repository mRepo;
    KSyntaxHighlighting::Definition mDefaultDef;
    QVector<QString> mDefinitions;
    bool mSyntaxHighlightingInitialized = false;
};

