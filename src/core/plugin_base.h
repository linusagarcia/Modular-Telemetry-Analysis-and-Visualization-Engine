#ifndef PLUGIN_BASE_H
#define PLUGIN_BASE_H

#include <memory>
#include <string>
#include "core/telemetry_data.h"

namespace Telemetry {

class PluginBase {

public: 
    virtual ~PluginBase() = default;

    virtual std::string get_name() const = 0;
    virtual std::string get_version() const = 0;
    virtual void initialize() = 0;
    virtual void shutdown() = 0;

    virtual TelemetryBatch process(const TelemetryBatch& input) = 0;

    virtual bool is_enabled() const { return enabled_; }
    virtual void set_enabled(bool enabled) 
}
}