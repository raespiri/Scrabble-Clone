/****************************************************************************
** Meta object code from reading C++ file 'Welcome.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Welcome.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Welcome.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Welcome[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x0a,
      21,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Welcome[] = {
    "Welcome\0\0addPlayer()\0openNewWindow()\0"
};

void Welcome::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Welcome *_t = static_cast<Welcome *>(_o);
        switch (_id) {
        case 0: _t->addPlayer(); break;
        case 1: _t->openNewWindow(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Welcome::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Welcome::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Welcome,
      qt_meta_data_Welcome, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Welcome::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Welcome::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Welcome::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Welcome))
        return static_cast<void*>(const_cast< Welcome*>(this));
    return QWidget::qt_metacast(_clname);
}

int Welcome::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
