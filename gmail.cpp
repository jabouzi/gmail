#include "gmail.h"

Gmail::Gmail(QObject* parent) :
    QObject(parent)
{
    newEmails = false;
    currentCount = 0;      
    connect(&http, SIGNAL(readyRead(QHttpResponseHeader)),
             this, SLOT(readData(QHttpResponseHeader)));
    connect(this, SIGNAL(finished()), this, SLOT(showEmails()));
    connection();   
}

void Gmail::connection()
{
    http.setHost("mail.google.com", QHttp::ConnectionModeHttps);
    http.setUser("jabouzi", "7024043");
    http.get("/mail/feed/atom");    
}

void Gmail::readData(const QHttpResponseHeader &resp)
{
    if (resp.statusCode() != 200)
    {
        http.abort();
        //emit probleme de connection
    }
    else {
        xml.addData(http.readAll());
        getEmails();
    }
}

void Gmail::getEmails()
{
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement()) {                  
            currentTag = xml.name().toString();    
        } else if (xml.isEndElement()) {          
            if (xml.name() == "entry")
            {
                if (!emailsIds.contains(emailId))
                {
                    emailsIds << emailId;
                    emailsList << emailDetails;
                }
            }
            else if (xml.name() == "feed")
            {
                emit(finished());
            }
            
        } else if (xml.isCharacters() && !xml.isWhitespace()) {
            if (currentTag == "fullcount")
            {               
                bool ok;
                emailsCount = xml.text().toString().toInt(&ok);
                if (emailsCount > currentCount)
                {
                    // emit new email
                    currentCount = emailsCount;     
                    newEmails = true;                                   
                }         
                else
                {
                    newEmails = false;
                }      
            }
            if (newEmails) // more than one email
            {        
                if (currentTag == "id")
                {
                   emailId = xml.text().toString();
                    //qDebug("title : %s",xml.text().toString().toLatin1().data());
                }
                if (currentTag == "title")
                {
                    emailDetails.title = xml.text().toString();
                    //qDebug("title : %s",xml.text().toString().toLatin1().data());
                }
                else if (currentTag == "summary")
                {
                    emailDetails.summary = xml.text().toString();
                    //qDebug("summary : %s",xml.text().toString().toLatin1().data());
                }
                else if (currentTag == "name")
                {
                    emailDetails.name = xml.text().toString();
                    //qDebug("name : %s",xml.text().toString().toLatin1().data());
                }
                else if (currentTag == "email")
                {
                    emailDetails.email = xml.text().toString();
                    //qDebug("email : %s",xml.text().toString().toLatin1().data());
                }               
            }            
        }
    }
    if (xml.error() && xml.error() != QXmlStreamReader::PrematureEndOfDocumentError) {
        qWarning() << "XML ERROR:" << xml.lineNumber() << ": " << xml.errorString();
        http.abort();
    }    
}

void Gmail::showEmails()
{
    qDebug(" You hane %d new emails : ",emailsList.size());
    for (int i = 0; i < emailsList.size(); i++)
    {       
        qDebug("Email # %d : ",i+1);
        qDebug("title : %s",emailsList.at(i).title.toLatin1().data());
        qDebug("summary : %s",emailsList.at(i).summary.toLatin1().data());
        qDebug("name : %s",emailsList.at(i).name.toLatin1().data());
        qDebug("email : %s",emailsList.at(i).email.toLatin1().data());
    }
    
    //~ qDebug("=================================");
    
    //~ for (int i = 0; i < emailsIds.size(); i++)
    //~ {       
        //~ qDebug("Email # %d : ",i+1);
        //~ qDebug("title : %s",emailsIds.at(i).toLatin1().data());        
    //~ }
}

