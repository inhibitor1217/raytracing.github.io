#pragma once

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
 public:
  sphere(const point3& center, double radius)
      : center(center), radius(std::fmax(0, radius)) {}

  bool hit(const ray& r, double t_min, double t_max,
           hit_record& rec) const override {
    vec3 oc = center - r.origin();
    double a = r.direction().length_squared();
    double h = dot(oc, r.direction());
    double c = oc.length_squared() - radius * radius;
    double discriminant = h * h - a * c;

    if (discriminant < 0) {
      return false;
    }

    double sqrt_d = std::sqrt(discriminant);
    double root = (h - sqrt_d) / a;
    if (root < t_min || t_max < root) {
      root = (h + sqrt_d) / a;
      if (root < t_min || t_max < root) {
        return false;
      }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
  }

 private:
  point3 center;
  double radius;
};
