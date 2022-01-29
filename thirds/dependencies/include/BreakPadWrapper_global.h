#include <QtCore/QtGlobal>

#if defined(BREAKPADWRAPPER_LIBRARY)
#  define CRASH_API Q_DECL_EXPORT
#else
#  define CRASH_API Q_DECL_IMPORT
#endif
