/* This file is part of the KDE project
   Copyright 1999-2007 David Faure <faure@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef KBUILD_MIME_TYPE_FACTORY_H
#define KBUILD_MIME_TYPE_FACTORY_H

#include <kmimetypefactory_p.h>
#include <QStringList>

/**
 * Mime-type factory for building ksycoca
 * @internal
 */
class KBuildMimeTypeFactory : public KMimeTypeFactory
{
public:
    /**
     * Create factory
     */
    explicit KBuildMimeTypeFactory(KSycoca *db);

    ~KBuildMimeTypeFactory() override;

    KSycocaEntry::List allEntries() const override;

    /**
     * Construct a KMimeType from a config file.
     */
    KSycocaEntry *createEntry(const QString &file) const override;

    MimeTypeEntry *createEntry(int) const override
    {
        assert(0);
        return nullptr;
    }

    KMimeTypeFactory::MimeTypeEntry::Ptr createFakeMimeType(const QString &name);

    /**
     * Write out mime type specific index files.
     */
    void save(QDataStream &str) override;

    /**
     * Write out header information
     *
     * Don't forget to call the parent first when you override
     * this function.
     */
    void saveHeader(QDataStream &str) override;
};

#endif
