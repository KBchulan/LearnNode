/******************************************************************************
 *
 * @file       Vector2.hpp
 * @brief      二维向量的实现
 *
 * @author     KBchulan
 * @date       2025/04/06
 * @history
 ******************************************************************************/

#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <cmath>

namespace math {

class Vector2 {
public:
  Vector2() = default;
  ~Vector2() = default;

  explicit Vector2(double vec_x, double vec_y) : x_(vec_x), y_(vec_y) {}

  [[nodiscard]] double x() const { return x_; }
  [[nodiscard]] double y() const { return y_; }

  Vector2 operator+(const Vector2 &other) const {
    return Vector2(this->x_ + other.x_, this->y_ + other.y_);
  }

  void operator+=(const Vector2 &other) {
    this->x_ += other.x_;
    this->y_ += other.y_;
  }

  Vector2 operator-(const Vector2 &other) const {
    return Vector2(this->x_ - other.x_, this->y_ - other.y_);
  }

  void operator-=(const Vector2 &other) {
    this->x_ -= other.x_;
    this->y_ -= other.y_;
  }

  Vector2 operator*(const double &scalar) const {
    return Vector2(this->x_ * scalar, this->y_ * scalar);
  }

  void operator*=(const double &scalar) {
    this->x_ *= scalar;
    this->y_ *= scalar;
  }

  double operator*(const Vector2 &other) const {
    return (this->x_ * other.x_) + (this->y_ * other.y_);
  }

  Vector2 operator/(const double &scalar) const {
    return Vector2(this->x_ / scalar, this->y_ / scalar);
  }

  void operator/=(const double &scalar) {
    this->x_ /= scalar;
    this->y_ /= scalar;
  }

  [[nodiscard]] double length() const {
    return std::sqrt((this->x_ * this->x_) + (this->y_ * this->y_));
  }

  [[nodiscard]] Vector2 normalize() const {
    double length = this->length();
    return Vector2(this->x_ / length, this->y_ / length);
  }

  auto operator<=>(const Vector2 &other) const {
    return this->length() <=> other.length();
  }
	
  bool operator==(const Vector2 &other) const{
    return this->x_ == other.x_ && this->y_ == other.y_;
  }

  bool operator!=(const Vector2 &other) const{
    return this->x_ != other.x_ || this->y_ != other.y_;
  }
  
private:
  double x_{0};
  double y_{0};
};

} // namespace math

#endif // VECTOR2_HPP
