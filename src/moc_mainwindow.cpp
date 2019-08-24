/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[17];
    char stringdata0[285];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 10), // "go_to_next"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 10), // "show_error"
QT_MOC_LITERAL(4, 34, 9), // "next_lock"
QT_MOC_LITERAL(5, 44, 24), // "on_actionStart_triggered"
QT_MOC_LITERAL(6, 69, 23), // "on_actionExit_triggered"
QT_MOC_LITERAL(7, 93, 10), // "set_tunnel"
QT_MOC_LITERAL(8, 104, 12), // "QMouseEvent*"
QT_MOC_LITERAL(9, 117, 11), // "set_barrier"
QT_MOC_LITERAL(10, 129, 12), // "change_state"
QT_MOC_LITERAL(11, 142, 22), // "on_stateButton_clicked"
QT_MOC_LITERAL(12, 165, 25), // "on_nextStepButton_clicked"
QT_MOC_LITERAL(13, 191, 24), // "on_preStepButton_clicked"
QT_MOC_LITERAL(14, 216, 24), // "on_RestartButton_clicked"
QT_MOC_LITERAL(15, 241, 21), // "on_AutoButton_clicked"
QT_MOC_LITERAL(16, 263, 21) // "on_WashButton_clicked"

    },
    "MainWindow\0go_to_next\0\0show_error\0"
    "next_lock\0on_actionStart_triggered\0"
    "on_actionExit_triggered\0set_tunnel\0"
    "QMouseEvent*\0set_barrier\0change_state\0"
    "on_stateButton_clicked\0on_nextStepButton_clicked\0"
    "on_preStepButton_clicked\0"
    "on_RestartButton_clicked\0on_AutoButton_clicked\0"
    "on_WashButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x0a /* Public */,
       3,    1,   85,    2, 0x0a /* Public */,
       4,    1,   88,    2, 0x0a /* Public */,
       5,    0,   91,    2, 0x08 /* Private */,
       6,    0,   92,    2, 0x08 /* Private */,
       7,    1,   93,    2, 0x08 /* Private */,
       9,    1,   96,    2, 0x08 /* Private */,
      10,    0,   99,    2, 0x08 /* Private */,
      11,    0,  100,    2, 0x08 /* Private */,
      12,    0,  101,    2, 0x08 /* Private */,
      13,    0,  102,    2, 0x08 /* Private */,
      14,    0,  103,    2, 0x08 /* Private */,
      15,    0,  104,    2, 0x08 /* Private */,
      16,    0,  105,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->go_to_next(); break;
        case 1: _t->show_error((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->next_lock((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_actionStart_triggered(); break;
        case 4: _t->on_actionExit_triggered(); break;
        case 5: _t->set_tunnel((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 6: _t->set_barrier((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 7: _t->change_state(); break;
        case 8: _t->on_stateButton_clicked(); break;
        case 9: _t->on_nextStepButton_clicked(); break;
        case 10: _t->on_preStepButton_clicked(); break;
        case 11: _t->on_RestartButton_clicked(); break;
        case 12: _t->on_AutoButton_clicked(); break;
        case 13: _t->on_WashButton_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
