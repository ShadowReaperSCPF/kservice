/* This file is part of the KDE project
   Copyright (C) 2000 Waldo Bastian <bastian@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef KCTIME_FACTORY_H
#define KCTIME_FACTORY_H

#include <ksycocafactory_p.h>
#include <QHash>

/**
 * Simple dict for assocating a timestamp with each file in ksycoca
 */
class KCTimeDict
{
public:
    void addCTime(const QString &path, const QByteArray &resource, quint32 ctime);
    quint32 ctime(const QString &path, const QByteArray &resource) const;
    void remove(const QString &path, const QByteArray &resource);
    void dump() const;
    bool isEmpty() const
    {
        return m_hash.isEmpty();
    }

    void load(QDataStream &str);
    void save(QDataStream &str) const;
private:
    typedef QHash<QString, quint32> Hash;
    Hash m_hash;
};

/**
 * Internal factory for storing the timestamp of each file in ksycoca
 * @internal
 */
class KCTimeFactory : public KSycocaFactory
{
    K_SYCOCAFACTORY(KST_CTimeInfo)
public:
    /**
     * Create factory
     */
    explicit KCTimeFactory(KSycoca *db);

    ~KCTimeFactory() override;

    /**
     * Write out header information
     */
    void saveHeader(QDataStream &str) override;

    /**
     * Write out data
     */
    void save(QDataStream &str) override;

    KSycocaEntry *createEntry(const QString &) const override
    {
        return nullptr;
    }
    KSycocaEntry *createEntry(int) const override
    {
        return nullptr;
    }

    // Loads the dict and returns it; does not set m_ctimeDict;
    // this is only used in incremental mode for loading the old timestamps.
    KCTimeDict loadDict() const;

    // The API for inserting/looking up entries is in KCTimeDict.
    KCTimeDict *dict()
    {
        return &m_ctimeDict;
    }

private:
    KCTimeDict m_ctimeDict;
    int m_dictOffset;
};

#endif
