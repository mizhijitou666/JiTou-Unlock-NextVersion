#include "utils.h"
#include <QDirIterator>
#include <QFileInfo>
#include <QFile>
#include <QtCore/QProcess>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <zlib.h>

bool Utils::copyDirectory(QString source, QString dest) {
    QFileInfo fileInfo(source);
    if(!fileInfo.isDir())
    {
        return false;
    }
    QDir().mkpath(dest);

    auto iterator = QDirIterator(source, QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    while (iterator.hasNext()) {
        iterator.next();

        auto sourceFilePath = iterator.filePath();
        auto relativeFilePath = iterator.fileInfo().absoluteFilePath().replace(source, "");
        auto destinationFilePath = dest + relativeFilePath;

        // Create the destination directory path if necessary
        QDir().mkpath(QFileInfo(destinationFilePath).absolutePath());

        // Copy the file or directory
        if (iterator.fileInfo().isFile()) {
            if (!QFile::copy(sourceFilePath, destinationFilePath)) {
                               return false;
            }
        } else if (iterator.fileInfo().isDir()) {
            if (!QDir().mkdir(destinationFilePath)) {
                                return false;
            }
        }
    }

    return true;
}
