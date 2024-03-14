//
// Created by Piotr Pszczółkowski on 14/03/2024.
//

#pragma once

/*------- include files:
-------------------------------------------------------------------*/
#include <QWidget>
#include <QString>

/*------- forward declarations:
-------------------------------------------------------------------*/
class Editor;

/*------- class:
-------------------------------------------------------------------*/
class Component : public QWidget {
public:
    explicit Component(QString const& title, QWidget* parent = nullptr);
    ~Component() override = default;
private:
    Editor* const editor_;
};
