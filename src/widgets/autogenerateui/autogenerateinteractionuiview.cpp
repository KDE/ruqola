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
AutoGenerateInteractionUiView::AutoGenerateInteractionUiView(QObject *parent)
    : QObject(parent)
{
}

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
    if (!mBlocks) {
        mBlocks = new AutoGenerateInteractionUiViewBlocks(this);
    }
    mBlocks->parse(json["blocks"_L1].toArray());
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

AutoGenerateInteractionUiViewBlocks *AutoGenerateInteractionUiView::blocks() const
{
    return mBlocks;
}

void AutoGenerateInteractionUiView::setBlocks(AutoGenerateInteractionUiViewBlocks *newBlocks)
{
    mBlocks = newBlocks;
}

void AutoGenerateInteractionUiView::generateWidget(QWidget *widget)
{
    // TODO customize title
    widget->setWindowTitle(mTitle.generateText());
    auto mainLayout = new QVBoxLayout(widget);
    if (!mBlocks) {
        mBlocks = new AutoGenerateInteractionUiViewBlocks(this);
    }
    mBlocks->generateWidget(widget);
    if (mCloseButton || mSubmitButton) {
        auto buttonDialog = new QDialogButtonBox(widget);
        if (mCloseButton) {
            auto closeButton = buttonDialog->addButton(QDialogButtonBox::Close);
            closeButton->setText(mCloseButton->text().generateText());
            connect(closeButton, &QPushButton::clicked, this, &AutoGenerateInteractionUiView::closed);
        }
        if (mSubmitButton) {
            auto okButton = buttonDialog->addButton(QDialogButtonBox::Ok);
            okButton->setText(mSubmitButton->text().generateText());
            connect(okButton, &QPushButton::clicked, this, &AutoGenerateInteractionUiView::submited);
        }
        mainLayout->addStretch(1);
        mainLayout->addWidget(buttonDialog);
    }
}

QJsonObject AutoGenerateInteractionUiView::serialize()
{
    // TODO
    return {};
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

#include "moc_autogenerateinteractionuiview.cpp"
