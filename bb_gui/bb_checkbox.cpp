#include "QtGui/QWidget"
#include "Qt/qpushbutton.h"
#include "Qt/qradiobutton.h"
#include "Qt/qcheckbox.h"
#include "Qt/qslider.h"
#include "Qt/qspinbox.h"
#include <QApplication>
#include <Qt/qobject.h>
#include <QObject>
#include <iostream> 

#include <stdio.h>

#include <QProcess>

//#include "bb_mon.h"
#include "bb_checkbox.h"


bb_checkbox::bb_checkbox (QWidget *parent, const QString &name, int x, int y)
{
  m_value = 0;

  BB_mon = new bb_mon_bool (parent, name);
  checkbox = new QCheckBox(name, parent);

  checkbox->move(x, y);
  checkbox->show();

  QObject::connect(BB_mon, SIGNAL(valueChanged(bool)),
		   checkbox, SLOT(setChecked(bool)));

  QObject::connect(checkbox, SIGNAL(stateChanged(int)),
		   BB_mon,   SLOT(setValue(int)));

  QObject::connect(checkbox, SIGNAL(stateChanged(int)),
		   this,     SLOT(setValue(int)));
}


void bb_checkbox::setValue (int ival)
{
  bool val;

  val = (bool) ival;
  printf ("setvalue called %d %d.\n", m_value,  val);
  if (val == m_value) return;

  m_value = val;
  emit valueChanged (val);

  BB_mon->setValue (ival);
  checkbox->setChecked (val);
}


