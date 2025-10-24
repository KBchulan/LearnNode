#ifndef CANDYITEM_H
#define CANDYITEM_H

#include <QString>
#include <type_traits>

class CandyItem
{
public:
    explicit CandyItem(const QString &name, double price);

    template <typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, T>::type getPrice() const
    {
        return static_cast<T>(m_price) * m_quantity;
    }

    QString getName() const
    {
        return m_name;
    }

    std::uint32_t getQuantity() const 
    {
        return m_quantity;
    }

    void setQuantity(std::uint32_t quantity) 
    { 
        m_quantity = quantity; 
    }

    void incrementQuantity() 
    {
        ++m_quantity; 
    }

    void decrementQuantity()
    {
        if (m_quantity > 0)
            --m_quantity;
    }

private:
    QString m_name;
    double m_price;
    std::uint32_t m_quantity{1};
};

#endif // CANDYITEM_H