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


#ifndef MIMEMESSAGE_H
#define MIMEMESSAGE_H

#include <QStringList>
#include <QDebug>
#include <QDate>
#include <QFileInfo>
#include <QCryptographicHash>

#define FORMAT_CNT 76
#define EMAIL_REGEXP "^[-a-z0-9!#$%&'*+/=?^_`{|}~]+(?:\\.[-a-z0-9!#$%&'*+/=?^_`{|}~]+)"\
  "*@(?:[a-z0-9]([-a-z0-9]{0,61}[a-z0-9])?\\.)*([a-z0-9]([-a-z0-9]{0,61}[a-z0-9])?\\.)[a-z][a-z]+$"

class MIMEmessage
{
public:
  MIMEmessage();

  bool setSender(const QString email, const QString &name); // format ("test@test.com", "Jon Smitt")
  bool setSender(const QString &name_and_email);            // format ("Jon Smitt <test@test.com>")
  bool addRcpt(const QString email, const QString &name);   // format ("test@test.com", "Jon Smitt")
  bool addRcpt(const QString &name_and_email);              // format ("Jon Smitt <test@test.com>")

  void setSubject(const QString &subj);
  void setMessageId(const QString &mess_id);
  void setUserAgent(const QString &user_agent);
  void setDate(const QDateTime dt);

  void addPlainTextPart(const QString &plain_text);
  void addHtmlPart(const QString &html_text);
  void addAlternative(const QString &plain_text, const QString &html_text);
  bool addAttach(const QString &filename);
  void addAttach(QByteArray *ba, const QString &name);

  void clearSender();
  void clearRcpt();
  void clearSubject();
  void clearMessageId();
  void clearUserAgent();
  void clearDate();
  void clearPlainText();
  void clearHtml();
  void clearAttachList();
  void clearMultipart();
  void clearAll();

  QStringList *rcptList();
  const QString getFromEmail();
  const QString getHash();

  const QString getMIMEText();

protected:
  const QString str2base64(const QString &str);
  const QString base64str(const QString &base64);
  const QString headerEncoder(const QString &str);
  const QString getNameFromFull(const QString &name_and_email);
  const QString getMailFromFull(const QString &name_and_email);
  const QString formatter(const QString &str);
  const QString dateToRFC2822(const QDateTime *dt);
  const QString getBoundary();
  bool emailIsValid(const QString &email);

  /* required fields */
  QString from;
  QString to;
  QString subject;
  /* other fields */
  QString messageId;
  QString userAgent;
  QString mDate;

  QString plainText;
  QString html;
  QString attach;
  QString multipart;
  QString s_from;

  QString lastBoundary;
  QString mainBoundary;

  QStringList rcptLst;
};



#endif // MIMEMESSAGE_H
