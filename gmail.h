#ifndef GMAIL_H
#define GMAIL_H

#include <QHttp>
#include <QtNetwork>
#include <QXmlStreamReader>
#include <QtSql>

struct emailStruct {
    QString title;
    QString summary;
    QString name;
    QString email;
};

class Gmail : public QObject
{  
  Q_OBJECT     
    public:
        Gmail(QObject* = 0); 
        void connection();   
        void getEmails();   
                 
    
    private:    
        int emailsCount;    
        int currentCount;
        int connectionId;
        bool newEmails;
        QXmlStreamReader xml;  
        QHttp http;     
        QString currentTag;
        QString emailId;
        QStringList emailsIds;
        emailStruct emailDetails;
        QList< emailStruct > emailsList;
        
    private slots:        
        void readData(const QHttpResponseHeader &);
        void showEmails(); 
        
    signals:   
        void finished();
    
};

#endif
