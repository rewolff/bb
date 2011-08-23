
#include <QObject>
#include <QProcess>


class bb_mon_bool : public QObject
{
  Q_OBJECT

public:
  bb_mon_bool(QWidget *parent, const QString &name);

  int value() const { return m_value; }
  
public slots:
  void setValue(int ival);

private slots:
  void data_available (void);
  
signals:
  void valueChanged(bool newValue);

private:
  QProcess *monProcess;
  QProcess *setProcess;
  int m_value;
};

class bb_mon_int : public QObject
{
  Q_OBJECT

public:
  bb_mon_int(QWidget *parent, const QString &name);

  int value() const { return m_value; }
  
public slots:
  void setValue(int val);

private slots:
  void data_available (void);
  
signals:
  void valueChanged(int newValue);

private:
  QProcess *monProcess;
  QProcess *setProcess;
  int m_value;
};
