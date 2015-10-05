/** 
 * @file	Document.h
 * @brief	header file of the class containing description of documents
 *
 * Copyright 2013 by Samsung Electronics, Inc.,
 * 
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#ifndef DOCUMENT
#define DOCUMENT

#include <vector>
#include <memory>

#include "Strings/StringTools.h"

using std::vector;
using std::shared_ptr;
using std::wstring;
using std::string;

namespace Tools
{

/** 
 * @class	Document
 * @brief	Base parent class for others types of documents
 *
 * Copyright 2013 by Samsung Electronics, Inc.,
 * 
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */
class Document
{
public:

    const vector<wstring>& content(void) const
    {
        return m_content;
    }

    vector<wstring>& content(void)
    {
        return m_content;
    }

    wstring title(void)
    {
        return m_title;
    }

    wstring category(void)
    {
        return m_category;
    }

    virtual void load(const string& i_filename) = 0;
    virtual void save(const string& i_filename) = 0;

    /**
     * @brief	Destructor
     */
    virtual ~Document(){}

protected:
    vector<wstring> m_content;
    wstring m_title;
    wstring m_category;
};


class DocumentPlain : public Document
{
public:
    DocumentPlain(){}
    DocumentPlain(const string& i_filename);

    void load(const string& i_filename);
    void save(const string& i_filename);
};


class DocumentExtended : public Document
{
public:
    DocumentExtended(){}
    DocumentExtended(const string& i_filename);

    wstring url(void)
    {
        return m_url;
    }

    wstring subcategory(void)
    {
        return m_subcategory;
    }

    wstring time(void)
    {
        return m_time;
    }

    wstring description(void)
    {
        return m_description;
    }


    void load(const string& i_filename);
    void save(const string& i_filename);

private:
    wstring m_subcategory;
    wstring m_time;
    wstring m_url;
    wstring m_description;
};


class DocumentCreator{
public:
    virtual shared_ptr<Document> loadDocument(const string& i_filename) = 0;
    virtual shared_ptr<Document> createDocument() = 0;
};

class DocumentPlainCreator : public DocumentCreator
{
public:
    virtual shared_ptr<Document> loadDocument(const string& i_filename);
    virtual shared_ptr<Document> createDocument();
};

class DocumentExtendedCreator : public DocumentCreator
{
public:
    virtual shared_ptr<Document> loadDocument(const string& i_filename);
    virtual shared_ptr<Document> createDocument();
};


}

#endif // DOCUMENT

