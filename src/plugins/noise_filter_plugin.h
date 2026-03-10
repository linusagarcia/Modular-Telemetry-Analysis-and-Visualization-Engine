#ifndef NOISE_FILTER_PLUGIN_H
#define NOISE_FILTER_PLUGIN_H

#include "core/plugin_base.h"
#include <random>

namespace Telemetry {

class NoiseFilterPlugin : public PluginBase {
public:
    NoiseFilterPlugin();
    ~NoiseFilterPlugin() override = default;

    std::string get_name() const override { return "NoiseFilter"; }
    std::string get_version() const override {return "1.0.0";}

    void inialize() override;
    void shutdown() override;

    TelemetryBatch process(const TelemtryBatch &input) override;

    void initialize() override:
    void shutdown() override;

    TelemetryBatch process(const TelemtryBatch &input) override;

    void set_noise
}
}


