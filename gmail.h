#ifndef GMAIL_H
#define GMAIL_H

#include <QObject>
#include <QTimer> 
#include <QDateTime>
#include <QCoreApplication>
#include <QHttp>
#include <QtNetwork>
#include <QXmlStreamReader>
#include <QtSql>

class Gmail : public QObject
{  
  Q_OBJECT     
    public:
        Gmail(QObject* = 0); 
        void connection();   
        void getEmails();             
    
    private:    
        int nbEmails;    
        QXmlStreamReader xml;  
        QHttp http;     
        QString currentTag;
        QString linkString;
        QString titleString;  
        
    private slots:        
        //void getEmails(const QHttpResponseHeader &); 
        
    signals:       
    
};

#endif
