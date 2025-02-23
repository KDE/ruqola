/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiview.h"
#include <QJsonArray>

using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiView::AutoGenerateInteractionUiView() = default;

AutoGenerateInteractionUiView::~AutoGenerateInteractionUiView() = default;

void AutoGenerateInteractionUiView::parseView(const QJsonObject &json)
{
    mCloseButton.parse(json["close"_L1].toObject());
    mSubmitButton.parse(json["submit"_L1].toObject());
    mShowIcon = json["showIcon"_L1].toBool();
    mId = json["id"_L1].toString().toLatin1();
    mTitle.parse(json["title"_L1].toObject());
    mBlocks.parse(json["blocks"_L1].toArray());
}

QByteArray AutoGenerateInteractionUiView::id() const
{
    return mId;
}

void AutoGenerateInteractionUiView::setId(const QByteArray &newId)
{
    mId = newId;
}

bool AutoGenerateInteractionUiView::operator==(const AutoGenerateInteractionUiView &other) const
{
    return other.id() == id() && other.showIcon() == showIcon();
}

bool AutoGenerateInteractionUiView::showIcon() const
{
    return mShowIcon;
}

void AutoGenerateInteractionUiView::setShowIcon(bool newShowIcon)
{
    mShowIcon = newShowIcon;
}

AutoGenerateInteractionUiViewButtonElement AutoGenerateInteractionUiView::closeButton() const
{
    return mCloseButton;
}

void AutoGenerateInteractionUiView::setCloseButton(const AutoGenerateInteractionUiViewButtonElement &newCloseButton)
{
    mCloseButton = newCloseButton;
}

AutoGenerateInteractionUiViewButtonElement AutoGenerateInteractionUiView::submitButton() const
{
    return mSubmitButton;
}

void AutoGenerateInteractionUiView::setSubmitButton(const AutoGenerateInteractionUiViewButtonElement &newSubmitButton)
{
    mSubmitButton = newSubmitButton;
}

AutoGenerateInteractionUiViewText AutoGenerateInteractionUiView::title() const
{
    return mTitle;
}

void AutoGenerateInteractionUiView::setTitle(const AutoGenerateInteractionUiViewText &newTitle)
{
    mTitle = newTitle;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiView &t)
{
    d.space() << "id:" << t.id();
    d.space() << "showIcon:" << t.showIcon();
    d.space() << "closeButton:" << t.closeButton();
    d.space() << "submitButton:" << t.submitButton();
    d.space() << "title:" << t.title();
    return d;
}
