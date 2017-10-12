#include "joytime-input-hidapi.hpp"
#include "joytime-input-hidapi.h"
#include <vector>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

JOYTIME_INPUT_HIDAPI_EXPORT Joytime_Controller** Joytime_scanForControllers(int* resultingSize) {
  std::vector<Joytime::Controller> controllers = Joytime::scanForControllers();

  int size = controllers.size();
  (*resultingSize) = size;

  Joytime_Controller** buf = (Joytime_Controller**)(malloc(size * sizeof(Joytime::Controller*)));
  for (int i = 0; i < size; i++) {
    buf[i] = (Joytime_Controller*)(new Joytime::Controller(controllers[i]));
  }

  return buf;
};

JOYTIME_INPUT_HIDAPI_EXPORT void Joytime_initController(Joytime_Controller* _controller, bool calibrate) {
  Joytime::Controller* controller = (Joytime::Controller*)_controller;
  Joytime::initController(controller, calibrate);
};

#ifdef __cplusplus
}
#endif /* __cplusplus */
