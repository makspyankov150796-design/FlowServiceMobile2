//---------------------------------------------------------------------------
#ifndef TRingFifoH
#define TRingFifoH
//---------------------------------------------------------------------------
#include <stdint.h>
#include <System.SyncObjs.hpp>
//---------------------------------------------------------------------------
class TRingFifo
{
  private:
    uint8_t *buffer;

    uint32_t Size;
    uint32_t FreeSpace;

    uint32_t wrIndex;
    uint32_t rdIndex;

    TCriticalSection *Cs;

  public:
    TRingFifo(uint32_t fifoSize);
    ~TRingFifo();

	uint32_t GetSize(void) const;

    #ifdef _WIN32
     #define RT
    #endif
	uint32_t GetFreeSpaceH(void) const;

	uint32_t GetBusySpace(void) const;

	bool IsEmpty(void) const;

	bool IsFull(void) const;

    bool Push(void *vpData, uint32_t dataLen);

    bool Pull(void *vpDst, uint32_t dataLen, uint32_t &BytesPulled);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#endif
//---------------------------------------------------------------------------
