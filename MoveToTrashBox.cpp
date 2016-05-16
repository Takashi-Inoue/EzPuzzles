/*
 * Copyright 2016 Takashi Inoue
 *
 * This file is part of EzPuzzles.
 *
 * EzPuzzles is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EzPuzzles is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EzPuzzles.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "MoveToTrashBox.h"
#include <QDir>
#include <QSysInfo>

MoveToTrashBox::MoveToTrashBox(const QString &path) :
    path(QDir::toNativeSeparators(path))
{
}

#ifdef Q_OS_WIN

#include <Shobjidl.h>
#include <comdef.h>
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "shell32.lib")

_COM_SMARTPTR_TYPEDEF(IShellItem, __uuidof(IShellItem));
_COM_SMARTPTR_TYPEDEF(IFileOperation, __uuidof(IFileOperation));

bool MoveToTrashBox::exec() const
{
    HRESULT result = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    if (FAILED(result))
        return false;

    IFileOperationPtr fo;
    result = fo.CreateInstance(CLSID_FileOperation, nullptr, CLSCTX_ALL);

    if (FAILED(result)) {
        CoUninitialize();
        return false;
    }

    ulong flags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;

//    if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS8)
//        flags |= FOFX_RECYCLEONDELETE;

    result = fo->SetOperationFlags(flags);

    if (SUCCEEDED(result)) {
        IShellItemPtr iShellItem;
        result = SHCreateItemFromParsingName(path.toStdWString().c_str(), nullptr, IID_PPV_ARGS(&iShellItem));

        if (SUCCEEDED(result))
            result = fo->DeleteItem(iShellItem, nullptr);

        if (SUCCEEDED(result))
            result = fo->PerformOperations();
    }

    CoUninitialize();

    return SUCCEEDED(result);
}

#endif
