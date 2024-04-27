/****************************************************************************
** Meta object code from reading C++ file 'emu_server.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../SYS/server/emu_server.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'emu_server.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_emu_server_t {
    QByteArrayData data[14];
    char stringdata0[138];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_emu_server_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_emu_server_t qt_meta_stringdata_emu_server = {
    {
QT_MOC_LITERAL(0, 0, 10), // "emu_server"
QT_MOC_LITERAL(1, 11, 13), // "s_socket_init"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 11), // "onm_new_emu"
QT_MOC_LITERAL(4, 38, 4), // "name"
QT_MOC_LITERAL(5, 43, 11), // "emu_socket*"
QT_MOC_LITERAL(6, 55, 6), // "socket"
QT_MOC_LITERAL(7, 62, 13), // "abstract_emu*"
QT_MOC_LITERAL(8, 76, 7), // "emu_ptr"
QT_MOC_LITERAL(9, 84, 16), // "onm_close_socket"
QT_MOC_LITERAL(10, 101, 3), // "ptr"
QT_MOC_LITERAL(11, 105, 13), // "onm_close_emu"
QT_MOC_LITERAL(12, 119, 8), // "emu_name"
QT_MOC_LITERAL(13, 128, 9) // "onm_start"

    },
    "emu_server\0s_socket_init\0\0onm_new_emu\0"
    "name\0emu_socket*\0socket\0abstract_emu*\0"
    "emu_ptr\0onm_close_socket\0ptr\0onm_close_emu\0"
    "emu_name\0onm_start"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_emu_server[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    3,   40,    2, 0x08 /* Private */,
       9,    1,   47,    2, 0x08 /* Private */,
      11,    1,   50,    2, 0x08 /* Private */,
      13,    0,   53,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 5, 0x80000000 | 7,    4,    6,    8,
    QMetaType::Void, 0x80000000 | 5,   10,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,

       0        // eod
};

void emu_server::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<emu_server *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->s_socket_init(); break;
        case 1: _t->onm_new_emu((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< emu_socket*(*)>(_a[2])),(*reinterpret_cast< abstract_emu*(*)>(_a[3]))); break;
        case 2: _t->onm_close_socket((*reinterpret_cast< emu_socket*(*)>(_a[1]))); break;
        case 3: _t->onm_close_emu((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->onm_start(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< abstract_emu* >(); break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< emu_socket* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< emu_socket* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (emu_server::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&emu_server::s_socket_init)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject emu_server::staticMetaObject = { {
    QMetaObject::SuperData::link<QTcpServer::staticMetaObject>(),
    qt_meta_stringdata_emu_server.data,
    qt_meta_data_emu_server,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *emu_server::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *emu_server::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_emu_server.stringdata0))
        return static_cast<void*>(this);
    return QTcpServer::qt_metacast(_clname);
}

int emu_server::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void emu_server::s_socket_init()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
