/***************************************************************************
                BitArray
                Amir Zror
                26/6/

*****************************************************************************/
#ifndef _BITARRAY_HPP
#define _BITARRAY_HPP

#include <vector>
#include <algorithm>

namespace ilrd
{
char g_lut[256];
const size_t g_bits_in_byte = 8;
const size_t g_bits_in_word = g_bits_in_byte * sizeof(size_t);

template<size_t SIZE>
class BitArray
{
private:
    class BitProxy;
    struct ShiftAndAssign;
public:
    BitArray();
    // ~BitArray();
    size_t CountTrueBits()const;
    void FlipAll();
    void FlipBit(size_t index);

    void Set(size_t index, bool state);
    bool Get(size_t index)const;

    BitArray& operator|=(BitArray& other);                  //ba1|=ba2;
    BitArray& operator&=(const BitArray& other);            //ba1&=ba2;
    BitArray& operator^=(const BitArray& other);            //ba1^=ba2

    bool operator==(const BitArray& other);
    bool operator!=(const BitArray& other);

    BitArray<SIZE>& operator<<=(size_t num_shifts);         //ba1<<=number;

    BitProxy operator[](size_t index);                      //ba[5] ;
    bool operator[](size_t index)const;                     //ba[3]


private:
    size_t GetBitIndex(size_t index)const;
    size_t GetCellIndex(size_t index)const;

    std::vector<size_t> m_vec;
};

/*******************
*****BitArray*******
*******************/

template<size_t SIZE>
BitArray<SIZE>::BitArray():m_vec((SIZE + g_bits_in_word - 1) / g_bits_in_word, 0x00ul)
{}

template<size_t SIZE>
bool BitArray<SIZE>::Get(size_t index) const
{
    return ((m_vec[GetCellIndex(index)] >> GetBitIndex(index)) & 1);
}

template<size_t SIZE>
void BitArray<SIZE>::Set(size_t index, bool state)
{
    size_t mask = 0x1ul << (GetBitIndex(index));
    m_vec[GetCellIndex(index)] &= (~mask) ;
    m_vec[GetCellIndex(index)] |= (mask * state) ;

}


template<size_t SIZE>
size_t BitArray<SIZE>::GetBitIndex(size_t index)const
{
    return ((index % g_bits_in_word));
}


template<size_t SIZE>
size_t BitArray<SIZE>::GetCellIndex(size_t index)const
{
    return (( (index) / g_bits_in_word));
}

template<size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::operator|=(BitArray<SIZE>& other)
{
    transform(m_vec.begin(),m_vec.end(),      //dest
              other.m_vec.begin(),              //src
              m_vec.begin(),                  //dest to copy
              OrEqual);                       //unari function

    return *this;
}

template<size_t SIZE>
bool BitArray<SIZE>::operator==(const BitArray<SIZE>& other)
{
    size_t num_of_cells = GetCellIndex(SIZE);

    for (size_t i = 0; i <= num_of_cells; i++)
    {
        if(m_vec[i] != other.m_vec[i])
        {
            return 1;
        }
    }
    return 0;
}

template<size_t SIZE>
bool BitArray<SIZE>::operator!=( const BitArray<SIZE>& other)
{
    return (!(*this == other));
}

template<size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::operator&=(const BitArray<SIZE>& other)
{
    size_t num_of_cells = GetCellIndex(SIZE);

    for (size_t i = 0; i <= num_of_cells; i++)
    {
        m_vec[i] &= other.m_vec[i];
    }

    return *this;
}

template<size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::operator^=(const BitArray<SIZE>& other)
{
    transform(m_vec.begin(),m_vec.end(),
              other.m_vec.begin(),
              m_vec.begin(),
              XorEqual);
    return *this;
}

template<size_t SIZE>
void BitArray<SIZE>::FlipBit(size_t index)
{
    size_t temp = 0x1ul;

    temp <<= GetBitIndex(index);
    m_vec[GetCellIndex(index)] ^= temp;
}

template<size_t SIZE>
void BitArray<SIZE>::FlipAll()
{
    for_each(m_vec.begin(),m_vec.end(), FlipAllBits);
    m_vec[GetCellIndex(SIZE)] <<= (g_bits_in_word - (GetBitIndex(SIZE))); //clean garbage values
    m_vec[GetCellIndex(SIZE)] >>= (g_bits_in_word - (GetBitIndex(SIZE)));
}

template<size_t SIZE>
bool BitArray<SIZE>::operator[](size_t index) const
{
    size_t mask = 0x1ul;
    mask <<= GetBitIndex(index);
    return (m_vec[GetCellIndex(index)] & mask);
}

template<size_t SIZE>
typename BitArray<SIZE>::BitProxy BitArray<SIZE>::operator[](size_t index)
{
    return BitProxy(*this, index);
}

template<size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::operator<<=(size_t num_shifts)
{
//For shifts that are larger then 64 , copying all the whole chuncks

    for (size_t i = 0; i < GetCellIndex(num_shifts); i++)
    {
          m_vec.insert(m_vec.begin(), 0x0ul);
          m_vec.erase(m_vec.end() - 1);
    }

    for_each(m_vec.begin(), m_vec.end(), ShiftAndAssign( GetBitIndex (num_shifts)));

    return *this;
}

template<size_t SIZE>
size_t BitArray<SIZE>::CountTrueBits()const
{
    //initialize Lut if needed
    if(!g_lut[1])
    {
        for (size_t i = 0; i < (g_bits_in_byte * g_bits_in_word); i++)
        {
            char temp = i;
            temp = ((temp & 0x55)) + ((temp >> 1)& 0x55);
            temp = ((temp & 0x33)) + ((temp >> 2)& 0x33);
            temp = ((temp & 0x0F)) + ((temp >> 4)& 0x0F);

            g_lut[i] = temp;
        }
    }

    size_t counter = 0;

    size_t last_cell_index = GetCellIndex(SIZE);

    for (size_t i = 0; i < last_cell_index; i++)
    {
        size_t temp = m_vec[i];

        for (size_t j = 0; j < g_bits_in_byte; j++)
        {
            counter += g_lut[temp];
            temp >>= (g_bits_in_byte);
        }
    }
    return counter;
}


/***********************************
*****BitArray<SIZE>::BitProxy*******
***********************************/
    template<size_t SIZE>
    class BitArray<SIZE>::BitProxy
    {
    public:
        explicit BitProxy(BitArray& origen, int index);

