/** License
*
* Copyright (c) 2015 Adam �migielski
*
*
*  Permission is hereby granted, free of charge, to any person obtaining a
*      copy of this software and associated documentation files (the
*      "Software"), to deal in the Software without restriction, including
*      without limitation the rights to use, copy, modify, merge, publish,
*      distribute, sublicense, and/or sell copies of the Software, and to
*      permit persons to whom the Software is furnished to do so, subject to
*      the following conditions: The above copyright notice and this permission
*      notice shall be included in all copies or substantial portions of the
*      Software.
*
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
*      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*      MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
*      IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
*      CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
*      TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
*      SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
**/

/**
* @author Adam �migielski
* @file Library.cpp
**/


#include "PCH.hpp"

#include "Library.hpp"

namespace Dynamic_library
{
    namespace Windows
    {
        Library::Library()
            : m_module(NULL)
        {
        }

        Library::~Library()
        {
            BOOL ret;

            if (NULL != m_module)
            {
                ret = FreeLibrary(m_module);
                m_module = NULL;

                if (FALSE == ret)
                {
                    DWORD err = GetLastError();
                    ERRLOG("FreeLibrary failed: " << err);
                }
            }

            LOG("Unloaded library " << m_name);
        }

        void Library::Init(HMODULE module, const char * name)
        {
            m_module = module;
            m_name = name;
        }

        Platform::proc_t Library::GetFunctionAddress(const char * function_name)
        {
            FARPROC function;

            DEBUGLOG("Getting function: " << function_name);

            if (NULL == m_module)
            {
                ERRLOG("Library hanLibrarye is not initialized");
                ASSERT(0);
                return nullptr;
            }

            if (nullptr == function_name)
            {
                ERRLOG("Invalid name");
                ASSERT(0);
                return nullptr;
            }

            function = GetProcAddress(m_module, function_name);

            if (nullptr == function)
            {
                DWORD err = GetLastError();
                ERRLOG("Failed to load function: " << err);
                ASSERT(0);
                return nullptr;
            }


            LOG("Function: " << function_name << " loaded");

            return (Platform::proc_t) function;
        }
    }

    Dynamic_library::Library * Load(const char * file_path)
    {
        DEBUGLOG("Library: " << file_path << " loading");

        if (nullptr == file_path)
        {
            ERRLOG("Invalid path");
            ASSERT(0);
            return nullptr;
        }

        auto module = LoadLibrary(file_path);
        if (NULL == module)
        {
            DWORD err = GetLastError();
            ERRLOG("Failed to load library: " << err);
            ASSERT(0);
            return nullptr;
        }

        auto library = new Dynamic_library::Windows::Library();
        if (nullptr == library)
        {
            ASSERT(0);
            ERRLOG("Memory allocation failed");
            FreeLibrary(module);
            return nullptr;
        }

        library->Init(module, file_path);


        LOG("Library: " << file_path << " loaded");

        return library;
    }
}
