/****************************************************************************
** Meta object code from reading C++ file 'bb_mon.h'
**
** Created: Thu Dec 8 10:56:16 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "bb_mon.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bb_mon.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_bb_mon_bool[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      22,   13,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      46,   41,   12,   12, 0x0a,
      60,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_bb_mon_bool[] = {
    "bb_mon_bool\0\0newValue\0valueChanged(bool)\0"
    "ival\0setValue(int)\0data_available()\0"
};

const QMetaObject bb_mon_bool::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_bb_mon_bool,
      qt_meta_data_bb_mon_bool, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &bb_mon_bool::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *bb_mon_bool::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *bb_mon_bool::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_bb_mon_bool))
        return static_cast<void*>(const_cast< bb_mon_bool*>(this));
    return QObject::qt_metacast(_clname);
}

int bb_mon_bool::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: valueChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: setValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: data_available(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void bb_mon_bool::valueChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_bb_mon_int[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      43,   39,   11,   11, 0x0a,
      57,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_bb_mon_int[] = {
    "bb_mon_int\0\0newValue\0valueChanged(int)\0"
    "val\0setValue(int)\0data_available()\0"
};

const QMetaObject bb_mon_int::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_bb_mon_int,
      qt_meta_data_bb_mon_int, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &bb_mon_int::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *bb_mon_int::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *bb_mon_int::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_bb_mon_int))
        return static_cast<void*>(const_cast< bb_mon_int*>(this));
    return QObject::qt_metacast(_clname);
}

int bb_mon_int::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: setValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: data_available(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void bb_mon_int::valueChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
