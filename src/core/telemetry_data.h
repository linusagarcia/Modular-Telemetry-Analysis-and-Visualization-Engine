#ifndef TELEMETRY_DATA_H
#define TELEMETRY_DATA_H

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <sstream>
#include <cmath>

namespace Telemetry {

    // Telemetry data types
    enum class SensorType : uint8_t {
        RADAR = 0,
        LIDAR = 1,
        GPS = 2,
        IMU = 3,
        CAMERA = 4,
        UNKOWN = 255
    };

    enum class TrackStatus : uint8_t {
        NEW = 0,
        UPDATED = 1,
        COASTED = 2,
        DROPPED = 3
    };

    struct TelemetryPoint {
        uint64_t track_id;
        double timestamp;

        
        // Position 
        double x;
        double y;
        double z;

        
        // Velocity 
        double vx;
        double vy;
        double vz;

        
        // Additional data
        double signal_strength;
        SensorType sensor_type;
        TrackStatus status;
        uint32_t sensor_id;

        TelemetryPoint();
        TelemetryPoint(uint64_t id, double ts, double px, double py, double pz);

        std::string to_string() const;
        bool is_valid() const;

    };

    class TelemetryBatch {
    public: 
        TelemetryBatch();
        explicit TelemetryBatch(size_t capacity);
        
        void add_point(const TelemetryPoint& point);
        void clear();
        bool empty() const;
        size_t size() const;

        const std::vector<TelemetryPoint>& get_points() const;
        std::vector<TelemetryPoint>& get_points();

        double get_start_time() const;
        double get_end_time() const;

    private:
        std::vector<TelemetryPoint> points_;
        double start_time_;
        double end_time_;
    };
} // namespac Telemtry

#endif // TELEMETRY_DATA_H