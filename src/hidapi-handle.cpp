#include "joytime-input-hidapi.hpp"
#include "hidapi.h"

Joytime::HIDAPI::Handle::Handle(Joytime::HIDAPI::ConnectionType connect, hid_device* dev) {
  hidapiHandle = dev;
  connection = connect;
};
