#ifndef VP_GLOBAL_H
#define VP_GLOBAL_H

// VideoPlayer global

class QTranslator;
class QString;
extern QTranslator *g_appTranslator;
void trloadEn();
void trloadZhCh();
void trload(const QString& lg);


#endif // VP_GLOBAL_H
