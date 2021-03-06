/*
 *  Copyright (C) 2006 David Faure   <faure@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License version 2 as published by the Free Software Foundation;
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */
#ifndef KSERVICETEST_H
#define KSERVICETEST_H

#include <kservice_export.h>

#include <QAtomicInt>
#include <QObject>

class KServiceTest : public QObject
{
    Q_OBJECT
public:
    KServiceTest() : m_sycocaUpdateDone(0) {}
private Q_SLOTS:
    void initTestCase();
#if KSERVICE_BUILD_DEPRECATED_SINCE(5, 0)
    void testKPluginMetaData();
#endif
    void cleanupTestCase();
    void testByName();
    void testConstructorFullPath();
    void testConstructorKDesktopFileFullPath();
    void testConstructorKDesktopFile();
    void testCopyConstructor();
    void testCopyInvalidService();
    void testProperty();
    void testAllServiceTypes();
    void testAllServices();
    void testServiceTypeTraderForReadOnlyPart();
    void testTraderConstraints();
    void testSubseqConstraints();
    void testHasServiceType1();
    void testHasServiceType2();
#if KSERVICE_BUILD_DEPRECATED_SINCE(5, 66)
    void testWriteServiceTypeProfile();
#endif
    void testDefaultOffers();
#if KSERVICE_BUILD_DEPRECATED_SINCE(5, 66)
    void testDeleteServiceTypeProfile();
#endif
    void testDBUSStartupType();
    void testByStorageId();
    void testActionsAndDataStream();
    void testServiceGroups();
    void testDeletingService();
    void testReaderThreads();
    void testThreads();
    void testOperatorKPluginName();
    void testKPluginInfoQuery();
    void testCompleteBaseName();
    void testEntryPathToName();
    void testTraderQueryMustRebuildSycoca();

private:
    void createFakeService(const QString &filenameSuffix, const QString &serviceType);
    void runKBuildSycoca(bool noincremental = false);

    QString m_firstOffer;
    bool m_hasKde5Konsole;
    QAtomicInt m_sycocaUpdateDone;
    bool m_hasNonCLocale;
};

#endif
