#pragma once

#include <functional>
#include <cstddef>
#include <unordered_map>
#include <utility>
#include <vector>

namespace fg {
enum class EventType { QuitRequested, PauseToggled, NextDayRequested };
struct Event { EventType type; };

class EventBus {
public:
    using Handler = std::function<void(const Event&)>;

    void subscribe(EventType type, Handler handler) {
        handlers_[type].push_back(std::move(handler));
    }

    void publish(Event event) const {
        const auto found = handlers_.find(event.type);
        if (found == handlers_.end()) return;
        for (const auto& handler : found->second) handler(event);
    }

private:
    struct Hash { std::size_t operator()(EventType type) const { return static_cast<std::size_t>(type); } };
    std::unordered_map<EventType, std::vector<Handler>, Hash> handlers_;
};
}