        bool operator=(bool state);                    //a[2]=true;
        bool operator=(const BitProxy& other);          //a[4] = a[5]

        operator bool()const;
    private:
        BitArray<SIZE>& m_ba;
        size_t m_index;
    };

    template<size_t SIZE>
    BitArray<SIZE>::BitProxy::BitProxy(BitArray& origen, int index):
                              m_ba(origen), m_index(index)
    {}

    template<size_t SIZE>
    bool BitArray<SIZE>::BitProxy::operator=(bool state)
    {
        m_ba.Set(m_index,state);
        return state;
    }

    template<size_t SIZE>
    bool BitArray<SIZE>::BitProxy::operator=(const BitProxy& other)
    {
        m_ba.Set(m_index, other.m_ba.Get(m_index));
        return m_ba.Get(m_index);
    }

    template<size_t SIZE>
    BitArray<SIZE>::BitProxy::operator bool()const
    {
        return m_ba.Get(m_index);
    }

    /*************************
    *****Functor*************
    *************************/
    template<size_t SIZE>
    struct BitArray<SIZE>::ShiftAndAssign
    {
        ShiftAndAssign(size_t num_to_shift);
        void operator()(size_t& cell);
        size_t m_num_to_shift;
        size_t m_my_msb;
        size_t m_last_msb;
        size_t m_mask;
    };

    template<size_t SIZE>
    BitArray<SIZE>::ShiftAndAssign::ShiftAndAssign(size_t num_to_shift):
    m_num_to_shift(num_to_shift),
    m_my_msb(0),m_last_msb(0)
    {
        m_mask = 0x1ul;
        for (size_t i = 0; i < m_num_to_shift; i++)
        {
            m_mask |= (m_mask << 1);
        }
        m_mask <<= (g_bits_in_word - m_num_to_shift);
    }

    template<size_t SIZE>
    void BitArray<SIZE>::ShiftAndAssign::operator()(size_t& cell)
    {
        m_my_msb = cell & m_mask;//save msb

        cell <<= m_num_to_shift;
        cell |= m_last_msb;

        m_last_msb = (m_my_msb >> (g_bits_in_word - m_num_to_shift));  //my msb are gonna be the next cell Lsb
    }
    /*************************
    *****Free Functions*******
    *************************/
    size_t OrEqual(const size_t& ba1, const size_t& ba2)
    {
        return (ba2 | ba1);
    }
    size_t XorEqual(const size_t& ba1, const size_t& ba2)
    {
        return (ba2 ^ ba1);
    }

    void FlipAllBits(size_t& ba)
    {
        (ba ^= 0xFFFFFFFFFFFFFFFF);
    }
}//ilrd

#endif
