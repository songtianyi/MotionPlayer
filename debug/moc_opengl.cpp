/****************************************************************************
** Meta object code from reading C++ file 'opengl.h'
**
** Created: Tue May 14 10:03:14 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../opengl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'opengl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_OpenGL[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      46,    8,    7,    7, 0x05,
      87,   79,    7,    7, 0x05,
     121,  115,    7,    7, 0x05,
     146,  144,    7,    7, 0x25,

 // slots: signature, parameters, type, tag, flags
     165,    7,    7,    7, 0x0a,
     184,    7,    7,    7, 0x0a,
     203,    7,    7,    7, 0x0a,
     224,    7,    7,    7, 0x0a,
     243,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_OpenGL[] = {
    "OpenGL\0\0index,sliderValue,sliderMax,dialValue\0"
    "initDataChanged(int,int,int,int)\0"
    "index,v\0sliderValueChanged(int,int)\0"
    "s,sec\0statusStr(QString,int)\0s\0"
    "statusStr(QString)\0setCurrFrameA(int)\0"
    "setCurrFrameB(int)\0setTimeInterval(int)\0"
    "setGroundSize(int)\0commonUpdate()\0"
};

const QMetaObject OpenGL::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_OpenGL,
      qt_meta_data_OpenGL, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OpenGL::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OpenGL::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OpenGL::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OpenGL))
        return static_cast<void*>(const_cast< OpenGL*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int OpenGL::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: initDataChanged((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4]))); break;
        case 1: sliderValueChanged((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 2: statusStr((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: statusStr((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: setCurrFrameA((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: setCurrFrameB((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: setTimeInterval((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: setGroundSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: commonUpdate(); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void OpenGL::initDataChanged(const int _t1, const int _t2, const int _t3, const int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void OpenGL::sliderValueChanged(const int _t1, const int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void OpenGL::statusStr(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
