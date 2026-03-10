#include "core/plugin_manager.h"
#include <dlfcn.h>
#include <iostream>

namespace Telemetry {

PluginManager::PluginManager() = default;

PluginManager::~PluginManager() {
#ifdef __linux__
    for (auto& handle : dynamic_handles_) {
        if (handle.second) {
            dlclose(handle.second);
        }
    }
#endif
    static_plugins_.clear();
}

bool PluginManager::load_plugin(const std::string& name, const std::string& library_path) {
#ifdef __linux__
    if (dynamic_handles_.find(name) != dynamic_handles_.end()) {
        std::cerr << "Plugin" << name << "already loaded" << std::endl;
        return false;
    }

    void* handle = dlopen(library_path.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Failed to load plugin" << name << ": " << dlerror() << std::endl;
        return false;
    }

    PluginCreateFunc create_func = reinterpret_cast<PluginCreateFunc>(
        dlsym(handle, "create_plugin"));

    if (!create_func) {
        std::cerr << "Failed to find create_plugin function in " << name << std::endl;
        dlclose(handle);
        return false;
    }

    PluginBase* plugin = create_func();
    if (!plugin) {
        std::cerr << "Failed to create plugin instance for " << name << std::endl;
        dlclose(handle);
        return false;
    }

    static_plugins_[name] = std::unique_ptr<PluginBase>(plugin);
    dynamic_handles_[name] = handle;

    plugin->initialize();
    return true;
#else
    std::cerr << "Dynamic plugin loading not supported on this platform" << std::endl;
    return false;
#endif
}

bool PluginManager::unload_plugin(const std::string& name) {
    auto static_it = static_plugins_.find(name);
    if (static_it == static_plugins_.end()) {
        return false;
    }

    static_it->second->shutdown();
    static_plugins_.erase(static_it);

#ifdef __linux__
    auto dynamic_it = dynamic_handles_.find(name);
    if (dynamic_it != dynamic_handles_.end()) {
        dlclose(dynamic_it->second);
        dynamic_handles_.erase(dynamic_it);
    }
#endif

    // Remove from processing order
    auto order_it = std::find(processing_order_.begin(), processing_order_.end(), name);
    if (order_it != processing_order_.end()) {
        processing_order_.erase(order_it);
    }

    return true;
}

void PluginManager::register_plugin(const std::string& name, std::unique_ptr<PluginBase> plugin) {
    if (static_plugins_.find(name) != static_plugins_.end()) {
        std::cerr << "Plugin " << name << " already registered" << std::endl;
        return;
    }

    plugin->initialize();
    static_plugins_[name] = std::move(plugin);

    // Add to processing order if not already there
    if (std::find(processing_order_.begin(), processing_order_.end(), name) == processing_order_.end()) {
        processing_order_.push_back(name);
    }
}

TelemetryBatch PluginManager::process_pipeline(const TelemetryBatch& input) {
    TelemetryBatch output = input;

    for (const auto& plugin_name : processing_order_) {
        auto it = static_plugins_.find(plugin_name);
        if (it != static_plugins_.end() && it->second->is_enabled()) {
            output = it->second->process(output);
        }
    }

    return output;
}

std::vector<std::string> PluginManager::get_plugin_names() const {
    std::vector<std::string> names;
    for (const auto& pair : static_plugins_) {
        names.push_back(pair.first); 
    }
    return names;
}

PluginBase* PluginManager::get_plugin(const std::string& name) {
    auto it = static_plugins_.find(name);
    if (it != static_plugins_.end()) {
        return it->second.get();
    }
    return nullptr;
}

void PluginManager::set_processing_order(const std::vector<std::string>& order) {
    processing_order_ = order;
}

void PluginManager::clear_pipeline() {
    processing_order_.clear();
}
} //namespace Telemetry