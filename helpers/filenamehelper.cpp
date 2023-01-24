#include "filenamehelper.h"


auto FileNameHelper::GetTestFolderPath() -> QString
{
#if defined (STRING) && defined (TARGI) && defined (SOURCE_PATH)
    return QDir(STRING(SOURCE_PATH)).filePath("testdata");
#else
    return ("/home/zoli/cncgen/testdata");
#endif
}
