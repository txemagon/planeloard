#ifndef __REFERENCE_COUNTER_H__
#define __REFERENCE_COUNTER_H__

class ReferenceCounter
{
    public:
        ReferenceCounter() :
            m_refcount(1) {}

        inline int get_reference_count() { return m_refcount; }

        inline void add_reference() { m_refcount++; }
        inline bool remove_reference() { return --m_refcount == 0; }

    protected:

    private:
        int m_refcount;
}

#endif
