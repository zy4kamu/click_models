#ifndef TERNARYSEARCHTREE_H
#define TERNARYSEARCHTREE_H

#include <memory>

namespace Tools
{

class ENotFinal
{
public:
    ENotFinal() {std::cout << "Empty node!" << std::endl;}
};

template <class Key, class Data>
class TernarySearchTree
{
    typedef typename Key::value_type KeyAtom;
private:
    class TSTNode
    {
    public:
        TSTNode(const KeyAtom& i_key) : m_key(i_key), m_final(false) { }
        std::shared_ptr<TSTNode> loChild() const {return m_loChild;}
        std::shared_ptr<TSTNode> hiChild() const {return m_hiChild;}
        std::shared_ptr<TSTNode> eqChild() const {return m_eqChild;}
        Data& data() { if(!m_final) {throw ENotFinal();} return m_data; }
        void setFinal() {m_final = true;}
    private:
        bool m_final;
        std::unique_ptr<Data> m_data;
        KeyAtom m_key;
        std::shared_ptr<TSTNode> m_loChild;
        std::shared_ptr<TSTNode> m_hiChild;
        std::shared_ptr<TSTNode> m_eqChild;
    };

public:
    TernarySearchTree()
    {
        m_size = 0;
    }
    bool has(const Key& i_key) noexcept
    {
        std::shared_ptr<TSTNode> current = m_top;
        size_t len = i_key.size();
        int c = 0;
        while(current)
        {
           if(i_key[c] < current->data())
           {
               current = current->loChild();
           }
           else if(i_key[c] == current->data())
           {
               c++;
               current = current->eqChild();
               if(c == len && current && current->final())
               {
                   m_cacheKey = i_key;
                   m_cacheNode = current;
                   return true;
               }
               else
               {
                   return false;
               }
           }
           else
           {
               current = current->hiChild();
           }
        }
        return false;
    }
    Data& operator[](const Key& i_key)
    {
        if(i_key == m_cacheKey)
        {
            return m_cacheNode->data();
        }
        std::shared_ptr<TSTNode> current = m_top;
        size_t len = i_key.size();
        int c = 0;
        while(c!=len)
        {
           if(i_key[c] < current->data())
           {
               current = current->loChild();
           }
           else if(i_key[c] == current->data())
           {
               c++;
               current = current->eqChild();
               if(!current)
               {
                   current = std::make_shared<TSTNode>(i_key[c]);
               }
               else if(c == len && current->final())
               {
                   return current->data();
               }
               else if(c == len)
               {
                   current->setFinal();
                   return current->data();
               }
           }
           else
           {
               current = current->hiChild();
           }
        }
        throw std::exception();
    }

private:
    std::shared_ptr<TSTNode> m_top;
    Key m_cacheKey;
    std::shared_ptr<TSTNode> m_cacheNode;
    int m_size();
};

}

#endif // TERNARYSEARCHTREE_H
