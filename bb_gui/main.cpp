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

int parse_file (QWidget *window, char *fname)
{
  FILE *f;
  char buf[1024];
  char command[0x40], arg[0x40];
  int pos = 30;

  f = fopen (fname, "r");
  while (fgets (buf, 1024, f)) {
     if (buf [0] == '#') continue;
     sscanf (buf, "%s %s", command, arg);
     if (strcmp (command, "checkbox") == 0) {
        bb_checkbox *bla = new bb_checkbox (window, arg, 20, pos);
        pos += 20; 
     } else {
       printf ("syntax error in config file.\n");
       exit (1);
     }   
  }
  return pos;
}


int main (int argc, char **argv)
{
  QApplication app(argc, argv);
  int endpos; 

  QWidget *window = new QWidget();

  endpos = 20;
  if (argc > 1) 
      endpos = parse_file (window, argv[1]);

  window->resize(320, endpos+30);
  window->show();

#if 0
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
#endif

  app.exec();
  exit (0);
}
