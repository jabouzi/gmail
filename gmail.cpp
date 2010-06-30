#include "gmail.h"

Gmail::Gmail(QObject* parent) :
    QObject(parent)
{
    QHttpResponseHeader resp;
    connection();
    //getEmails();
    //connect(&http, SIGNAL(responseHeaderReceived (QHttpResponseHeader) ), this, SLOT(getEmails(QHttpResponseHeader)));
    //connect(&http, SIGNAL(requestFinished ( int, bool ) ), this, SLOT(getEmails(QHttpResponseHeader)));
}

void Gmail::connection()
{
    http.setHost("mail.google.com", QHttp::ConnectionModeHttps);
    http.setUser("jabouzi", "7024043");
    int connectionId = http.get("/mail/feed/atom");
    qDebug("connection id %d",connectionId);
    getEmails();
}

void Gmail::getEmails()
{
    QByteArray data = http.readAll();
    //if (resp.statusCode() != 200)
        //http.abort();
    //else {
        xml.addData(data); 
        qDebug("YO");
        while (!xml.atEnd()) {
            xml.readNext();
            qDebug("%s",xml.name().toString());
            if (xml.isStartElement()) {
                qDebug("%s",xml.name().toString());
                if (xml.name() == "fullcount")
                {
                    qDebug("%s",xml.text().toString().toLatin1().data());
                    currentTag = xml.name().toString();
                    qDebug("%s",currentTag.toLatin1().data());
                }
                else
                {
                    currentTag = xml.name().toString();
                    qDebug("%s",currentTag.toLatin1().data());
                }
            } else if (xml.isEndElement()) {
                if (xml.name() == "item") {
                    
                }

            } else if (xml.isCharacters() && !xml.isWhitespace()) {
                if (currentTag == "title")
                    titleString += xml.text().toString();
                else if (currentTag == "link")
                    linkString += xml.text().toString();
            }
        }
        if (xml.error() && xml.error() != QXmlStreamReader::PrematureEndOfDocumentError) {
            qWarning() << "XML ERROR:" << xml.lineNumber() << ": " << xml.errorString();
            http.abort();
        }
    //}
}

