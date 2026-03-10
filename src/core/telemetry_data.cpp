#include "core/telemetry_data.h"

namespace Telemetry {

TelemetryPoint::TelemetryPoint()
    : track_id(0), timestamp(0.0), x(0.0), y(0.0), z(0.0),
      vx(0.0), vy(0.0), vz(0.0), signal_strength(0.0),
      sensor_type(SensorType::UNKOWN), status(TrackStatus::NEW),
      sensor_id(0) {}

TelemetryPoint::TelemetryPoint(uint64_t id, double ts, double px, double py, double pz)
    : track_id(id), timestamp(ts), x(px), y(py), z(pz),
      vx(0.0), vy(0.0), vz(0.0), signal_strength(1.0),
      sensor_type(SensorType::RADAR), status(TrackStatus::NEW),
      sensor_id(0) {}

std::string TelemetryPoint::to_string() const {
    std::stringstream ss;
    ss << "Track[" << track_id << "] "
       << "Pos(" << x << "," << y << "," << z << ") "
       << "Vel(" << vx << "," << vy << "," << vz << ") "
       << "Time: " << timestamp << " Status: "
       << static_cast<int>(status);
    return ss.str();
}

bool TelemetryPoint::is_valid() const {
    return !std::isnan(x) && !std::isnan(y) && !std::isnan(z) &&
           !std::isinf(x) && !std::isinf(y) && !std::isinf(z);
}

TelemetryBatch::TelemetryBatch() : start_time_(0.0), end_time_(0.0) {}

TelemetryBatch::TelemetryBatch(size_t capacity) {
    points_.reserve(capacity);
    start_time_ = 0.0;
    end_time_ = 0.0;
}

void TelemetryBatch::add_point(const TelemetryPoint& point) {
    if (points_.empty()) {
        start_time_ = point.timestamp;
        end_time_ = point.timestamp;
    } else {
        start_time_ = std::min(start_time_, point.timestamp);
        end_time_ = std::max(end_time_, point.timestamp);
    }
    points_.push_back(point);
}

void TelemetryBatch::clear() {
    points_.clear();
    start_time_ = 0.0;
    end_time_ = 0.0;
}

bool TelemetryBatch::empty() const {
    return points_.empty();
}

size_t TelemetryBatch::size() const {
    return points_.size(); 
}

const std::vector<TelemetryPoint>& TelemetryBatch::get_points() const {
    return points_;
}

std::vector<TelemetryPoint>& TelemetryBatch::get_points() {
    return points_;
}

double TelemetryBatch::get_start_time() const {
    return start_time_;
}

double TelemetryBatch::get_end_time() const {
    return end_time_;
}

} // namespace Telemetry