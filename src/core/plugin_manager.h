#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include "core/plugin_base.h"
#include "core/telemetry_data.h"

namespace Telemetry {

class PluginManager {
public:
    PluginManager();
    ~PluginManager();

    // Prevent copying
    PluginManager(const PluginManager&) = delete;
    PluginManager& operator=(const PluginManager&) = delete;

    // Plugin management
    bool load_plugin(const std::string& name, const std::string& library_path = "");
    bool unload_plugin(const std::string& name);

    // Static plugin registartion
    void register_plugin(const std::string& name, std::unique_ptr<PluginBase> plugin);

    // Processing pipeline
    TelemetryBatch process_pipeline(const TelemetryBatch& input);

    // Plugin info
    std::vector<std::string> get_plugin_names() const;
    PluginBase* get_plugin(const std::string& name);

    // Pipeline configuration 
    void set_processing_order(const std::vector<std::string>& order);
    void clear_pipeline();

private:
    std::unordered_map<std::string, std::unique_ptr<PluginBase>> static_plugins_;
    std::vector<std::string, std::unique_ptr<PluginBase>> static_plugins_;

#ifdef __linux__
    std::unordered_map<std::string, void*> dynamic_handles_;
#endif
};

} // namespace Telemetry

#endif // PLUGIN_MANAGER_H