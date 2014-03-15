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


#include "mimemessage.h"

MIMEmessage::MIMEmessage()
{
  clearAll();
  setDate(QDateTime::currentDateTime());
  mainBoundary = getBoundary();
}

bool MIMEmessage::setSender(const QString email, const QString &name)
{
  if (!emailIsValid(email))
    return false;
  if (name.trimmed().isEmpty())
    from = "\r\nFrom: " + email;
  else
    from = "\r\nFrom: " + headerEncoder(name.trimmed()) + " <" + email + ">";
  s_from = "<" + email + ">";
  return true;
}

bool MIMEmessage::setSender(const QString &name_and_email)
{
  if (name_and_email.trimmed().isEmpty())
    return false;
  QString name, email;
  if (emailIsValid(name_and_email))
  {
    name.clear();
    email = name_and_email;
  }
  else
  {
    name = getNameFromFull(name_and_email);
    email = getMailFromFull(name_and_email);
  }
  return setSender(email,name);
}

bool MIMEmessage::addRcpt(const QString email, const QString &name)
{
  if (!emailIsValid(email))
    return false;
  if (to.isEmpty()) to = "\r\nTo: ";
  if (name.trimmed().isEmpty())
  {
    if (to.indexOf('@') == -1)
      to += email;
    else
      to += ",\r\n    " + email;
  }
  else
  {
    if (to.indexOf('@') == -1)
      to += headerEncoder(name.trimmed()) + " <" + email + ">";
    else
      to += ",\r\n    " + headerEncoder(name.trimmed()) + " <" + email + ">";
  }
  rcptLst.append("<" + email + ">");
  return true;
}

bool MIMEmessage::addRcpt(const QString &name_and_email)
{
  if (name_and_email.trimmed().isEmpty())
    return false;
  QString name, email;
  if (emailIsValid(name_and_email))
  {
    name.clear();
    email = name_and_email;
  }
  else
  {
    name = getNameFromFull(name_and_email);
    email = getMailFromFull(name_and_email);
  }
  return addRcpt(email,name);
}

void MIMEmessage::clearRcpt()
{
  to.clear();
  rcptLst.clear();
}

void MIMEmessage::clearSubject()
{
  subject.clear();
}

void MIMEmessage::clearMessageId()
{
  messageId.clear();
}

void MIMEmessage::clearUserAgent()
{
  userAgent.clear();
}

void MIMEmessage::clearDate()
{
  mDate.clear();
}

void MIMEmessage::clearPlainText()
{
  plainText.clear();
}

void MIMEmessage::clearHtml()
{
  html.clear();
}

void MIMEmessage::clearAttachList()
{
  attach.clear();
}

void MIMEmessage::clearMultipart()
{
  multipart.clear();
}

void MIMEmessage::clearAll()
{
  clearSender();
  clearRcpt();
  clearSubject();
  clearMessageId();
  clearUserAgent();
  clearDate();
  clearPlainText();
  clearHtml();
  clearAttachList();
  clearMultipart();
}

QStringList *MIMEmessage::rcptList()
{
  return &rcptLst;
}

const QString MIMEmessage::getFromEmail()
{
  return s_from;
}

const QString MIMEmessage::getMIMEText()
{
  if (from.isEmpty() || to.isEmpty())
    return "";

  QString output = "MIME-Version: 1.0";
  output.append(messageId);
  output.append(userAgent);
  output.append(mDate);
  output.append(from);
  output.append(to);
  output.append(subject);

  output.append("\r\nContent-Type: multipart/mixed;");
  output.append("\r\n  boundary=" + mainBoundary + "\r\n");

  output.append(plainText);
  output.append(html);
  output.append(multipart);
  output.append(attach);

  output.append("\r\n--" + mainBoundary + "--");
  output.append("\r\n");
  return output;
}

const QString MIMEmessage::getHash()
{
  QCryptographicHash h(QCryptographicHash::Md5);

  h.addData(from.toLatin1());
  h.addData(to.toLatin1());
  h.addData(subject.toLatin1());
  h.addData(plainText.toLatin1());
  h.addData(html.toLatin1());
  h.addData(multipart.toLatin1());
  h.addData(attach.toLatin1());

  return h.result().toHex();
}

void MIMEmessage::clearSender()
{
  from.clear();
}

void MIMEmessage::setSubject(const QString &subj)
{
  subject = "\r\nSubject: " + headerEncoder(subj.trimmed());
}

void MIMEmessage::setMessageId(const QString &mess_id)
{
  messageId = "\r\nMessage-ID: " + mess_id.trimmed();
}

void MIMEmessage::setUserAgent(const QString &user_agent)
{
  userAgent = "\r\nUser-Agent: " + user_agent.trimmed();
}

void MIMEmessage::setDate(const QDateTime dt)
{
  mDate = "\r\nDate: " + dateToRFC2822(&dt);
}

void MIMEmessage::addPlainTextPart(const QString &plain_text)
{
  if (plain_text.isEmpty()) return;
  plainText.append("\r\n--"+mainBoundary);
  plainText.append("\r\nContent-Type: text/plain; charset=UTF-8");
  plainText.append("\r\nContent-Transfer-Encoding: base64\r\n\r\n");
  plainText.append(formatter(str2base64(plain_text)));
}

