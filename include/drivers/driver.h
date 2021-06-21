#ifndef __CITOS_DRIVERS_DRIVER_H
#define __CITOS_DRIVERS_DRIVER_H

namespace citos
{
  namespace drivers
  {

    class Driver
    {
    public:
      Driver();
      ~Driver();

      virtual void Activate();
      virtual int Reset();
      virtual void Deactivate();
    };

    class DriverManager
    {
    private:
      Driver *drivers[256];
      int numDrivers;

    public:
      DriverManager();
      void AddDriver(Driver *driver);
      void ActivateAll();
    };
    
  } // namespace drivers

} // namespace citos

#endif