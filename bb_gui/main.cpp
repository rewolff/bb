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

//#include <QProcess>

//#include "bb_mon.h"
#include "bb_checkbox.h"


int main (int argc, char **argv)
{
  QApplication app(argc, argv);

  QWidget *window = new QWidget();
  window->resize(320, 240);
  window->show();

  //bb_mon_bool *BB_mon = new bb_mon_bool (window, "led6");

  QRadioButton *button = new QRadioButton("Press me", window);
  button->move(100, 100);
  button->show();

  QSlider *slider = new QSlider(Qt::Horizontal,window);
  slider->move(100, 160);
  slider->show();

  QSpinBox *spinbox = new QSpinBox (window);
  spinbox->move(100, 190);
  spinbox->show();

  bb_checkbox *bb_cb7 = new bb_checkbox (window, "led7", 100,50);
  bb_checkbox *bb_cb6 = new bb_checkbox (window, "led6", 100,70);
  bb_checkbox *bb_cb8 = new bb_checkbox (window, "testbitje", 100,130);

  app.exec();
  exit (0);
}
