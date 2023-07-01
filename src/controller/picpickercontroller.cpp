//
// Created by 杨明 on 2023/6/30.
//

#include "picpickercontroller.h"
#include "../model/appconstants.h"
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QJsonArray>
#include <QJsonParseError>


int PicPickerController::getIndex() const {
    return m_appInfo->getIndex();
}

QString PicPickerController::currPic() const {
    return m_appInfo->currPic();
}

QString PicPickerController::nextPic() {
    return m_appInfo->nextPic();
}

QString PicPickerController::prevPic() {
    return m_appInfo->prevPic();
}


QString PicPickerController::delCurrPic() {
    m_appInfo->delCurrPic();
};

QString PicPickerController::unDelPic() {
    return m_appInfo->unDelPic();
}

QString PicPickerController::getPicsFolder() {
    return m_appInfo->getPicsFolder();
}

QString PicPickerController::debugInfo() const {
    return m_appInfo->debugInfo();
}

QString PicPickerController::logfile() const {
    QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString logfile = QString(homeDir + QDir::separator() + APP_LOCATION + QDir::separator() + "log.txt");
    return logfile;
}

void PicPickerController::close() {
    qDebug() << "normal close.";
}

PicPickerController::PicPickerController() :
        m_appInfo(new AppInfo) {
}


int PicPickerController::init() {
    // firstly, normal init
    m_appInfo->setIndex(0);
    m_appInfo->setPicsFolder(QDir::homePath());
    if (m_appInfo->getTempFolder() == nullptr) {
        QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        qInfo() << "homeDir = " << homeDir;
        QString tmpPath = QDir(homeDir).filePath(DEFAULT_TEMP_DIR);
        qInfo() << "tmpPath = " << tmpPath;
        QDir tmpDir(tmpPath);
        if (!tmpDir.exists()) {
            if (tmpDir.mkpath(tmpPath)) {
                // Directory created successfully
                qInfo() << "tmpPath = " << tmpPath;
            } else {
                // Error creating directory
                return 1;
            }
        } else {
            //TODO Directory already exists
        }
        m_appInfo->setTempFolder(tmpPath);
    }
    qInfo() << "picsFolder = " << m_appInfo->getPicsFolder();
    qInfo() << "tempFolder = " << m_appInfo->getTempFolder();
    // secondly, init from data.json
    QString dataJsonPath = this->getStoreDataPath();

    QFile file(dataJsonPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file";
        return 2;
    }

    QByteArray jsonData = file.readAll();
    QJsonParseError error{};
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &error);

    if (doc.isNull()) {
        qDebug() << "Failed to parse JSON: " << error.errorString();
        return 3;
    }

    if (!doc.isObject()) {
        qDebug() << "JSON is not an object";
        return 4;
    }

    m_appInfo->fromJson(doc.object());
    return 0;
}

QString PicPickerController::getStoreDataPath() {
    QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    return QDir(homeDir).filePath(APP_LOCATION + QDir::separator() + "data.json");
}

QString PicPickerController::getStoreDataBakPath() {
    QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    return QDir(homeDir).filePath(APP_LOCATION + QDir::separator() + "data.json.bak");
}
