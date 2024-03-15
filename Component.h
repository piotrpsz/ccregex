//
// Created by Piotr Pszczółkowski on 14/03/2024.
//

#pragma once

/*------- include files:
-------------------------------------------------------------------*/
#include "Types.h"
#include "Editor.h"
#include <QWidget>

/*------- forward declarations:
-------------------------------------------------------------------*/
class Editor;

/*------- class:
-------------------------------------------------------------------*/
class Component : public QWidget {
public:
    explicit Component(qstr const& title, QWidget* parent = nullptr);
    ~Component() override = default;

    QString content() const noexcept {
        return editor_->content();
    }
private:
    Editor* const editor_;
};
