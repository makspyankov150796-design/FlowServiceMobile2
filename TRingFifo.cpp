//---------------------------------------------------------------------------

#pragma hdrstop
#include "TRingFifo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
TRingFifo::TRingFifo(uint32_t fifoSize)
{
  wrIndex   = 0;
  rdIndex   = 0;
  Size      = 0;
  FreeSpace = 0;
  Cs        = nullptr;

  buffer = reinterpret_cast<uint8_t*>(malloc(fifoSize));

  if(buffer != nullptr)
  {
    Size      = fifoSize;
    FreeSpace = fifoSize;
    Cs        = new TCriticalSection;
  }
}
//---------------------------------------------------------------------------
TRingFifo::~TRingFifo()
{
  if(buffer != nullptr)
  {
    free(buffer);
    buffer = nullptr;
  }

  if(Cs != nullptr)
  {
    delete Cs;
    Cs = nullptr;
  }
}
//---------------------------------------------------------------------------
uint32_t TRingFifo::GetSize(void) const
{
  return Size;
}
//---------------------------------------------------------------------------
uint32_t TRingFifo::GetFreeSpaceH(void) const
{
  uint32_t retValue = 0;

  if(Size)
  {
	Cs->Enter();
    retValue = FreeSpace;
    Cs->Leave();
  }

  return retValue;
}
//---------------------------------------------------------------------------
uint32_t TRingFifo::GetBusySpace(void) const
{
  uint32_t retValue = 0;

  if(Size)
  {
    Cs->Enter();
    retValue = Size - FreeSpace;
    Cs->Leave();
  }

  return retValue;
}
//---------------------------------------------------------------------------
bool TRingFifo::IsEmpty(void) const
{
  return (Size == FreeSpace);
}
//---------------------------------------------------------------------------
bool TRingFifo::IsFull(void) const
{
  return (FreeSpace == 0);
}
//---------------------------------------------------------------------------
bool TRingFifo::Push(void *vpData, uint32_t dataLen)
{
  bool success = false;
  uint8_t *workPtr = reinterpret_cast<uint8_t*>(vpData);

  do
  {
    if(0 == Size) break;

    if(0 == dataLen || nullptr == vpData || nullptr == workPtr)
      break;

    if(dataLen > FreeSpace)
      break;

    Cs->Enter();

    for(uint32_t i = 0; i < dataLen; i++)
    {
      if(wrIndex >= Size) wrIndex = 0;

      buffer[wrIndex] = *workPtr;
      workPtr++;
      wrIndex++;
    }

    FreeSpace -= dataLen;

    Cs->Leave();

    success = true;
  } while(0);

  return success;
}
//---------------------------------------------------------------------------
bool TRingFifo::Pull(void *vpDst, uint32_t dataLen, uint32_t &BytesPulled)
{
  bool success = false;
  uint8_t *dstPtr = reinterpret_cast<uint8_t*>(vpDst);
  uint32_t busySpace = 0;

  do
  {
    if(0 == Size) break;

    if(nullptr == vpDst || nullptr == dstPtr || 0 == dataLen)
      break;

    Cs->Enter();

    busySpace   = Size - FreeSpace;
    BytesPulled = (dataLen > busySpace) ? busySpace : dataLen;

    for(uint32_t i = 0; i < BytesPulled; i++)
    {
      if(rdIndex >= Size)
        rdIndex = 0;

      *dstPtr = buffer[rdIndex];

      rdIndex++;
      dstPtr++;
    }

    FreeSpace += BytesPulled;

    Cs->Leave();

    success = true;
  } while(0);

  return success;
}
//---------------------------------------------------------------------------
