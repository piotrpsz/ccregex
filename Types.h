//
// Created by Piotr Pszczółkowski on 15/03/2024.
//
#pragma once
#include <QVariant>
#include <QString>
#include <QSet>
#include <QList>

/*------- include files:
-------------------------------------------------------------------*/
using i32 = qint32;
using isize = qsizetype;
using qstr = QString;
using qvar = QVariant;

template<typename T>
    using qvec = QVector<T>;
template<typename T>
    using qset = QSet<T>;
template<typename K, typename V>
    using qhash = QHash<K,V>;
template<typename T>
    using qlist = QList<T>;
