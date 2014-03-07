## Features
- Easy to use
- SMTP authentication support (LOGIN methods)
- SSL connections support
- Plain text, HTML, multiple attachments content in emails
- Use UTF-8 charset and BASE64 transport encoding (support most email clients)

## Usage
(see ssmtp_example project)
Simple example:
```c++
// add "QT += network" in priject file
// include "mimemessage.h" and "ssmtp.h" in source

//1. Create MIMEmessage object
MIMEmessage message;

// 2. Set sender, add recipients, set subject
message.setSender("My name <myemail.example.com>");
message.addRcpt("Jon Smitt <jon@test.com>");
message.addRcpt("mary@test.com");
message.setSubject("Test subject");

// 3. Add content
message.addPlainTextPart("Only plain text message");
// or
message.addHtmlPart("<!DOCTYPE html><html><body><p>Only HTML text messag</p></body></html>")
// or add alternative part (MIME content type as "multipart/alternative")
message.addAlternative(/*plain text*/,/*html text*/)
// Add attachments if need
message.addAttach("/file/name/to/attach.pdf");
// or
message.addAttach(myByteArray,"file.dat")

// 4. Create SSMTP object
SSMTP cln("smtp.myserver.com",25,false);

// 5. Set login/pass if need
cln.setLogin("login");
cln.setPass("pass");

// 6. Send email
cln.send(&message);

```
