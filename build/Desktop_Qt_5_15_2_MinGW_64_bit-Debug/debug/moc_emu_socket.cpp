/****************************************************************************
** Meta object code from reading C++ file 'emu_socket.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../CMU/tcp_socket/emu_socket.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'emu_socket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_emu_socket_t {
    QByteArrayData data[18];
    char stringdata0[197];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_emu_socket_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_emu_socket_t qt_meta_stringdata_emu_socket = {
    {
QT_MOC_LITERAL(0, 0, 10), // "emu_socket"
QT_MOC_LITERAL(1, 11, 9), // "s_new_msg"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 4), // "data"
QT_MOC_LITERAL(4, 27, 9), // "s_new_emu"
QT_MOC_LITERAL(5, 37, 4), // "name"
QT_MOC_LITERAL(6, 42, 11), // "emu_socket*"
QT_MOC_LITERAL(7, 54, 6), // "socket"
QT_MOC_LITERAL(8, 61, 13), // "abstract_emu*"
QT_MOC_LITERAL(9, 75, 3), // "emu"
QT_MOC_LITERAL(10, 79, 7), // "s_close"
QT_MOC_LITERAL(11, 87, 3), // "ptr"
QT_MOC_LITERAL(12, 91, 20), // "onm_deal_socket_data"
QT_MOC_LITERAL(13, 112, 24), // "onm_transfer_socket_data"
QT_MOC_LITERAL(14, 137, 15), // "onm_start_timer"
QT_MOC_LITERAL(15, 153, 18), // "onm_timer_callback"
QT_MOC_LITERAL(16, 172, 14), // "onm_stop_timer"
QT_MOC_LITERAL(17, 187, 9) // "onm_close"

    },
    "emu_socket\0s_new_msg\0\0data\0s_new_emu\0"
    "name\0emu_socket*\0socket\0abstract_emu*\0"
    "emu\0s_close\0ptr\0onm_deal_socket_data\0"
    "onm_transfer_socket_data\0onm_start_timer\0"
    "onm_timer_callback\0onm_stop_timer\0"
    "onm_close"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_emu_socket[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       4,    3,   62,    2, 0x06 /* Public */,
      10,    1,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,   72,    2, 0x0a /* Public */,
      13,    0,   73,    2, 0x0a /* Public */,
      14,    0,   74,    2, 0x0a /* Public */,
      15,    0,   75,    2, 0x0a /* Public */,
      16,    0,   76,    2, 0x0a /* Public */,
      17,    0,   77,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 6, 0x80000000 | 8,    5,    7,    9,
    QMetaType::Void, 0x80000000 | 6,   11,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void emu_socket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<emu_socket *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->s_new_msg((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 1: _t->s_new_emu((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< emu_socket*(*)>(_a[2])),(*reinterpret_cast< abstract_emu*(*)>(_a[3]))); break;
        case 2: _t->s_close((*reinterpret_cast< emu_socket*(*)>(_a[1]))); break;
        case 3: _t->onm_deal_socket_data(); break;
        case 4: _t->onm_transfer_socket_data(); break;
        case 5: _t->onm_start_timer(); break;
        case 6: _t->onm_timer_callback(); break;
        case 7: _t->onm_stop_timer(); break;
        case 8: _t->onm_close(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
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
            using _t = void (emu_socket::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&emu_socket::s_new_msg)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (emu_socket::*)(QString , emu_socket * , abstract_emu * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&emu_socket::s_new_emu)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (emu_socket::*)(emu_socket * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&emu_socket::s_close)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject emu_socket::staticMetaObject = { {
    QMetaObject::SuperData::link<QTcpSocket::staticMetaObject>(),
    qt_meta_stringdata_emu_socket.data,
    qt_meta_data_emu_socket,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *emu_socket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *emu_socket::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_emu_socket.stringdata0))
        return static_cast<void*>(this);
    return QTcpSocket::qt_metacast(_clname);
}

int emu_socket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpSocket::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void emu_socket::s_new_msg(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void emu_socket::s_new_emu(QString _t1, emu_socket * _t2, abstract_emu * _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void emu_socket::s_close(emu_socket * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
