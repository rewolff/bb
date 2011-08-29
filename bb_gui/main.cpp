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



int main (int argc, char **argv)
{
  QApplication app(argc, argv);

  QWidget *window = new QWidget();
  window->resize(320, 240);
  window->show();

  bb_mon_bool *BB_mon = new bb_mon_bool (window, "led6");

  QRadioButton *button = new QRadioButton("Press me", window);
  button->move(100, 100);
  button->show();

  QCheckBox *checkbox = new QCheckBox("Press me too", window);
  checkbox->move(100, 130);
  checkbox->show();

  QSlider *slider = new QSlider(Qt::Horizontal,window);
  slider->move(100, 160);
  slider->show();

  QSpinBox *spinbox = new QSpinBox (window);
  spinbox->move(100, 190);
  spinbox->show();

  

  QObject::connect(BB_mon, SIGNAL(valueChanged(bool)),
		   checkbox, SLOT(setChecked(bool)));

  QObject::connect(checkbox, SIGNAL(stateChanged(int)),
		   BB_mon,   SLOT(setValue(int)));

  QObject::connect(slider, SIGNAL(valueChanged(int)),
		   spinbox, SLOT(setValue(int)));
  QObject::connect(spinbox, SIGNAL(valueChanged(int)),
		   slider, SLOT(setValue(int)));

  app.exec();
  printf ("exec done\n");
  exit (0);
}