void MIMEmessage::addHtmlPart(const QString &html_text)
{
  if (html_text.isEmpty()) return;
  html.append("\r\n--"+mainBoundary);
  html.append("\r\nContent-Type: text/html; charset=UTF-8");
  html.append("\r\nContent-Transfer-Encoding: base64\r\n\r\n");
  html.append(formatter(str2base64(html_text)));
}

void MIMEmessage::addAlternative(const QString &plain_text, const QString &html_text)
{
  if (plain_text.isEmpty() || html_text.isEmpty()) return;

  QString partBoundary = getBoundary();
  multipart.append("\r\n--"+mainBoundary);
  multipart.append("\r\nContent-Type: multipart/alternative;");
  multipart.append("\r\n  boundary=" + partBoundary + "\r\n\r\n");

  multipart.append("--"+partBoundary);
  multipart.append("\r\nContent-Type: text/plain; charset=UTF-8");
  multipart.append("\r\nContent-Transfer-Encoding: base64\r\n\r\n");

  multipart.append(formatter(str2base64(plain_text)));

  multipart.append("\r\n--"+partBoundary);
  multipart.append("\r\nContent-Type: text/html; charset=UTF-8");
  multipart.append("\r\nContent-Transfer-Encoding: base64\r\n\r\n");

  multipart.append(formatter(str2base64(html_text)));
  multipart.append("--"+partBoundary+"--\r\n");
}

bool MIMEmessage::addAttach(const QString &filename)
{
  if (filename.trimmed().isEmpty()) return false;
  QFile f(filename);
  if (!f.open(QIODevice::ReadOnly)) return false;
  QString fn = headerEncoder(QFileInfo(f).fileName());
  QByteArray ba = f.readAll();
  f.close();
  addAttach(&ba,fn);
  return true;
}

void MIMEmessage::addAttach(QByteArray *ba, const QString &name)
{
  attach.append("\r\n--" + mainBoundary);
  attach.append("\r\nContent-Type: application/octet-stream;");
  attach.append("\r\n  name=\"" + name + "\"");
  attach.append("\r\nContent-Transfer-Encoding: base64");
  attach.append("\r\nContent-Disposition: attachment;");
  attach.append("\r\n  filename=\"" + name + "\"");
  attach.append("\r\n\r\n");
  attach.append(formatter(ba->toBase64()));
}

const QString MIMEmessage::base64str(const QString &base64)
{
  QByteArray a = QByteArray::fromBase64(base64.toLatin1());
  return QString::fromUtf8(a.data());
}

const QString MIMEmessage::headerEncoder(const QString &str)
{
  return "=?utf-8?B?" + str2base64(str) + "?=";
}

const QString MIMEmessage::getNameFromFull(const QString &name_and_email)
{
  return name_and_email.section('<',0,0).trimmed();
}

const QString MIMEmessage::getMailFromFull(const QString &name_and_email)
{
  QString e = name_and_email.section('<',1,1).trimmed();
  e.replace(">","");
  return e;
}

const QString MIMEmessage::formatter(const QString &str)
{
  int pos = 0;
  int cnt = FORMAT_CNT;
  QString result = "";
  while(pos < str.length()-1)
  {
    result.append(str.mid(pos,cnt) + "\r\n");
    pos += cnt;
    if(pos + FORMAT_CNT < str.length()-1)
      cnt = FORMAT_CNT;
    else
      cnt = str.length() - pos;
  }
  return result;
}

const QString MIMEmessage::dateToRFC2822(const QDateTime *dt)
{
  /*-----------------------------------------------------------
   * Implementation from Qt5 qdatetime.cpp QDateTime::toString
   * for Qt4 support
   * ----------------------------------------------------------
   */

  QString buf;

  QLocale l = QLocale(QLocale::English);
  QString ddd = l.dayName(dt->date().dayOfWeek(),QLocale::ShortFormat);
  QString MMM = l.monthName(dt->date().month(),QLocale::ShortFormat);
  buf = ddd + ", " + dt->toString("d ") + MMM + dt->toString(" yyyy hh:mm:ss ");
  int utcOffset = dt->utcOffset();
  if (dt->timeSpec() == Qt::LocalTime)
  {
    QDateTime utc = dt->toUTC();
    utc.setTimeSpec(dt->timeSpec());
    utcOffset = utc.secsTo(*dt);
  }
  const int offset = qAbs(utcOffset);
  buf += QLatin1Char((offset == utcOffset) ? '+' : '-');
  const int hour = offset / 3600;
  if (hour < 10)
      buf += QLatin1Char('0');
  buf += QString::number(hour);
  const int min = (offset - (hour * 3600)) / 60;
  if (min < 10)
      buf += QLatin1Char('0');
  buf += QString::number(min);
  return buf;
}

const QString MIMEmessage::getBoundary()
{
  QCryptographicHash h(QCryptographicHash::Md5);
  QString dt = QDateTime::currentDateTime().toString("yyyyddMMhhmmsszzz");
  QString recursive_boundary = dt + lastBoundary;
  QByteArray data = recursive_boundary.toLatin1();
  h.addData(data);
  lastBoundary = h.result().toHex();
  return lastBoundary;
}

bool MIMEmessage::emailIsValid(const QString &email)
{
  QRegExp rx(EMAIL_REGEXP);
  return (rx.indexIn(email) != -1);
}

const QString MIMEmessage::str2base64(const QString &str)
{
  return str.toUtf8().toBase64();
}
