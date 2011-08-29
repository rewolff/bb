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

#include "bb_mon.h"


void bb_mon_bool::data_available (void)
{
  char data[0x100]; 
  int value;

  monProcess->read (data, 0x100);
  sscanf (data, "%d", &value);
  if (value != m_value) {
    m_value = value; 
    if (value)
      emit valueChanged (TRUE);
    else
      emit valueChanged (FALSE);
  }
}


bb_mon_bool::bb_mon_bool (QWidget *parent, const QString &name)
{ 
  m_value = 0; 
  QString mprogram = "/home/wolff/bb/bb_mon";
  QString sprogram = "/home/wolff/bb/bb_nom";
  QStringList arguments;
  arguments << name;

  monProcess = new QProcess (parent);
  monProcess->start (mprogram, arguments);

  QObject::connect (monProcess, SIGNAL (readyReadStandardOutput(void)), 
		    this, SLOT (data_available(void)));  

  setProcess = new QProcess (parent);
  setProcess->start (sprogram, arguments);
}


void bb_mon_bool::setValue (int ival)
{
  bool val;

  val = (bool) ival;
  //printf ("setvalue called %d %d.\n", m_value,  val);
  if (val == m_value) return;

  m_value = val;
  setProcess->write (val?"1\n":"0\n");
  emit valueChanged (val);
}

void bb_mon_int::data_available (void)
{
  char data[0x100]; 
  int value;

  monProcess->read (data, 0x100);
  sscanf (data, "%d", &value);
  setValue (value);
  //  if (value != m_value) {
  //  m_value = value;
  //  emit valueChanged (value);
  //}
}


bb_mon_int::bb_mon_int (QWidget *parent, const QString &name)
{ 
  m_value = 0;
  QString mprogram = "/home/wolff/bb/bb_mon";
  QString sprogram = "/home/wolff/bb/bb_nom";
  QStringList arguments;
  arguments << name;

  monProcess = new QProcess (parent);
  monProcess->start (mprogram, arguments);

  QObject::connect (monProcess, SIGNAL (readyReadStandardOutput(void)), 
		    this, SLOT (data_available(void)));  

  setProcess = new QProcess (parent);
  setProcess->start (sprogram, arguments);
}


void bb_mon_int::setValue (int val)
{
  char buf[0x100]; 

  if (val == m_value) return;

  m_value = val;
  sprintf (buf, "%d\n", val);
  setProcess->write (buf);
  emit valueChanged (val);
}

