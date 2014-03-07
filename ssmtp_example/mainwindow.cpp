#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  connect(ui->send,SIGNAL(clicked()),this,SLOT(slotSend()));
  connect(ui->open_attach,SIGNAL(clicked()),this,SLOT(slotOpenFile()));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::slotSend()
{
  SSMTP cln(ui->server->text(),
            ui->port->text().toInt(),
            ui->ssl->isChecked());

  cln.setLogin(ui->login->text());
  cln.setPass(ui->pass->text());

  MIMEmessage message;

  message.setSender(ui->from->text());
  message.setSubject(ui->subj->text());
  message.addRcpt(ui->to->text());

  message.addAlternative(ui->textEdit->toPlainText(),
                         ui->textEdit->toHtml());
  message.addAttach(ui->attach->text());

  /* sending message */
  cln.send(&message);

}

void MainWindow::slotOpenFile()
{
  ui->attach->setText(
        QFileDialog::getOpenFileName(this,"All files",QDir::homePath(), "*.*"));
}
