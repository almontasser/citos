#ifndef __CITOS_HARDWARE_PCI
#define __CITOS_HARDWARE_PCI

#include <gdt.h>
#include <common/types.h>
#include <hardware/port.h>
#include <hardware/interrupts.h>
#include <drivers/driver.h>

namespace citos
{
  namespace hardware
  {
    using namespace citos::common;
    using namespace citos::drivers;
    using namespace citos::hardware;

    enum BaseAddressRegisterType
    {
      MemoryMapping = 0,
      InputOutput = 1
    };

    class BaseAddressRegister
    {
    public:
      bool prefetchable;
      uint8_t* address;
      uint32_t size;
      BaseAddressRegisterType type;
    };

    class PeripheralComponentInterconnectDeviceDescriptor
    {
    public:
      uint32_t portBase;
      uint32_t interrupt;

      uint16_t bus;
      uint16_t device;
      uint16_t function;

      uint16_t vendor_id;
      uint16_t device_id;

      uint8_t class_id;
      uint8_t subclass_id;
      uint8_t interface_id;

      uint8_t revision;

      PeripheralComponentInterconnectDeviceDescriptor();
      ~PeripheralComponentInterconnectDeviceDescriptor();
    };

    class PeripheralComponentInterconnectController
    {
      Port32Bit dataPort;
      Port32Bit commandPort;

    public:
      PeripheralComponentInterconnectController();
      ~PeripheralComponentInterconnectController();

      uint32_t Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset);
      void Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value);

      bool DeviceHasFunctions(uint16_t bus, uint16_t device);

      void SelectDrivers(DriverManager *driverManager, InterruptManager* interruptManager);
      PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function);
      Driver* GetDriver(PeripheralComponentInterconnectDeviceDescriptor deviceDescriptor, InterruptManager* interruptManager);
      BaseAddressRegister GetBaseAddressRegister(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar);
    };

  } // namespace hardware

} // namespace cit

#endif