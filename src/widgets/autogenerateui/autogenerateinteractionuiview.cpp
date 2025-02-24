/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiview.h"
#include <QDialogButtonBox>
#include <QJsonArray>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiView::AutoGenerateInteractionUiView() = default;

AutoGenerateInteractionUiView::~AutoGenerateInteractionUiView()
{
    delete mCloseButton;
    delete mSubmitButton;
}

void AutoGenerateInteractionUiView::parseView(const QJsonObject &json)
{
    if (json.contains("close"_L1)) {
        mCloseButton = new AutoGenerateInteractionUiViewButtonElement;
        mCloseButton->parse(json["close"_L1].toObject());
    }
    if (json.contains("submit"_L1)) {
        mSubmitButton = new AutoGenerateInteractionUiViewButtonElement;
        mSubmitButton->parse(json["submit"_L1].toObject());
    }
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
    return other.id() == id() && other.showIcon() == showIcon() && *other.closeButton() == *closeButton() && *other.submitButton() == *submitButton()
        && other.title() == title() && other.blocks() == blocks();
}

bool AutoGenerateInteractionUiView::showIcon() const
{
    return mShowIcon;
}

void AutoGenerateInteractionUiView::setShowIcon(bool newShowIcon)
{
    mShowIcon = newShowIcon;
}

AutoGenerateInteractionUiViewButtonElement *AutoGenerateInteractionUiView::closeButton() const
{
    return mCloseButton;
}

void AutoGenerateInteractionUiView::setCloseButton(AutoGenerateInteractionUiViewButtonElement *newCloseButton)
{
    mCloseButton = newCloseButton;
}

AutoGenerateInteractionUiViewButtonElement *AutoGenerateInteractionUiView::submitButton() const
{
    return mSubmitButton;
}

void AutoGenerateInteractionUiView::setSubmitButton(AutoGenerateInteractionUiViewButtonElement *newSubmitButton)
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

AutoGenerateInteractionUiViewBlocks AutoGenerateInteractionUiView::blocks() const
{
    return mBlocks;
}

void AutoGenerateInteractionUiView::setBlocks(const AutoGenerateInteractionUiViewBlocks &newBlocks)
{
    mBlocks = newBlocks;
}

void AutoGenerateInteractionUiView::generateWidget(QWidget *widget)
{
    // TODO customize title
    widget->setWindowTitle(mTitle.text());
    auto mainLayout = new QVBoxLayout(widget);
    mBlocks.generateWidget(widget);
    if (mCloseButton || mSubmitButton) {
        auto buttonDialog = new QDialogButtonBox(widget);
        // TODO connect button
        if (mCloseButton) {
            auto closeButton = buttonDialog->addButton(QDialogButtonBox::Close);
            closeButton->setText(mCloseButton->text().text());
        }
        if (mSubmitButton) {
            auto okButton = buttonDialog->addButton(QDialogButtonBox::Ok);
            okButton->setText(mSubmitButton->text().text());
        }
        mainLayout->addStretch(1);
        mainLayout->addWidget(buttonDialog);
    }
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiView &t)
{
    d.space() << "id:" << t.id();
    d.space() << "showIcon:" << t.showIcon();
    d.space() << "closeButton:" << *t.closeButton();
    d.space() << "submitButton:" << *t.submitButton();
    d.space() << "title:" << t.title();
    d.space() << "blocks:" << t.blocks();
    return d;
}
