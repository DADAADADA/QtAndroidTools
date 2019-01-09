/*
 *	MIT License
 *
 *	Copyright (c) 2018 Fabio Falsini <falsinsoft@gmail.com>
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 */
#pragma once

#include <QtAndroidExtras>
#include <QQmlEngine>

struct ExpansionFileInfo
{
    Q_GADGET
    Q_PROPERTY(uint version MEMBER Version)
    Q_PROPERTY(uint size MEMBER Size)
public:
    ExpansionFileInfo() : Version(0), Size(0) {}
    uint Version;
    uint Size;
};
Q_DECLARE_METATYPE(ExpansionFileInfo)

class QAndroidApkExpansionFiles : public QObject
{
    Q_PROPERTY(ExpansionFileInfo main READ getMainExpansionFileInfo WRITE setMainExpansionFileInfo)
    Q_PROPERTY(ExpansionFileInfo patch READ getPatchExpansionFileInfo WRITE setPatchExpansionFileInfo)
    Q_PROPERTY(QString base64PublicKey READ getBase64PublicKey WRITE setBase64PublicKey)
    Q_PROPERTY(QVector<int> salt READ getSALT WRITE setSALT)
    Q_DISABLE_COPY(QAndroidApkExpansionFiles)
    Q_ENUMS(DOWNLOAD_STATE)
    Q_ENUMS(APKEF_STATE)
    Q_ENUMS(REQUEST_ID)
    Q_ENUMS(STRING_ID)
    Q_OBJECT

    QAndroidApkExpansionFiles();

public:
    ~QAndroidApkExpansionFiles();

    enum DOWNLOAD_STATE
    {
        STATE_IDLE = 1,
        STATE_FETCHING_URL,
        STATE_CONNECTING,
        STATE_DOWNLOADING,
        STATE_COMPLETED,
        STATE_PAUSED_NETWORK_UNAVAILABLE,
        STATE_PAUSED_BY_REQUEST,
        STATE_PAUSED_WIFI_DISABLED_NEED_CELLULAR_PERMISSION,
        STATE_PAUSED_NEED_CELLULAR_PERMISSION,
        STATE_PAUSED_WIFI_DISABLED,
        STATE_PAUSED_NEED_WIFI,
        STATE_PAUSED_ROAMING,
        STATE_PAUSED_NETWORK_SETUP_FAILURE,
        STATE_PAUSED_SDCARD_UNAVAILABLE,
        STATE_FAILED_UNLICENSED,
        STATE_FAILED_FETCHING_URL,
        STATE_FAILED_SDCARD_FULL,
        STATE_FAILED_CANCELED,
        STATE_FAILED
    };
    enum APKEF_STATE
    {
        APKEF_NO_DOWNLOAD_REQUIRED = 0,
        APKEF_STORAGE_READ_PERMISSION_REQUIRED,
        APKEF_STORAGE_WRITE_PERMISSION_REQUIRED,
        APKEF_LVL_CHECK_REQUIRED,
        APKEF_DOWNLOAD_STARTED,
        APKEF_INVALID_JAVA_CLASS,
        APKEF_INVALID_BASE64_PUBLIC_KEY,
        APKEF_INVALID_SALT,
        APKEF_UNKNOWN_ERROR
    };
    enum REQUEST_ID
    {
        REQUEST_ABORT_DOWNLOAD = 0,
        REQUEST_PAUSE_DOWNLOAD,
        REQUEST_CONTINUE_DOWNLOAD,
        REQUEST_DOWNLOAD_STATUS
    };
    enum STRING_ID
    {
        STRING_IDLE = 0,
        STRING_FETCHING_URL,
        STRING_CONNECTING,
        STRING_DOWNLOADING,
        STRING_COMPLETED,
        STRING_PAUSED_NETWORK_UNAVAILABLE,
        STRING_PAUSED_BY_REQUEST,
        STRING_PAUSED_WIFI_DISABLED_NEED_CELLULAR_PERMISSION,
        STRING_PAUSED_NEED_CELLULAR_PERMISSION,
        STRING_PAUSED_WIFI_DISABLED,
        STRING_PAUSED_NEED_WIFI,
        STRING_PAUSED_ROAMING,
        STRING_PAUSED_NETWORK_SETUP_FAILURE,
        STRING_PAUSED_SDCARD_UNAVAILABLE,
        STRING_FAILED_UNLICENSED,
        STRING_FAILED_FETCHING_URL,
        STRING_FAILED_SDCARD_FULL,
        STRING_FAILED_CANCELED,
        STRING_FAILED,
        STRING_UNKNOWN,
        STRING_TIME_LEFT,
        STRING_NOTIFICATION_CHANNEL_NAME
    };

    static QObject* qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);
    static QAndroidApkExpansionFiles* instance();

    Q_INVOKABLE QString mainFileName();
    Q_INVOKABLE QString patchFileName();
    Q_INVOKABLE APKEF_STATE startDownloadFiles();
    Q_INVOKABLE QString getString(int stringID);
    Q_INVOKABLE void sendRequest(REQUEST_ID requestID);

    const QString& getBase64PublicKey() const;
    void setBase64PublicKey(const QString &Base64PublicKey);
    const QVector<int>& getSALT() const;
    void setSALT(const QVector<int> &SALT);
    const ExpansionFileInfo& getMainExpansionFileInfo() const;
    void setMainExpansionFileInfo(const ExpansionFileInfo &MainExpansionFileInfo);
    const ExpansionFileInfo& getPatchExpansionFileInfo() const;
    void setPatchExpansionFileInfo(const ExpansionFileInfo &PatchExpansionFileInfo);

signals:
    void downloadStateChanged(int newState);
    void downloadProgress(qlonglong overallTotal, qlonglong overallProgress, qlonglong timeRemaining, float currentSpeed);

private slots:
    void ApplicationStateChanged(Qt::ApplicationState State);

private:
    const QAndroidJniObject m_JavaApkExpansionDownloader;
    static QAndroidApkExpansionFiles *m_pInstance;
    ExpansionFileInfo m_ExpansionsFileInfo[2];
    QString m_Base64PublicKey;
    QVector<int> m_SALT;

    enum APP_STATE
    {
        APP_STATE_CREATE = 0,
        APP_STATE_START = 1,
        APP_STATE_STOP = 2,
        APP_STATE_DESTROY = 3
    };
    void SetNewAppState(APP_STATE NewState);
};