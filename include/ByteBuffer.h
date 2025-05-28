#ifndef _BYTE_BUFFER_H_
#define _BYTE_BUFFER_H_

#include <vector>
#include <string>
#include <cstring>
#include <cstdint>
#include <cassert>

/**
 * @brief Typedef for byte
 */
typedef uint8_t byte;

/**
 * @brief Class for handling binary data
 * 
 * ByteBuffer provides functionality for reading and writing binary data,
 * with support for different data types and endianness.
 */
class ByteBuffer {
public:
    /**
     * @brief Default constructor
     */
    ByteBuffer() : m_rpos(0), m_wpos(0) {}
    
    /**
     * @brief Constructor with initial size
     * 
     * @param size Initial size of the buffer
     */
    ByteBuffer(size_t size) : m_rpos(0), m_wpos(0) {
        m_buffer.resize(size);
    }
    
    /**
     * @brief Constructor with initial data
     * 
     * @param data Pointer to initial data
     * @param size Size of initial data
     */
    ByteBuffer(const byte* data, size_t size) : m_rpos(0), m_wpos(size) {
        m_buffer.resize(size);
        memcpy(m_buffer.data(), data, size);
    }
    
    /**
     * @brief Copy constructor
     */
    ByteBuffer(const ByteBuffer& other) : m_rpos(other.m_rpos), m_wpos(other.m_wpos), m_buffer(other.m_buffer) {}
    
    /**
     * @brief Assignment operator
     */
    ByteBuffer& operator=(const ByteBuffer& other) {
        if (this != &other) {
            m_rpos = other.m_rpos;
            m_wpos = other.m_wpos;
            m_buffer = other.m_buffer;
        }
        return *this;
    }
    
    /**
     * @brief Get pointer to the buffer data
     * 
     * @return Pointer to the buffer data
     */
    const byte* contents() const {
        return m_buffer.data();
    }
    
    /**
     * @brief Get the size of the buffer
     * 
     * @return Size of the buffer in bytes
     */
    size_t size() const {
        return m_buffer.size();
    }
    
    /**
     * @brief Clear the buffer
     */
    void clear() {
        m_buffer.clear();
        m_rpos = 0;
        m_wpos = 0;
    }
    
    /**
     * @brief Resize the buffer
     * 
     * @param newSize New size for the buffer
     */
    void resize(size_t newSize) {
        m_buffer.resize(newSize);
    }
    
    /**
     * @brief Get the current read position
     * 
     * @return Current read position
     */
    size_t rpos() const {
        return m_rpos;
    }
    
    /**
     * @brief Set the read position
     * 
     * @param pos New read position
     */
    void rpos(size_t pos) {
        assert(pos <= m_buffer.size());
        m_rpos = pos;
    }
    
    /**
     * @brief Get the current write position
     * 
     * @return Current write position
     */
    size_t wpos() const {
        return m_wpos;
    }
    
    /**
     * @brief Set the write position
     * 
     * @param pos New write position
     */
    void wpos(size_t pos) {
        assert(pos <= m_buffer.size());
        m_wpos = pos;
    }
    
    /**
     * @brief Get the number of unread bytes
     * 
     * @return Number of unread bytes
     */
    size_t remaining() const {
        return m_wpos > m_rpos ? m_wpos - m_rpos : 0;
    }
    
    /**
     * @brief Append data to the buffer
     * 
     * @param data Pointer to data
     * @param size Size of data
     */
    void append(const byte* data, size_t size) {
        if (size == 0) return;
        
        size_t newSize = m_wpos + size;
        if (newSize > m_buffer.size()) {
            m_buffer.resize(newSize);
        }
        
        memcpy(&m_buffer[m_wpos], data, size);
        m_wpos += size;
    }
    
    /**
     * @brief Append data from another ByteBuffer
     * 
     * @param buffer The buffer to append
     */
    void append(const ByteBuffer& buffer) {
        if (buffer.size() == 0) return;
        
        append(buffer.contents(), buffer.size());
    }
    
    /**
     * @brief Append a string to the buffer
     * 
     * @param str The string to append
     */
    void append(const std::string& str) {
        append((const byte*)str.c_str(), str.length());
    }
    
    /**
     * @brief Put a value at a specific position
     * 
     * @param pos Position to put the value
     * @param value Value to put
     */
    template<typename T>
    void put(size_t pos, T value) {
        assert(pos + sizeof(T) <= m_buffer.size());
        memcpy(&m_buffer[pos], &value, sizeof(T));
    }
    
    /**
     * @brief Read data from the buffer
     * 
     * @param dest Destination buffer
     * @param size Number of bytes to read
     */
    void read(byte* dest, size_t size) {
        assert(m_rpos + size <= m_buffer.size());
        memcpy(dest, &m_buffer[m_rpos], size);
        m_rpos += size;
    }
    
    /**
     * @brief Write data to the buffer
     * 
     * @param src Source data
     * @param size Number of bytes to write
     */
    void write(const byte* src, size_t size) {
        size_t newSize = m_wpos + size;
        if (newSize > m_buffer.size()) {
            m_buffer.resize(newSize);
        }
        
        memcpy(&m_buffer[m_wpos], src, size);
        m_wpos += size;
    }
    
    /**
     * @brief Read a string from the buffer
     * 
     * @param str String to store the result
     */
    void readString(std::string& str) {
        str.clear();
        while (m_rpos < m_wpos) {
            char c = read<char>();
            if (c == 0)
                break;
            str.push_back(c);
        }
    }
    
    /**
     * @brief Write a string to the buffer
     * 
     * @param str String to write
     */
    void writeString(const std::string& str) {
        if (str.empty()) {
            write<char>(0);
            return;
        }
        
        write((const byte*)str.c_str(), str.length());
        write<char>(0); // Null terminator
    }
    
    /**
     * @brief Read a value from the buffer
     * 
     * @return The read value
     */
    template<typename T>
    T read() {
        T r;
        read((byte*)&r, sizeof(T));
        return r;
    }
    
    /**
     * @brief Write a value to the buffer
     * 
     * @param value Value to write
     */
    template<typename T>
    void write(T value) {
        write((const byte*)&value, sizeof(T));
    }
    
    /**
     * @brief Extract a value from the buffer
     * 
     * @param value Reference to store the extracted value
     */
    template<typename T>
    void operator>>(T& value) {
        value = read<T>();
    }
    
    /**
     * @brief Insert a value into the buffer
     * 
     * @param value Value to insert
     */
    template<typename T>
    void operator<<(T value) {
        write<T>(value);
    }

private:
    size_t m_rpos;             ///< Current read position
    size_t m_wpos;             ///< Current write position
    std::vector<byte> m_buffer; ///< Internal buffer
};

#endif // _BYTE_BUFFER_H_