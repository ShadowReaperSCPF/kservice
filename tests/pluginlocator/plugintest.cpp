/******************************************************************************
*   Copyright 2013 Sebastian Kügler <sebas@kde.org>                           *
*                                                                             *
*   This library is free software; you can redistribute it and/or             *
*   modify it under the terms of the GNU Library General Public               *
*   License as published by the Free Software Foundation; either              *
*   version 2 of the License, or (at your option) any later version.          *
*                                                                             *
*   This library is distributed in the hope that it will be useful,           *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU          *
*   Library General Public License for more details.                          *
*                                                                             *
*   You should have received a copy of the GNU Library General Public License *
*   along with this library; see the file COPYING.LIB.  If not, write to      *
*   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,      *
*   Boston, MA 02110-1301, USA.                                               *
*******************************************************************************/

#include "plugintest.h"

#include <QDebug>
#include <kservice.h>
#include <kservicetypetrader.h>
#include <kplugininfo.h>
#include <kplugintrader.h>

#include <qcommandlineparser.h>

#include <QElapsedTimer>
#include <QStandardPaths>
#include <QStringList>

static QTextStream cout(stdout);

class PluginTestPrivate
{
public:
    QString pluginName;
};

PluginTest::PluginTest() :
    QObject(nullptr)
{
    d = new PluginTestPrivate;
}

PluginTest::~PluginTest()
{
    delete d;
}

int PluginTest::runMain()
{
    // measure performance
    QElapsedTimer timer;
    int runs = 1;
    QList<qint64> timings;

    cout << "-- KPluginTrader Test --"
        #if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
         << endl
        #else
         << Qt::endl
        #endif
            ;
    bool ok = true;

    // KSycoca querying
    timer.start();

    for (int _i = 0; _i < runs; _i++) {
        timer.restart();
        if (!loadFromKService(QStringLiteral("time"))) {
            ok = false;
        }
        timings << timer.nsecsElapsed();
    }
    report(timings, QStringLiteral("KServiceTypeTrader"));
    timings.clear();

    // -- Metadata querying
    for (int _i = 0; _i < runs; _i++) {
        timer.restart();
        if (!loadFromMetaData()) {
            ok = false;
        }
        //if (!loadFromMetaData2("Plasma/ContainmentActions")) ok = false;
        timings << timer.nsecsElapsed();
    }
    report(timings, QStringLiteral("Metadata"));
    timings.clear();

    findPlugins();

    if (ok) {
        qDebug() << "All tests finished successfully";
        return 0;
    }
    return 0; // We return successfully in any case, since plugins aren't installed for most people
}

void PluginTest::report(const QList<qint64> timings, const QString &msg)
{
    qulonglong totalTime = 0;

    int unitDiv = 1000;
    QString unit = QStringLiteral("microsec");
    int i = 0;
    for (qint64 t : timings) {
        int msec = t / 1000000;
        qDebug() << "  Run " << i << ": " << msec << " msec";
        totalTime += t;
        i++;
    }
    QString av = QString::number((totalTime / timings.count() / unitDiv), 'f', 1);
    qDebug() << " Average: " << av << " " << unit << " (" << msg << ")";
}

bool PluginTest::loadFromKService(const QString &name)
{
    bool ok = false;
    QString constraint = QStringLiteral("[X-KDE-PluginInfo-Name] == '%1'").arg(name);
    KService::List offers = KServiceTypeTrader::self()->query(QStringLiteral("Plasma/DataEngine"),
                            constraint);
    QString error;

    if (offers.isEmpty()) {
        qDebug() << "offers are empty for " << name << " with constraint " << constraint;
    } else {
        QVariantList allArgs;
        allArgs << offers.first()->storageId();
        const QString _n = offers.first()->property(QStringLiteral("Name")).toString();
        if (!_n.isEmpty()) {
            qDebug() << "Found Dataengine: " << _n;
            ok = true;
        } else {
            qDebug() << "Nothing found. ";
        }
    }

    return ok;
}

bool PluginTest::loadFromMetaData(const QString &serviceType)
{
    bool ok = false;
    QString pluginName("time");
    QString constraint = QStringLiteral("[X-KDE-PluginInfo-Name] == '%1'").arg(pluginName);
    const KPluginInfo::List res = KPluginTrader::self()->query(QStringLiteral("kf5"), serviceType, QString());
    qDebug() << "----------- Found " << res.count() << " Plugins" << constraint;
    ok = res.count() > 0;
    for (const KPluginInfo &info : res) {
        qDebug() << "   file: " << info.libraryPath();

    }

    return ok;

}

bool PluginTest::findPlugins()
{
    QElapsedTimer timer;
    QList<qint64> timings;
    const QString pluginDir("/media/storage/testdata/");
    const QStringList sizes = QStringList() << QStringLiteral("50")
                              << QStringLiteral("100")
                              << QStringLiteral("150")
                              << QStringLiteral("200")
                              << QStringLiteral("250")
                              << QStringLiteral("300")
                              << QStringLiteral("400")
                              << QStringLiteral("500")
                              << QStringLiteral("600")
                              << QStringLiteral("700")
                              << QStringLiteral("800")
                              << QStringLiteral("1000")
                              << QStringLiteral("1500")
                              << QStringLiteral("2000")
                              << QStringLiteral("5000");
    QStringList datadirs;

    for (const QString &_s : sizes) {
        datadirs << pluginDir + _s;
    }
    for (const QString &subdir : qAsConst(datadirs)) {
        const QString pluginName;
        const QString constraint;
        const QString serviceType;

        timer.restart();
        KPluginInfo::List res = KPluginTrader::self()->query(subdir, serviceType, constraint);
        timings << timer.nsecsElapsed();
        qDebug() << "Found " << res.count() << " Plugins in " << subdir;
    }
    report(timings, QStringLiteral("reading monsterdirs"));
    return true;
}

#include "moc_plugintest.cpp"

