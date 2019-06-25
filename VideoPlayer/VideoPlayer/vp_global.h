#ifndef VP_GLOBAL_H
#define VP_GLOBAL_H

// VideoPlayer global
class QSettings;
class QString;

// Configure Map
class VPConfigure
{
public:
    static VPConfigure* getInstance() {
        return s_configure;
    };
    void initSettings(const QString& organizationName, const QString& appName);
    QSettings* settings() { return mSettings; };
private:
    VPConfigure() : mSettings(nullptr) {};
    VPConfigure(const VPConfigure&) {};
    VPConfigure& operator=(const VPConfigure&) {
        return *s_configure;
    };
private:
    static VPConfigure *s_configure;
    QSettings *mSettings;
};


// Translate
// ----------------
class QTranslator;
class QString;
extern QTranslator *g_appTranslator;
QString trloadEn();
QString trloadZhCh();
QString trload(const QString& lg);

#endif // VP_GLOBAL_H
