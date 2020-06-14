#ifndef __JAMSIM_DBG_PRINT_H__
#define __JAMSIM_DBG_PRINT_H__

/**
 * @brief JamsimDbgPrint
 *    print to a 2k circular buffer.
 * 
 */
class JamsimDbgPrint 
{
public:
    JamsimDbgPrint();
    virtual ~JamsimDbgPrint() {};
    /**
     * @brief printf to a debug circular buffer.
     * 
     * @param format 
     * @param ... 
     */
    void printf(const char *format, ...);

    /**
     * @brief hasData -- return if we have data to print
     * 
     * @return true -- true if we have data
     * @return false -- false no date
     */
    bool hasData() {return(*_bufferStart != 0);}
    /**
     * @brief Get the Print Buffer 
     *    return a printbuffer that we can write to the output port
     * 
     * @return char* printbuffer
     */
    char *getPrintBuffer() { return(_bufferStart);}

    /**
     * @brief clear the print buffer.
     * 
     */
    void clear();

protected:
    enum {
        HALF_BUFFER_SIZE=2*1024
    };
    char _bufferStart[HALF_BUFFER_SIZE*2];
    char *_buffer;
    //unsigned _written;


private:

};



#endif
