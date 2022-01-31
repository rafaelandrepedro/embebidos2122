/****************************************************************************
** Meta object code from reading C++ file 'wifi.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.1.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../App/wifi.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wifi.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.1.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Wifi_t {
    const uint offsetsAndSize[12];
    char stringdata0[58];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Wifi_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Wifi_t qt_meta_stringdata_Wifi = {
    {
QT_MOC_LITERAL(0, 4), // "Wifi"
QT_MOC_LITERAL(5, 4), // "init"
QT_MOC_LITERAL(10, 0), // ""
QT_MOC_LITERAL(11, 15), // "connectToServer"
QT_MOC_LITERAL(27, 12), // "sendToServer"
QT_MOC_LITERAL(40, 17) // "receiveFromServer"

    },
    "Wifi\0init\0\0connectToServer\0sendToServer\0"
    "receiveFromServer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Wifi[] = {

 // content:
       9,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   38,    2, 0x0a,    0 /* Public */,
       3,    0,   39,    2, 0x0a,    1 /* Public */,
       4,    1,   40,    2, 0x0a,    2 /* Public */,
       5,    0,   43,    2, 0x0a,    4 /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,

       0        // eod
};

void Wifi::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Wifi *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->init(); break;
        case 1: _t->connectToServer(); break;
        case 2: _t->sendToServer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->receiveFromServer(); break;
        default: ;
        }
    }
}

const QMetaObject Wifi::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Wifi.offsetsAndSize,
    qt_meta_data_Wifi,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Wifi_t

, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *Wifi::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Wifi::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Wifi.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Wifi::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
