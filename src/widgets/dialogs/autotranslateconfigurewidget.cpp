/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "autotranslateconfigurewidget.h"
#include "model/autotranslatelanguagesmodel.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "room.h"
#include <QVBoxLayout>
#include <KLocalizedString>
#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>

AutoTranslateConfigureWidget::AutoTranslateConfigureWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mAutoTranslate = new QCheckBox(i18n("Auto-Translate"), this);
    mAutoTranslate->setObjectName(QStringLiteral("mAutoTranslate"));
    mainLayout->addWidget(mAutoTranslate);
    connect(mAutoTranslate, &QCheckBox::clicked, this, &AutoTranslateConfigureWidget::slotChangeAutoTranslate);

    auto *horizontalLayout = new QHBoxLayout;
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    horizontalLayout->setContentsMargins({});
    mainLayout->addLayout(horizontalLayout);

    QLabel *label = new QLabel(i18n("Language:"), this);
    label->setObjectName(QStringLiteral("label"));
    label->setTextFormat(Qt::PlainText);
    horizontalLayout->addWidget(label);

    mLanguage = new QComboBox(this);
    mLanguage->setObjectName(QStringLiteral("mLanguage"));
    mLanguage->setModel(Ruqola::self()->rocketChatAccount()->autoTranslateLanguagesModel());
    connect(mLanguage, QOverload<int>::of(&QComboBox::activated),
            this, &AutoTranslateConfigureWidget::slotLanguageChanged);

    horizontalLayout->addWidget(mLanguage);

    mainLayout->addStretch(1);
}

AutoTranslateConfigureWidget::~AutoTranslateConfigureWidget()
{
}

Room *AutoTranslateConfigureWidget::room() const
{
    return mRoom;
}

void AutoTranslateConfigureWidget::slotLanguageChanged(int index)
{
    Ruqola::self()->rocketChatAccount()->autoTranslateSaveLanguageSettings(mRoom->roomId(), Ruqola::self()->rocketChatAccount()->autoTranslateLanguagesModel()->selectedLanguage(index));
}

void AutoTranslateConfigureWidget::slotChangeAutoTranslate(bool status)
{
    Ruqola::self()->rocketChatAccount()->autoTranslateSaveAutoTranslateSettings(mRoom->roomId(), status);
}

void AutoTranslateConfigureWidget::slotAutoTranslateChanged()
{
    mAutoTranslate->setChecked(mRoom->autoTranslate());
}

void AutoTranslateConfigureWidget::slotAutoTranslateLanguageChanged()
{
    mLanguage->setCurrentIndex(Ruqola::self()->rocketChatAccount()->autoTranslateLanguagesModel()->currentLanguage(mRoom->autoTranslateLanguage()));
}

void AutoTranslateConfigureWidget::setRoom(Room *room)
{
    mRoom = room;
    connect(mRoom, &Room::autoTranslateChanged, this, &AutoTranslateConfigureWidget::slotAutoTranslateChanged);
    connect(mRoom, &Room::autoTranslateLanguageChanged, this, &AutoTranslateConfigureWidget::slotAutoTranslateLanguageChanged);
    slotAutoTranslateChanged();
    slotAutoTranslateLanguageChanged();
}
