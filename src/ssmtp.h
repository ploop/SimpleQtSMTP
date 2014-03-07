/*
** SimpleQtSMTP
**
** Simple SMTP client and MIME-text generator
** to send email from Qt-applications
**
** Copyright (c) 2014 - Balykin Yuriy aka ploop
**
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
*/


#ifndef SSMTP_H
#define SSMTP_H

#include <QTcpSocket>
#include <QSslSocket>
#include "mimemessage.h"

class SSMTP
{
public:
  SSMTP(QString host, quint16 port = 25, bool ssl = false);
  ~SSMTP();

  void setLogin(const QString &login);
  void setPass(const QString &pass);
  void setName(const QString &name);
  void setConnectionTimeout(int timeout);

  QString getLog();

  bool send(MIMEmessage *message);

protected:
  bool tcpConnect();
  void tcpDisconnect();
  bool checkRespCode(int code);
  bool sendCmd(const QString &cmd, int respCode);
  bool waitForResponse(bool isMessage = false);
  bool sendText(const QString &text, bool isMessage = false);
  void writeLog(const QString &str);

  QTcpSocket *socket;
  MIMEmessage *message;
  QString smtp_host;
  quint16 smtp_port;
  QString smtp_user;
  QString smtp_pass;
  QString smtp_name;
  QString responseText;
  QString log;
  bool ssl;
  int responseCode;
  int connectionTimeout;
  int commandRespTimeout;
  int messageRespTimeout;
  int writeTimeout;
};

#endif // SSMTP_H
