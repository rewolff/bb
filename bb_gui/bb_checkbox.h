
#include <QObject>
#include <QProcess>
#include "Qt/qcheckbox.h"
#include "bb_mon.h"


class bb_checkbox : public QObject
{
  Q_OBJECT

public:
  bb_checkbox(QWidget *parent, const QString &name, int x, int y);

  int value() const { return m_value; }

public slots:
  void setValue(int ival);

signals:
  void valueChanged(bool newValue);

private:
  QCheckBox *checkbox;
  //QProcess *BB_mon;
  bb_mon_bool *BB_mon;
  int m_value;
};


