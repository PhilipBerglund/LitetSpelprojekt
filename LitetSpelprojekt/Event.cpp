#include "Event.h"

std::vector<std::pair<X, void*>> Event::reset = {};
std::vector<std::pair<X, void*>> Event::stateChange = {};
std::vector<std::pair<X, void*>> Event::buttonDown = {};
std::vector<std::pair<X, void*>> Event::leftClick = {};
std::vector<std::pair<X, void*>> Event::mouseMove = {};
std::vector<std::pair<X, void*>> Event::W;
std::vector<std::pair<X, void*>> Event::A;
std::vector<std::pair<X, void*>> Event::S;
std::vector<std::pair<X, void*>> Event::D;
std::vector<std::pair<X, void*>> Event::E;
std::vector<std::pair<X, void*>> Event::escape;
EventType Event::currentEvent;