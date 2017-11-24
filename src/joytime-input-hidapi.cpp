#include <vector>
#include <cstdint>
#include <iostream>
#include "joytime-input-hidapi.hpp"
#include "joytime-core.hpp"
#include "hidapi.h"

void transmitBuffer(void* _handle, uint8_t* buffer, int size) {
  Joytime::HIDAPI::Handle* handle = static_cast<Joytime::HIDAPI::Handle*>(_handle);

  hid_write(handle->hidapiHandle, buffer, size);
};

uint8_t* receiveBuffer(void* _handle, int desiredSize, int* bytesRead) {
  Joytime::HIDAPI::Handle* handle = static_cast<Joytime::HIDAPI::Handle*>(_handle);

  uint8_t* buf = (uint8_t*)(malloc(sizeof(uint8_t) * desiredSize));

  *bytesRead = hid_read(handle->hidapiHandle, buf, desiredSize);

  return buf;
};

void freeBuffer(uint8_t* buf) {
  free(buf);
};

JOYTIME_INPUT_HIDAPI_EXPORT std::vector<Joytime::Controller> Joytime::scanForControllers() {
  std::vector<Joytime::Controller> controllers;
  hid_device_info* devices;
  hid_device_info* currentDevice;

  hid_init();

  devices = hid_enumerate(vendor, 0);
  currentDevice = devices;
  while (currentDevice) {
    ControllerType type;
    HIDAPI::ConnectionType connectionType;
    bool foundOne = false;
    switch (currentDevice->product_id) {
      case (uint16_t)(Joytime::ProductIDs::LeftJoycon):
        type = Joytime::ControllerType::LeftJoycon;
        // assume Bluetooth for now
        connectionType = Joytime::HIDAPI::ConnectionType::Bluetooth;
        foundOne = true;
        break;
      case (uint16_t)(Joytime::ProductIDs::RightJoycon):
        type = Joytime::ControllerType::RightJoycon;
        // assume Bluetooth for now
        connectionType = Joytime::HIDAPI::ConnectionType::Bluetooth;
        foundOne = true;
        break;
      case (uint16_t)(Joytime::ProductIDs::Pro):
        type = Joytime::ControllerType::Pro;
        // assume Bluetooth for now
        connectionType = Joytime::HIDAPI::ConnectionType::Bluetooth;
        foundOne = true;
        break;
    }
    if (foundOne) {
      controllers.push_back(Joytime::Controller(type, static_cast<void*>(new Joytime::HIDAPI::Handle(connectionType, hid_open(vendor, currentDevice->product_id, currentDevice->serial_number))), &transmitBuffer, &receiveBuffer, &freeBuffer));
    }
    currentDevice = currentDevice->next;
  }
  hid_free_enumeration(devices);

  return controllers;
};

JOYTIME_INPUT_HIDAPI_EXPORT void Joytime::initController(Joytime::Controller* controller, bool calibrate) {
  Joytime::HIDAPI::Handle* handle = static_cast<Joytime::HIDAPI::Handle*>(controller->handle);
  if (handle->connection == Joytime::HIDAPI::ConnectionType::USB) {
    // USB requires some extra initializition,
    // but that's unimplemented for now
  }
  hid_set_nonblocking(handle->hidapiHandle, 1);

  controller->initialize(calibrate);
};
