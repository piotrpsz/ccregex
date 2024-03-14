//
// Created by Piotr Pszczółkowski on 14/03/2024.
//
#include "Component.h"
#include "Editor.h"
#include <QBoxLayout>
#include <QLabel>

Component::Component(QString const& title, QWidget *parent) :
        QWidget(parent),
        editor_{new Editor}
{
    auto description{new QLabel{title}};
    auto font = description->font();
    font.setPointSize(11);
    description->setFont(font);

    auto header_layout{new QHBoxLayout};
    header_layout->setContentsMargins(4, 4, 0, 4);
    header_layout->setSpacing(0);
    header_layout->addWidget(description);
    header_layout->addStretch();

    auto main_layout{new QVBoxLayout};
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->addLayout(header_layout);
    main_layout->addWidget(editor_);

    setLayout(main_layout);
}
